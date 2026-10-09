# Kode Tes Komponen - Semi-Autonomous Differential Drive Robot

Upload satu per satu, tiap folder = 1 sketch (nama folder = nama .ino).
Board Mega: "Arduino Mega or Mega 2560". Serial Monitor: 115200 baud.

| Sketch | Target | Yang dites |
|---|---|---|
| T01_LCD_I2C_Scanner | Mega | I2C scan + LCD 20x4 |
| T02_Ultrasonik | Mega | 3x HC-SR04 |
| T03_Proximity_Buzzer | Mega | Proximity + buzzer |
| T04_Relay_Gripper | Mega | Relay aktif LOW |
| T05_Servo_Arm | Mega | Servo arm |
| T06_Motor_BTS7960 | Mega | Motor kiri/kanan (roda diangkat!) |
| ESP32_PS3_MPU6050 | ESP32 | PS3 + MPU6050 -> UART |
| T07_UART_ESP32_Mega | Mega | Terima paket dari ESP32 |
| T08_UART_RPi_Mega | Mega | Terima/balas paket dari Pi |
| RPi/test_uart_mega.py | Pi | Pasangan T08 |

Urutan saran: T01 -> T02 -> T03 -> T04 -> T05 -> T06 -> ESP32 + T07 -> T08.
