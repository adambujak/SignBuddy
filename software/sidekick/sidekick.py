import sys
import os
import time
import signal
import serial
import serial.tools.list_ports
import zlib
from threading import Thread, Lock, Event
from enum import Enum

sidekick = None

SYNC_BYTE = 0x16
FRAME_HEADER_SIZE = 6 # SYNC + LENGTH + 4 CRC

class DecoderParseState(Enum):
    HEADER = 0
    BODY = 1

# SignBuddy Stream Decoder
class SBSDecoder():
    def __init__(self, callback):
        self.encoded = bytearray()
        self.state = DecoderParseState.HEADER
        self.currentFrameSize = 0
        self.callback = callback
        self.decodedCRC = 0

    def _decode(self):
        while True:
            if self.state == DecoderParseState.HEADER:
                if len(self.encoded) < FRAME_HEADER_SIZE:
                    return

                if self.encoded[0] != SYNC_BYTE:
                    self.encoded = self.encoded[1:]
                    continue
                print('sync found')

                self.currentFrameSize = self.encoded[1]
                self.state = DecoderParseState.BODY

                self.decodedCRC = 0
                self.decodedCRC |= self.encoded[2] << 24
                self.decodedCRC |= self.encoded[3] << 16
                self.decodedCRC |= self.encoded[4] << 8
                self.decodedCRC |= self.encoded[5] << 0

                # pop out entire header
                self.encoded = self.encoded[FRAME_HEADER_SIZE:]

            if self.state == DecoderParseState.BODY:
                if len(self.encoded) < self.currentFrameSize:
                    return

                frameData = self.encoded[0:self.currentFrameSize]
                calculatedCRC = int(zlib.crc32(frameData))

                print('decodedCRC: ', hex(self.decodedCRC))
                print('calculatedCRC: ', hex(calculatedCRC))
                if self.decodedCRC != calculatedCRC:
                    self._soft_reset()
                    continue

                # shift out data
                self.encoded = self.encoded[self.currentFrameSize:]
                # reset for next packet
                self._soft_reset()

                #pass to decoder
                self.callback(frameData)

    def _soft_reset(self):
        self.state = DecoderParseState.HEADER
        self.currentFrameSize = 0
        self.decodedCRC = 0

    def decode(self, encoded):
        self.encoded = self.encoded + encoded
        self._decode()

    def reset(self):
        self.encoded = bytearray()
        self.state = DecoderParseState.HEADER
        self.currentFrameSize = 0
        self.decodedCRC = 0


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

    def read_raw_char(self):
        self.serialMutex.acquire()
        readval = self.serialInstance.read(1)
        self.serialMutex.release()
        return readval

    def read_callback(self):
        # User should overwrite this in their own class
        pass

class Sidekick(SerialDevice):
    def read_callback(self, byteData):
        self.decoder.decode(byteData)

    def write_packet(self, packet):
        time.sleep(0.001)

    def decoder_init(self):
        self.decoder = SBSDecoder(self.decoder_callback)

    def decoder_callback(self):
        print('frame complete')

def quit():
    global sidekick
    sidekick.stop_read()
    del sidekick
    exit(0)

def main():
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
        sidekick.decoder_init()

main()
