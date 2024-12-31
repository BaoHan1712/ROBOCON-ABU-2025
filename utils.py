import cv2 
import struct


def offset_backboard(frame_2,cx):
    """Tính toán trung tâm khung hình"""
    frame_center_x = frame_2.shape[1] // 2
    cv2.line(frame_2, (frame_center_x, 0), (frame_center_x, frame_2.shape[0]), (0, 255, 0), 1)
    offset = cx - frame_center_x
    return offset

def calculator_offset_stm32(frame, cx, x1, y2):
    number = 7
    offset = offset_backboard(frame, cx)
    """  Tình độ lệch của rổ
        offset < 0 -> map sang 1-99
        offset = 0 -> map thành 100  
        offset > 0 -> map sang 101-254"""
    
    if offset < -number:
        # Map giá trị âm sang 1
        mapped_value =max(1, min(99, int(offset) + 100))
        cv2.putText(frame, f'lech trai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    elif offset > number:
        # Map giá trị dương sang 101-254 
        mapped_value = max(101, min(254, int(offset) + 100))
        
        cv2.putText(frame, f'lech phai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    else:
        mapped_value = 100
        cv2.putText(frame, f'chuan', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
    return mapped_value

# Từ 94-106 là chuẩn, đèn sẽ sáng khi từ 60-90 or từ 110 đến 140

def create_stm32_message_1(offset, distance, ser):
    """
    Tạo gói tin với cấu trúc:
    - Start Byte: 0x02
    - Data1: Offset (1 byte)
    - Data2: Distance (2 bytes)
    - Checksum: Tổng modulo 256
    - End Byte: 0x03
    """
    offset = max(1, min(200, int(offset)))
    distance = max(1, min(10000, int(distance)))

    header = 0x02
    end_byte = 0x03
    checksum = (header + offset + (distance >> 8) + (distance & 0xFF)) % 256

    packet = struct.pack(">B", header) + struct.pack(">B", offset) + struct.pack(">H", distance) + struct.pack(">B", checksum) + struct.pack(">B", end_byte)
    ser.write(packet)


