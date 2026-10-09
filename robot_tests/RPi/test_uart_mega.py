#!/usr/bin/env python3
"""
Tes UART Raspberry Pi <-> Mega (pasangan T08).
Persiapan: sudo raspi-config -> Interface -> Serial Port:
           login shell = No, serial hardware = Yes. Lalu reboot.
           pip install pyserial
Jalankan : python3 test_uart_mega.py
Jika Pi disambung ke Mega lewat USB, ganti PORT ke /dev/ttyACM0.
"""
import time
import serial

PORT = "/dev/serial0"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=0.1)
time.sleep(0.5)
print(f"Terbuka: {PORT} @ {BAUD}")

x, y, area = 320, 240, 1500
try:
    while True:
        pkt = f"OBJ,{x},{y},{area}\n"
        ser.write(pkt.encode())
        print("Kirim :", pkt.strip())
        t0 = time.time()
        while time.time() - t0 < 0.5:
            line = ser.readline().decode(errors="ignore").strip()
            if line:
                print("Terima:", line)
        x = (x + 10) % 640
        time.sleep(0.5)
except KeyboardInterrupt:
    ser.close()
