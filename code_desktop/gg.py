from cover.utils import *
import time
import serial

ser = serial.Serial('COM8', 115200)
while True:
    create_stm32_message_1(1, 100, ser)
    time.sleep(1)

