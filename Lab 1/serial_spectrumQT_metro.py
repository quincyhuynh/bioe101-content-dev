import numpy as np
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg

import struct
import pyaudio
from scipy.fftpack import fft

import sys
import serial
import time


class SerialStream(object):
    def __init__(self):

        # pyqtgraph stuff
        pg.setConfigOptions(antialias=True)
        self.traces = dict()
        self.app = QtGui.QApplication(sys.argv)
        self.win = pg.GraphicsWindow(title='Spectrum Analyzer')
        self.win.setWindowIcon(QtGui.QIcon('./spectrum_icon.png'))
        self.win.setWindowTitle('Spectrum Analyzer')
        self.win.setGeometry(5, 115, 1810, 1000)
        self.win.showFullScreen()

        self.tray = QtGui.QSystemTrayIcon()
        self.tray.setIcon(QtGui.QIcon('./spectrum_icon.png'))
        self.tray.setVisible(True)

        # wf_xlabels = [(0, '0'), (2048, '2048'), (4096, '4096')]
        wf_xaxis = pg.AxisItem(orientation='bottom')
        # wf_xaxis.setTicks([wf_xlabels])

        # wf_ylabels = [(0, '0'), (127, '128'), (255, '255')]
        wf_yaxis = pg.AxisItem(orientation='left')
        # wf_yaxis.setTicks([wf_ylabels])

        # sp_xlabels = [
        #     (np.log10(10), '10'), (np.log10(100), '100'),
        #     (np.log10(1000), '1000'), (np.log10(22050), '22050')
        # ]
        sp_xaxis = pg.AxisItem(orientation='bottom')
        # sp_xaxis.setTicks([sp_xlabels])

        self.waveform = self.win.addPlot(
            title='WAVEFORM', row=1, col=1, axisItems={'bottom': wf_xaxis, 'left': wf_yaxis},
        )
        self.spectrum = self.win.addPlot(
            title='SPECTRUM', row=2, col=1, axisItems={'bottom': sp_xaxis},
        )

        # pyaudio stuff
        self.FORMAT = pyaudio.paInt16
        self.CHANNELS = 1
        self.RATE = 16000
        self.CHUNK = 1024*2

        self.p = pyaudio.PyAudio()
        self.stream = self.p.open(
            format=self.FORMAT,
            channels=self.CHANNELS,
            rate=self.RATE,
            input=True,
            output=True,
            frames_per_buffer=self.CHUNK,
        )
        # waveform and spectrum x points

        self.x = np.arange(0, 2 * self.CHUNK, 2)/self.RATE
        self.f = np.linspace(0, self.RATE / 2, self.CHUNK / 2)


    def start(self):
        if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
            QtGui.QApplication.instance().exec_()

    def stop(self):
        QtGui.QApplication.quit()

    def set_plotdata(self, name, data_x, data_y):
        if name in self.traces:
            self.traces[name].setData(data_x, data_y)
        else:
            if name == 'waveform':
                self.traces[name] = self.waveform.plot(pen='c', width=3)
                self.waveform.setYRange(0, 1024, padding=0)
                self.waveform.setXRange(0, max(self.x), padding=0.005)
            if name == 'spectrum':
                self.traces[name] = self.spectrum.plot(pen='m', width=3)
                self.spectrum.setYRange(0, 1024, padding=0)
                self.spectrum.setXRange(
                    0, self.RATE / 2, padding=0.005)

    def update(self):
        wf_data = np.array(self.sample(self.CHUNK))
        self.x = np.arange(0, 2 * self.CHUNK, 2)/self.RATE
        print(self.x)
        self.set_plotdata(name='waveform', data_x=self.x, data_y=wf_data,)
        y_fft = fft(wf_data)
        y_fft[0] = 0 # remove DC for FFT
        sp_data = np.abs(y_fft)[0:self.CHUNK//2] * 2/self.CHUNK
        self.set_plotdata(name='spectrum', data_x=self.f, data_y=sp_data)
        self.waveform.setYRange(0, 1.1*max(wf_data), padding=0)
        self.spectrum.setYRange(0, 1.1*max(sp_data), padding=0)

    def animation(self):
        timer = QtCore.QTimer()
        timer.timeout.connect(self.update)
        timer.start(20)
        self.start()

    def sample(self, size):
        # serial write section
        device.write('s\r\n'.encode())
        device.flush()
        device.readline()
        time.sleep(0.1)

        # # serial read section
        while True:
            try:
                serial_buf = device.readline().decode('utf-8').strip().split(',')
                if serial_buf == ['']:
                    break
                data, self.RATE = [3.3*float(i)/(2**16) for i in serial_buf[:-1]], int(size/(float(serial_buf[-1])))
                return data
            except Exception as e:
                print(e)
        return self.CHUNK*[0]

    def try_float(self, s):
        try:
            return float(s)
        except:
            return 0

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("You must enter in a COM or /dev/ port")
        sys.exit()
    global port, baud, device
    port = sys.argv[1]
    print(port)
    baud = 115200
    device = serial.Serial(
                port = port,
                baudrate = baud,
                timeout = 0.1
                )
    try:
        serial_app = SerialStream()
        serial_app.animation()
    except Exception as e:
        print(e)
        serial_app.stop()
        device.close()
    serial_app.stop()
    device.close()

