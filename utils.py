import cv2 
import struct
import numpy as np


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

from collections import deque

distance_buffer = deque(maxlen=5)  

def calculator_distance(frame, x1, y2, depth_frame, depth_x, depth_y):
    """
    Hàm tính khoảng cách và hiển thị trên khung hình.
    """
    distance_cm = get_average_distance(depth_frame, depth_x, depth_y, kernel_size=30)
    
    # Lọc giá trị qua buffer để làm mượt
    if distance_cm > 0:
        distance_buffer.append(distance_cm)
    elif distance_buffer:  
        distance_cm = np.mean(distance_buffer)
    
    if distance_cm > 0:
        cv2.putText(frame, f'Kc: {distance_cm:.1f} cm', (x1, y2 + 20),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
    

def get_average_distance(depth_frame, cx, cy, kernel_size=30):
    """
    Lấy khoảng cách trung bình trong vùng lân cận, có lọc nhiễu.
    """
    width = depth_frame.get_width()
    height = depth_frame.get_height()
    
    if cx < 0 or cx >= width or cy < 0 or cy >= height:
        return 0.0

    # Tăng kích thước kernel cho khoảng cách xa
    if kernel_size < 50:
        kernel_size = 50  # Tăng kích thước vùng lấy mẫu

    # Lấy vùng lân cận xung quanh điểm (cx, cy)
    x_start = max(0, cx - kernel_size // 2)
    x_end = min(width, cx + kernel_size // 2 + 1)
    y_start = max(0, cy - kernel_size // 2)
    y_end = min(height, cy + kernel_size // 2 + 1)

    # Thu thập các giá trị độ sâu hợp lệ trong vùng từ 10m trở xuống
    depth_values = []
    for py in range(y_start, y_end):
        for px in range(x_start, x_end):
            depth = depth_frame.get_distance(px, py)
            if 0 < depth < 10: 
                depth_values.append(depth)

    if not depth_values:
        return 0.0

    # Lọc nhiễu bằng phương pháp trung vị
    depth_values = np.array(depth_values)
    median_depth = np.median(depth_values)
    
    # Chỉ lấy các giá trị trong khoảng ±15% của trung vị
    valid_mask = np.abs(depth_values - median_depth) <= 0.15 * median_depth
    filtered_values = depth_values[valid_mask]

    if len(filtered_values) == 0:
        return 0.0

    # Tính trung bình sau khi đã lọc
    filtered_depth = np.mean(filtered_values)
    
    # Làm tròn đến 1 chữ số thập phân
    return int(filtered_depth*100)  # Chuyển đổi sang cm

import struct

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


