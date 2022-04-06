#!/usr/bin/env python -i
# on the rpi
import struct
import serial
import time
import io
import numpy as np
import os
from skyfield.api import load
from skyfield.api import N, E, wgs84
from picamera import PiCamera
from PIL import Image

os.chdir('/home/pi/stargazers')

camera = PiCamera()
camera.resolution = (480, 320)

planets = load('de421.bsp')

def get_star_location(star: str, lat: float, lon: float):
    ts = load.timescale()
    t = ts.now()
    earth = planets['earth']
    star = planets[star]
    location = earth + wgs84.latlon(lat * N, lon * E)
    astrometric = location.at(t).observe(star)
    alt, az, _ = astrometric.apparent().altaz()
    return alt.degrees, az.degrees


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


def handle_requests():
    ser = serial.Serial('/dev/ttyS0', baudrate=115200)
    print("starting server")

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
                print("extra S args:", *extra)
                continue

            try:
                lat, lon = float(lat), float(lon)
            except ValueError:
                print("malformed args:", lat, lon)
                continue

            alt, az = get_star_location(star, lat, lon)
            #print("computer calculated:", alt, az)
            ser.write(struct.pack('<ff', alt, az))
        elif request[0] == 'P':
            # take picture
            arr = capture_image()
            time0 = time.perf_counter()
            
            for chunk in chunks(arr.flatten(), 0xffff):
                chunk = bytes(chunk)
                print("first 9 bytes: ", *chunk[:9])
                ser.write(chunk)
                
            time1 = time.perf_counter()
            print(f'image transaction took {time1 - time0} s')
        elif request[0] == '$':
            # gps request
            msg_id, utc_time, latitude, ns_indicator, longitude, ew_indicator, position_fix, n_satellites, hdop, msl_altitude, units1, geoidal_separation, units2, idk, checksum = request.split(',')
            print("gps:", request.split(','))
            
            if latitude:
                latitude = int(latitude[:2]) * float(latitude[2:]) / 60
                if ns_indicator == 'S':
                    latitude *= -1
            else:
                print('no latitude')
                    
            if longitude:
                longitude = int(longitude[:2]) * float(longitude[2:]) / 60
                if ew_indicator == 'W':
                    longitude *= -1
            else:
                print('no longitude')
                        
            print(f"gps: {msg_id=} {utc_time=} {latitude=} {ns_indicator=} {longitude=} {ew_indicator=} {position_fix=} {n_satellites=} {hdop=} {msl_altitude=} {units1=} {geoidal_separation=} {units2=} {idk=} {checksum=}")
        else:
            print(f"log: {request}")
            
            

if __name__ == "__main__":
    handle_requests()
