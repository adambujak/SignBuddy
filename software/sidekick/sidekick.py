import sys
import os
import copy
import time
import signal
import serial
import serial.tools.list_ports
import libscrc
from threading import Thread, Lock, Event
from enum import Enum

import sign_buddy_pb2 as sbp

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

                self.currentFrameSize = self.encoded[1]
                self.state = DecoderParseState.BODY

                self.decodedCRC = 0
                self.decodedCRC |= self.encoded[2] << 0
                self.decodedCRC |= self.encoded[3] << 8
                self.decodedCRC |= self.encoded[4] << 16
                self.decodedCRC |= self.encoded[5] << 24

                # pop out entire header
                self.encoded = self.encoded[FRAME_HEADER_SIZE:]

            if self.state == DecoderParseState.BODY:
                if len(self.encoded) < self.currentFrameSize:
                    return

                frameData = self.encoded[0:self.currentFrameSize]

                if self.decodedCRC != 0xA5A5A5A5: #hard coded value for now
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
    def open_output_file(self, letter):
        self.outputFile = open("output/{}_{}".format(letter, time.time()), "wb")
        self.outputEnabled = True

    def save_output_file(self):
        self.outputFile.close()

    def initialize_sampler(self):
        self.sampling = False
        self.current_letter = None
        self.outputEnabled = False

    def read_callback(self, byteData):
        if self.outputEnabled:
            self.outputFile.write(byteData)
        self.decoder.decode(byteData)

    def write_packet(self, packet):
        time.sleep(0.001)

    def decoder_init(self):
        self.decoder = SBSDecoder(self.decoder_callback)

    def decoder_callback(self, frameData):
        message = sbp.SBPMessage()

        try:
            message.ParseFromString(frameData)
        except:
            print("ERROR PARSING MESSAGE\r\n");

        if self.sampling:
            if message.id == 1:
                print(self.current_letter)

        print(message)

    def start_static_sample(self):
        cmd = bytearray(b'\x01')
        self.write_bytes(cmd)

    def start_dynamic_sample(self):
        cmd = bytearray(b'\x02')
        self.write_bytes(cmd)

    def sample(self, letter):
        self.current_letter = letter
        self.sampling = True
        self.open_output_file(letter)
        self.start_dynamic_sample()
        time.sleep(5)
        self.save_output_file()


def train():
    global sidekick
    while (1):
        print("input letter: ")
        letter = input()
        if (ord(letter) < ord('a') or ord(letter) > ord('z')):
            print('invalid letter')
            continue
        sidekick.sample(letter)

def quit():
    global sidekick
    sidekick.stop_read()
    del sidekick
    exit(0)

def main():
    global sidekick

    connected = False
    ports = serial.tools.list_ports.comports()

    if connected == False:

        for i in range(len(ports)):
            print("{}: {}".format(i+1, ports[i].name))

        print("Which serial port do you want to use?")
        selectedPort = int(input()) - 1

        serialPort = ports[selectedPort].device

        sidekick = Sidekick(serialPort, 115200, 1)
        sidekick.initialize_sampler()
        sidekick.decoder_init()
        train()


main()
