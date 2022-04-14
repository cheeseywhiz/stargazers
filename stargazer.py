#!/usr/bin/env python -i
# on the rpi
import struct
import serial
import time
import io
import numpy as np
import os
import datetime
from PIL import Image
from skyfield.api import load
from skyfield.api import N, E, wgs84
import wmm2020
from picamera import PiCamera

os.chdir('/home/pi/stargazers')

camera = PiCamera()
camera.resolution = (480, 320)

planets = load('de421.bsp')

# initial time to be set by very first star request
t0 = None
TIME_SCALE_FACTOR = 5


def pack_star_location(status: str, alt: float, azi: float, decl: float):
    return struct.pack('<cfff', status, alt, azi, decl)


def get_star_location(star: str, lat: float, lon: float, t: datetime.datetime):
    print(f'get star location: {star = } {lat = } {lon = } {t = }')
    ts = load.timescale()
    t = ts.from_datetime(t)
    earth = planets['earth']
    star = planets[star]
    location = earth + wgs84.latlon(lat * N, lon * E)
    astrometric = location.at(t).observe(star)
    alt, az, _ = astrometric.apparent().altaz()
    return alt.degrees, az.degrees


def get_declination(lat: float, lon: float):
    data = wmm2020.wmm([lat], [lon], 2022, 0)
    declination = float(data.decl[0][0])
    return declination


def capture_image():
    stream = io.BytesIO()
    camera.start_preview()
    time.sleep(2)
    camera.capture(stream, format='jpeg')
    camera.stop_preview()
    stream.seek(0)
    image = Image.open(stream)
    array = np.asarray(image)
    return array


def chunks(arr, n):
    return [
        arr[i:i + n]
        for i in range(0, len(arr), n)
    ]


def gps_error(msg, ser):
    print(msg)
    ser.write(pack_star_location('V', 0, 0, 0))


def handle_requests():
    ser = serial.Serial('/dev/ttyS0', baudrate=115200)
    print('starting server')

    while True:
        request = ser.readline()

        if not request:
            time.sleep(0.1)
            continue

        print(request)
        request = request.decode()[:-1] # remove trailing whitespace

        if request[0] == 'S':
            _, star, lat, lon, *extra = request.split()

            if extra:
                print('extra S args:', *extra)
                continue

            try:
                lat, lon = float(lat), float(lon)
            except ValueError:
                print('malformed args:', lat, lon)
                continue

            alt, az = get_star_location(star, lat, lon)
            #print('computer calculated:', alt, az)
            ser.write(struct.pack('<ff', alt, az))
        elif request[0] == 'P':
            # take picture
            arr = capture_image()
            time0 = time.perf_counter()

            for chunk in chunks(arr.flatten(), 0xffff):
                chunk = bytes(chunk)
                print('first 9 bytes: ', *chunk[:9])
                ser.write(chunk)

            time1 = time.perf_counter()
            print(f'image transaction took {time1 - time0} s')
        elif request[0] == '$':
            # gps request
            msg_id, utc_time, status, latitude, ns_indicator, longitude, ew_indicator, sog, cog, \
                date, magnetic_variation, mode, checksum, star = request.split(',')
            # NOTE: star field is added on Nucleo, not by gps chip
            print('gps:', request.split(','))

            if msg_id != 'GPRMC':
                gps_error(f'not GPRMC data: {msg_id=}', ser)
                continue

            if status != 'A':
                gps_error(f'not valid: {status=}', ser)
                continue

            if not latitude or len(latitude) != len('DDMM.MMMM'):
                gps_error(f'bad {latitude=}', ser)
                continue

            if not longitude or len(longitude) != len('DDDMM.MMMM'):
                gps_error(f'bad {longitude=}', ser)
                continue

            if not utc_time or len(utc_time) != len('HHMMSS.SSS'):
                gps_error(f'bad {utc_time=}', ser)
                continue

            if not date or len(date) != len('DDMMYY'):
                gps_error(f'bad {date=}', ser)
                continue

            try:
                latitude = int(latitude[:2]) * float(latitude[2:]) / 60
            except ValueError:
                gps_error(f'malformed {latitude=}', ser)
                continue

            if ns_indicator == 'S':
                latitude *= -1

            try:
                longitude = int(longitude[:3]) * float(longitude[3:]) / 60
            except ValueError:
                gps_error(f'malformed {longitude=}', ser)
                continue

            if ew_indicator == 'W':
                longitude *= -1

            try:
                hour = int(utc_time[0:2])
                minute = int(utc_time[2:4])
                second = int(utc_time[4:6])
                millisecond = int(utc_time[7:10])
            except ValueError:
                gps_error(f'malformed {utc_time=}', ser)
                continue

            try:
                day = int(date[0:2])
                month = int(date[2:4])
                year = 2000 + int(date[4:6])
            except ValueError:
                gps_error(f'malformed {date=}', ser)
                continue

            t1 = datetime.datetime(year, month, day, hour, minute, second, millisecond * 1000,
                                   datetime.timezone.utc)
            # set initial time
            nonlocal t0
            if t0 is None:
                t0 = t1
            # accelerate time
            t1 = (t1 - t0) * TIME_SCALE_FACTOR + t0
            alt, az = get_star_location(star, latitude, longitude, t1)
            print('calculated star location:', alt, az)
            decl = get_declination(latitude, longitude)
            ser.write(pack_star_location(status, alt, az, decl))
        else:
            print(f'log: {request}')


if __name__ == '__main__':
    handle_requests()
