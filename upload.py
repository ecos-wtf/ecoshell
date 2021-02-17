#!/usr/bin/env python3
'''
Write the given file content into RAM by taking advantage of 'write_memory'
CLI function of Broadcom eCOS platform.

An integrity check is performed by reading content off the device's memory
by using 'read_memory' CLI function and comparing its content to the original
file.

Author: Quentin Kaiser <quentin@ecos.wtf>
'''
import serial
import re
import sys
import struct

def upload_and_run(filename, dest_addr=0x80810000):
    with serial.Serial() as ser:
        ser.baudrate = 115200
        ser.port = '/dev/ttyUSB0'
        ser.open()
        ser.write(b"\n")
        ser.readline()

        with open(filename, 'rb') as f:
            payload = f.read()

        print("[+] Writing payload to memory.")
        for off in range(0, len(payload), 4):
            addr = hex(dest_addr + off)
            val = payload[off:off+4].hex()
            cmd = bytes(f'write_memory -s 4 {addr} 0x{val}\r\n', 'utf-8')
            ser.write(cmd)
            ser.readline() # echo
            ser.readline() # newline
            ser.readline() # echo
            ser.readline() # newline
            ser.readline() # newline

        effective_payload = b""
        print("[+] Reading payload from memory.")
        for off in range(0, len(payload), 4):
            addr = hex(dest_addr + off)
            val = payload[off:off+4].hex()
            cmd = bytes(f'read_memory -n 4 {addr}\r\n', 'utf-8')
            ser.write(cmd)
            match = []
            while not match:
                output = ser.readline() # content
                match = re.findall(b"[0-9-a-f]{8}: ([0-9-a-f ]{11})",output)
            address = int(match[0].replace(b" ", b"").decode('utf-8'), 16)
            effective_payload += struct.pack(">I", address)

        if payload == effective_payload:
            print("[+] Integrity check passed.")
            print('call func -r -a 0x{:2x}'.format(dest_addr))

if __name__ == "__main__":

    if len(sys.argv) < 3:
        print(
            "Usage: {} shellcode address (e.g. 0x80810000)"\
            .format(sys.argv[0])
        )
        sys.exit(-1)
    upload_and_run(sys.argv[1], dest_addr=int(sys.argv[2], 16))
