#!/usr/bin/env python3

import serial

ser = serial.Serial('/dev/ttyS3', 115200)
while True:
    b, = ser.read(1)
    print(f'{b:02x}', end=' ', flush=True)
