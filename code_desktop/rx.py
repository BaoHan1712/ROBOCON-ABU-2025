import serial
from cover.utils import *
import time

ser = serial.Serial('COM4', baudrate=115200)

current_value = 1  # bắt đầu từ 1

try:
    while True:
        if ser.in_waiting > 0:
            data = ser.read(1)   # Đọc 1 byte
            value = int.from_bytes(data, byteorder='big')
            print(f"Received value from UART6: {value}")
            
            if value == 1:
                if current_value <= 200:
                    offset = current_value   # offset tăng dần từ 1 -> 200
                    distance = 200  # bạn có thể chỉnh distance cố định, hoặc tăng theo offset
                    create_stm32_message_1(offset, distance, ser)
                    print(f"Sent back: offset={offset}, distance={distance}")
                    current_value += 1
                else:
                    print("Finished sending up to 200.")

except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()
