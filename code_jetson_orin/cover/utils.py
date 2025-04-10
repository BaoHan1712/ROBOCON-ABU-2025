import cv2 
import struct
import numpy as np
from cover.color_utils import *

def offset_backboard(frame_2,cx):
    """Tính toán trung tâm khung hình"""
    frame_center_x = frame_2.shape[1] // 2
    cv2.line(frame_2, (frame_center_x, 0), (frame_center_x, frame_2.shape[0]), (0, 255, 0), 1)
    offset = cx - frame_center_x
    return offset

def calculator_offset_stm32(frame, cx, x1, y2):
    number =1
    offset = offset_backboard(frame, cx)
    """  Tình độ lệch của rổ
        offset < 0 -> map sang 1-99
        offset = 0 -> map thành 100  -
        offset > 0 -> map sang 101-254"""
    
    if offset <= -number:
        mapped_value = int(abs(offset))

        cv2.putText(frame, f'lech trai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    elif offset >= number:  
        mapped_value = int(offset)
        cv2.putText(frame, f'lech phai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    else:
        mapped_value = 100
        cv2.putText(frame, f'chuan', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
    return mapped_value



def create_stm32_message_1(offset, distance, ser):
    """
    Tạo gói tin với cấu trúc:
    - Start Byte: 0x02
    - Data1: Offset (1 byte)
    - Data2: Distance (2 bytes)
    - Data3: Position (1 byte)
    - Checksum: Tổng modulo 256
    - End Byte: 0x03
    """
    # Kiểm tra và gán giá trị mặc định nếu tham số là None
    offset = 100 if offset is None else max(1, min(200, int(offset)))
    distance = 55555 if distance is None else max(2, min(65510, int(distance)))

    
    header = 0x02
    end_byte = 0x03

    # Tính checksum trên các thành phần
    checksum = (header + offset + (distance >> 8) + (distance & 0xFF) ) % 256

    try:
        packet = (
            struct.pack(">B", header) +
            struct.pack(">B", offset) +
            struct.pack(">H", distance) +
            struct.pack(">B", checksum) +
            struct.pack(">B", end_byte)
        )
        
        # ser.write(packet)
        
        print(f"Đã gửi {offset}, Distance: {distance}")

    except struct.error as e:
        print(f"Lỗi tạo gói tin STM32: {e}")
        print(f"Offset: {offset}, Distance: {distance}")


def draw_plus_sign(image, center, size=10, color=(0, 0, 255), thickness=2):
    """
    Vẽ dấu cộng (+) trên hình ảnh.

    Parameters:
        image (numpy.ndarray): Ảnh cần vẽ dấu cộng.
        center (tuple): Tọa độ trung tâm của dấu cộng (cx, cy).
        size (int): Kích thước của dấu cộng (độ dài mỗi nhánh).
        color (tuple): Màu sắc của dấu cộng (BGR).
        thickness (int): Độ dày của đường vẽ.
    """
    cx, cy = center
    # Vẽ đường ngang
    cv2.line(image, (cx - size, cy), (cx + size, cy), color, thickness)
    # Vẽ đường dọc
    cv2.line(image, (cx, cy - size), (cx, cy + size), color, thickness)

def process_detections(detections, tracker):
    """Xử lý detections và trả về thông tin đối tượng được phát hiện"""
    basket_detected = False
    backboard_detected = False
    basket_info = None
    backboard_info = None
    conf = None

    if detections.shape[0] > 0:
        basket_detections = detections[detections[:, 5] == 0]  
        backboard_detections = detections[detections[:, 5] == 1] 
        

        # Xử lý basket nếu có
        if basket_detections.shape[0] > 0:
            track_result = tracker.update(basket_detections[:1].astype(np.float32))
            if track_result.shape[0] > 0:
                basket_detected = True
                basket_info = track_result[0]
                conf = int(basket_detections[0][4])

        # Xử lý backboard nếu không có basket
        elif backboard_detections.shape[0] > 0:
            track_result = tracker.update(backboard_detections[:1].astype(np.float32))
            if track_result.shape[0] > 0:
                backboard_detected = True
                backboard_info = track_result[0]
                conf = int(backboard_detections[0][4])

    return basket_detected, backboard_detected, basket_info, backboard_info, conf



