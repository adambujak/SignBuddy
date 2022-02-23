import sys
import os
import time
import signal
import serial
import serial.tools.list_ports
from threading import Thread, Lock, Event

sidekick = None

class SerialDevice:
    def __init__(self, serialPort, baudRate=115200, readSleepTimeMs=1):
        self.serialMutex = Lock()
        self.readSleepTime = readSleepTimeMs / 1000;
        self.serialInstance = serial.Serial(serialPort, baudRate, timeout=0.5)
        self.stopReadEvent = Event()
        self.stopReadEvent.clear()
        self.readThreadInstance = Thread(target=self.read_thread, args=[])
        self.readThreadInstance.start()
        self.bufferFlushCount = 96

    def __del__(self):
        try:
            self.readThreadInstance.join()
        except:
            pass
        try:
            self.stop_read()
        except:
            pass

    def stop_read(self):
        self.stopReadEvent.set()

    def read_stopped(self):
        return self.stopReadEvent.is_set()

    def read_thread(self):
        while 1:
            if self.read_stopped():
                break
            data_buffer = bytes()
            while 1:
                readData = self.read_raw_char()
                if readData == bytes(): #empty bytes
                    # only send if not empty
                    if data_buffer != bytes():
                        self.read_callback(data_buffer)
                    break
                data_buffer += readData
                if len(data_buffer) > self.bufferFlushCount:
                    self.read_callback(data_buffer[0:self.bufferFlushCount])
                    data_buffer = data_buffer[self.bufferFlushCount:]
            time.sleep(self.readSleepTime)

    def write_bytes(self, byteData):
        self.serialMutex.acquire()
        maxsize = 31
        while len(byteData) > maxsize:
            self.serialInstance.write(byteData[0:maxsize])
            byteData = byteData[maxsize:]
            time.sleep(0.001)

        self.serialInstance.write(byteData)
        self.serialMutex.release()

    def read_callback(self):
        # User should overwrite this in their own class
        pass

class Sidekick(SerialDevice):
        def read_callback(self, byteData):
        self.decoder.decode(byteData)

    def write_packet(self, packet):
        frame = self.encoder.pack_packets_into_frame([packet])
        self.write_bytes(frame)
        time.sleep(0.001)

def quit():
    global sidekick
    sidekick.stop_read()
    del sidekick
    exit(0)

def main(update_att_values, update_alt_values, update_yaw_values):
    global sidekick

    connected = False
    ports = serial.tools.list_ports.comports()

    #for i in range(len(ports)):
    #    if ports[i].product == "STM32 STLink":
    #        sidekick = Sidekick(ports[i].device, 115200, 1)
    #        connected = True
    #        print("Connected to Albus")
    #        break

    if connected == False:
        # Delete bluetooth port
        for i in range(len(ports)):
            if "Bluetooth" in ports[i].name:
                ports.pop(i)
                break

        if len(ports) == 1:
            selectedPort = 0
            print("Using port: {}".format(ports[0].name))

        else:
            for i in range(len(ports)):
                print("{}: {}".format(i+1, ports[i].name))

            print("Which serial port do you want to use?")
            selectedPort = int(input()) - 1

        serialPort = ports[selectedPort].device

        sidekick = Sidekick(serialPort, 115200, 1)
