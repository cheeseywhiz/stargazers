#!/usr/bin/env python -i
# on the rpi
import struct
import serial
import time
from skyfield.api import load
from skyfield.api import N, E, wgs84


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


def handle_requests():
    #ser = serial.Serial('/dev/ttyS1')
    ser = serial.Serial('COM33', 115200, timeout=999)

    while True:
        #print("before readline")
        request = ser.readline()

        if not request:
            time.sleep(0.1)
            continue

        print(request)
        request = request.decode().split()

        if request[0] == 'S':
            _, star, lat, lon, *extra = request

            if extra:
                print("extra S args:", *extra)
                continue

            try:
                lat, lon = float(lat), float(lon)
            except ValueError:
                print("malformed args:", lat, lon)
                continue

            alt, az = get_star_location(star, lat, lon)
            print("computer calculated:", alt, az)
            time.sleep(.05)
            ser.write(struct.pack('<ff', alt, az))
        elif request[0] == 'P':
            # take picture
            print("TODO:  print request")
            ...
        else:
            print("log:", *request)
            
        time.sleep(.5)

handle_requests()
