import time
import serial
import serial.tools.list_ports
from threading import Thread, Lock, Event
from enum import Enum

import sign_buddy_pb2 as sbp

sidekick = None

SYNC_BYTE = 0x16
FRAME_HEADER_SIZE = 6  # SYNC + LENGTH + 4 CRC


class DecoderParseState(Enum):
    HEADER = 0
    BODY = 1


# SignBuddy Stream Decoder
class SBSDecoder:
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

                frame_data = self.encoded[0:self.currentFrameSize]

                if self.decodedCRC != 0xA5A5A5A5:  # hard coded value for now
                    self._soft_reset()
                    continue

                # shift out data
                self.encoded = self.encoded[self.currentFrameSize:]
                # reset for next packet
                self._soft_reset()

                # pass to decoder
                self.callback(frame_data)

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
    def __init__(self, serial_port, baud_rate=115200, read_sleep_time_ms=1):
        self.serialMutex = Lock()
        self.readSleepTime = read_sleep_time_ms / 1000
        self.serialInstance = serial.Serial(serial_port, baud_rate, timeout=0.5)
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
                read_data = self.read_raw_char()
                if read_data == bytes():  # empty bytes
                    # only send if not empty
                    if data_buffer != bytes():
                        self.read_callback(data_buffer)
                    break
                data_buffer += read_data
                if len(data_buffer) > self.bufferFlushCount:
                    self.read_callback(data_buffer[0:self.bufferFlushCount])
                    data_buffer = data_buffer[self.bufferFlushCount:]
            time.sleep(self.readSleepTime)

    def write_bytes(self, byte_data):
        self.serialMutex.acquire()
        maxsize = 31
        while len(byte_data) > maxsize:
            self.serialInstance.write(byte_data[0:maxsize])
            byte_data = byte_data[maxsize:]
            time.sleep(0.001)

        self.serialInstance.write(byte_data)
        self.serialMutex.release()

    def read_raw_char(self):
        self.serialMutex.acquire()
        read_val = self.serialInstance.read(1)
        self.serialMutex.release()
        return read_val

    def read_callback(self, byte_data):
        # User should overwrite this in their own class
        pass


class Sidekick(SerialDevice):
    def __init__(self, serial_port, baud_rate=115200, read_sleep_time_ms=1):
        super().__init__(serial_port, baud_rate, read_sleep_time_ms)
        self.decoder = None
        self.message = sbp.SBPMessage()

    def read_callback(self, byte_data):
        self.decoder.decode(byte_data)

    def decoder_init(self):
        self.decoder = SBSDecoder(self.decoder_callback)

    def decoder_callback(self, frame_data):
        try:
            self.message.ParseFromString(frame_data)
        except:
            print("ERROR PARSING MESSAGE\r\n")

        print(self.message)


def quit():
    global sidekick
    sidekick.stop_read()
    del sidekick
    exit(0)


def main():
    global sidekick

    connected = False
    ports = serial.tools.list_ports.comports()

    if not connected:
        # Delete bluetooth port
        for i in range(len(ports)):
            if "Bluetooth" in ports[i].name:
                ports.pop(i)
                break

        if len(ports) == 1:
            selected_port = 0
            print("Using port: {}".format(ports[0].name))

        else:
            for i in range(len(ports)):
                print("{}: {}".format(i + 1, ports[i].name))

            print("Which serial port do you want to use?")
            selected_port = int(input()) - 1

        serial_port = ports[selected_port].device

        sidekick = Sidekick(serial_port, 115200, 1)
        sidekick.decoder_init()


main()
