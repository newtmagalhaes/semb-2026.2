import struct
import time
from enum import Enum
from logging import Logger
from typing import Literal

import serial


class SerialStatus(Enum):
    # unespected
    UNKNOWN_ERROR = 0x91

    # Send
    SEND_OK = 0x80
    SEND_FAIL_INIT = 0x81
    SEND_FAIL_DATA = 0x82
    SEND_FAIL_END = 0x83

    # Receive
    RECEIVE_OK = 0x70
    RECEIVE_FAIL_INIT = 0x71
    RECEIVE_FAIL_DATA = 0x72
    RECEIVE_FAIL_END = 0x73

CMD_SORT = 0x01
CMD_MSG = 0x02
type SerialCommand = Literal[0x01, 0x02]


class ClientUART:
    def __init__(self, port: str, logger: Logger | None = None, baudrate=38400, timeout=2):
        self.serial = serial.Serial(port, baudrate, timeout=timeout)
        self.logger = logger
        # time.sleep(2) # Wait for STM32 reset if necessary

    def log(self, message):
        if self.logger:
            self.logger.warning(message)

    def format_packet_log(self, direction, cmd: SerialCommand, size, payload, checksum):
        return (f"{direction}\n"
                f"  CMD:     0x{cmd:02X}\n"
                f"  SIZE:    {size}\n"
                f"  CSUM:    0x{checksum:02X}\n"
                f"  PAYLOAD: {payload.hex().upper()}\n")

    def _calculate_checksum(self, cmd: SerialCommand, size, payload):
        cs = cmd ^ size
        for b in payload:
            cs ^= b
        return cs

    def send_packet(self, cmd: SerialCommand, payload=b""):
        size = len(payload)
        cs = self._calculate_checksum(cmd, size, payload)
        packet = struct.pack("BB", cmd, size) + payload + struct.pack("B", cs)
        self.log(self.format_packet_log("TX ->", cmd, size, payload, cs))
        self.serial.write(packet)

    def receive_packet(self):
        header = self.serial.read(2)
        if not header or len(header) < 2:
            self.log("RX <- Timeout or incomplete header\n")
            return None, None, None
        
        cmd, size = struct.unpack("BB", header)
        payload = self.serial.read(size)
        if len(payload) < size:
            self.log("RX <- Timeout or incomplete payload\n")
            return None, None, None
            
        received_cs = self.serial.read(1)
        if not received_cs:
            self.log("RX <- Timeout on checksum\n")
            return None, None, None
            
        calculated_cs = self._calculate_checksum(cmd, size, payload)
        self.log(self.format_packet_log("RX <-", cmd, size, payload, received_cs[0]))

        if calculated_cs != received_cs[0]:
            self.log(f"Checksum Error: Calc {calculated_cs} != Recv {received_cs[0]}\n")
            return None, None, None

        return cmd, size, payload
