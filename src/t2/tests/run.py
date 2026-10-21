from core.uart_client import *
from logging import getLogger

logger = getLogger(__name__)

if __name__ == '__main__':
    client = ClientUART('/dev/ttyACM0', logger=logger)
    payload = struct.pack(f'<3h', *[5,4,3])
    print(f'payload: {payload}')
    client.send_packet(CMD_SORT, payload)
    cmd, size, buff = client.receive_packet()
    rec = struct.unpack(f'<{len(buff)//2}h', buff)
    print(f'recebido: {rec}')
