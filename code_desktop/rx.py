import serial
import struct
import time

ser = serial.Serial('COM4', baudrate=115200, timeout=1)

try:
    while True:
        if ser.in_waiting >= 7:
            data = ser.read(7)
            
            # Kiểm tra start byte
            if data[0] == 0x02:
                # Giải mã các byte còn lại: 3 số uint16_t big endian
                CL, CT, BC = struct.unpack('>HHH', data[1:])  # '>' là Big Endian, H = uint16_t
                print(f"Nhận được: CL = {CL}, CT = {CT}, BC = {BC}")
            else:
                print(f"Start byte không hợp lệ: {hex(data[0])}")
                print(f"Dữ liệu raw: {[hex(b) for b in data]}")

        time.sleep(0.05)  # giảm tải CPU

except KeyboardInterrupt:
    print("\nThoát chương trình.")
finally:
    ser.close()
