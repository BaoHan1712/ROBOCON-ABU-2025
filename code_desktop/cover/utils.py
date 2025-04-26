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
    number =1
    offset = offset_backboard(frame, cx)
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
    # Giới hạn giá trị cho offset và distance
    offset = max(1, min(200, int(offset)))
    distance = max(2, min(65510, int(distance)))

    # Các byte cấu trúc gói tin
    header = 0x02
    # end_byte = 0x03

    # # Tính checksum
    # checksum = (header + offset + (distance >> 8) + (distance & 0xFF)) % 256

    try:
        packet = struct.pack(">B", header) + struct.pack(">B", offset) + struct.pack(">H", distance)
        ser.write(packet)  # Gửi gói tin qua UART
    except struct.error as e:
        print(f"Error creating STM32 message: {e}")


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
    """X lý detections và tr v thông tin đi tng đc phát hin"""
    basket_detected = False
    backboard_detected = False
    basket_info = None
    backboard_info = None
    conf = None

    if detections.shape[0] > 0:
        basket_detections = detections[detections[:, 5] == 1]  
        backboard_detections = detections[detections[:, 5] == 0] 
        

        # X lý basket nu có
        if basket_detections.shape[0] > 0:
            track_result = tracker.update(basket_detections[:1].astype(np.float32))
            if track_result.shape[0] > 0:
                basket_detected = True
                basket_info = track_result[0]
                conf = int(basket_detections[0][4])

        # X lý backboard nu không có basket
        elif backboard_detections.shape[0] > 0:
            track_result = tracker.update(backboard_detections[:1].astype(np.float32))
            if track_result.shape[0] > 0:
                backboard_detected = True
                backboard_info = track_result[0]
                conf = int(backboard_detections[0][4])

    return basket_detected, backboard_detected, basket_info, backboard_info, conf

