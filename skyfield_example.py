#!/usr/bin/env python -i
from skyfield.api import load
from skyfield.api import N, E, wgs84

# Create a timescale and ask the current time.
ts = load.timescale()
t = ts.now()

# Load the JPL ephemeris DE421 (covers 1900-2050).
planets = load('de421.bsp')
earth, mars = planets['earth'], planets['mars']

# What's the position of Mars, viewed from Earth?
astrometric = earth.at(t).observe(mars)
ra, dec, distance = astrometric.radec()

print(f'{ra = }')
print(f'{dec = }')
print(f'{distance = }')

boston = earth + wgs84.latlon(42.3583 * N, -71.0636 * E)
astrometric = boston.at(t).observe(mars)
alt, az, d = astrometric.apparent().altaz()

print(f'{alt.degrees = }')
print(f'{az.degrees = }')
