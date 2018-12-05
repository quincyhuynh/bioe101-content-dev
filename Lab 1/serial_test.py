import numpy as np
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg

import struct
import pyaudio
from scipy.fftpack import fft

import sys
import serial
import time
import pdb

def sample(buf_size):
        # serial write section
        device.write('s'.encode())
        device.flush()
        time.sleep(0.1)

        # # serial read section
        try:
            serial_buf = device.readline().decode('utf-8').strip().split(',')
            print(serial_buf)
            data, rate = [float(i) for i in serial_buf[:-1]], buf_size/(float(serial_buf[-1]))
            return data, rate
        except Exception as e:
            print(e)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("You must enter in a COM or /dev/ port")
        sys.exit()
    global port, baud, arduino
    port = sys.argv[1]
    print(port)
    baud = 115200
    device = serial.Serial(
                port = port,
                baudrate = baud,
                timeout = 0.1
                )
    for _ in range(5):
        print(sample(1024*4))
    device.close()