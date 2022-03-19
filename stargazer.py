#!/usr/bin/env python -i
# on the rpi
import struct
import serial
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
    ser = serial.Serial('/dev/ttyS1')

    while True:
        request = ser.readline().decode().split()

        if request[0] == 'S':
            _, star, lat, lon = request
            alt, az = get_star_location(star, float(lat), float(lon))
            ser.write(struct.pack('<ff', alt, az))
        elif request[0] == 'P':
            # take picture
            ...
