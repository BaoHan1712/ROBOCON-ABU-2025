import serial
import time

# Hàm tạo và gửi gói tin theo định dạng: [0x02][offset][distance_H][distance_L]
def send_message(offset, distance, ser):
    start_byte = 0x02
    distance_bytes = distance.to_bytes(2, 'big')  # 2 byte, big-endian
    packet = bytes([start_byte, offset]) + distance_bytes
    ser.write(packet)

# Mở cổng serial
ser = serial.Serial('COM4', 115200)

offset = 1
while offset <= 200:
    distance = offset * 100  # ví dụ tạo giá trị distance theo offset
    send_message(offset, distance, ser)
    print(f"Đã gửi: offset = {offset}, distance = {distance}")
    offset += 1
    time.sleep(0.5)  # 100ms
