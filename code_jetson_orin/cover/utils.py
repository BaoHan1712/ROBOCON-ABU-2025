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
        # Map giá trị âm sang 1
        mapped_value =max(0, min(99, int(offset) + 100))
        cv2.putText(frame, f'lech trai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    elif offset >= number:
        # Map giá trị dương sang 101-254 
        mapped_value = max(101, min(200, int(offset) + 100))
        
        cv2.putText(frame, f'lech phai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    else:
        mapped_value = 100
        cv2.putText(frame, f'chuan', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
    return mapped_value

def create_stm32_message_1(offset, distance, position, ser):
    """
    Tạo gói tin với cấu trúc:
    - Start Byte: 0x02
    - Data1: Offset (1 byte)
    - Data2: Distance (2 bytes) 
    - Data3: Position (2 bytes) - Có dấu
    - Checksum: Tổng modulo 256
    - End Byte: 0x03
    """
    # Kiểm tra và gán giá trị mặc định nếu tham số là None
    offset = 100 if offset is None else max(1, min(200, int(offset)))
    distance = 55555 if distance is None else max(2, min(65510, int(distance)))
    if position is None:
        position = 555
    
    # Chuyển position thành số 16-bit có dấu
    position_bytes = position.to_bytes(2, byteorder='big', signed=True)
    
    header = 0x02
    end_byte = 0x03
    
    # Tính checksum với position_bytes
    checksum = (header + offset + (distance >> 8) + (distance & 0xFF) + 
               position_bytes[0] + position_bytes[1]) % 256
    
    try:
        packet = (struct.pack(">B", header) + 
                 struct.pack(">B", offset) + 
                 struct.pack(">H", distance) + 
                 position_bytes +  # Sử dụng bytes có dấu
                 struct.pack(">B", checksum) + 
                 struct.pack(">B", end_byte))
        # ser.write(packet)
        print(f"Offset: {offset}, Distance: {distance}, Position: {position}")
    except struct.error as e:
        print(f"Error creating STM32 message: {e}")
        print(f"Offset: {offset}, Distance: {distance}, Position: {position}")

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

def calculate_position(frame, cx):
    """
        position: Giá trị từ -1350 đến 450, trong đó:
        - Từ trái đến tâm: -450 đến 450 
        - Từ tâm đến phải: -1350 đến -450
    """
    frame_width = frame.shape[1]
    frame_center = frame_width // 2
    cv2.line(frame, (frame_center, 0), (frame_center, frame.shape[0]), (0, 255, 0), 1)
    offset = cx - frame_center

    if cx < frame_center:  # Bên trái tâm (-450 đến 450)
        position = np.interp(cx, [0, frame_center], [-450, 450])
    else:  # Bên phải tâm (-1350 đến -450) 
        position = np.interp(cx, [frame_center, frame_width], [-1350, -450])
    
    # Hiển thị giá trị vị trí
    cv2.putText(frame, f'Pos: {int(position)}', (cx, 100), 
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    
    return int(position)

# def process_detections(detections, tracker, frame):
#     """Xử lý detections, kiểm tra màu đỏ và trả về thông tin đối tượng được phát hiện"""
#     basket_detected = False
#     backboard_detected = False
#     basket_info = None
#     backboard_info = None
#     conf = None

#     if detections.shape[0] > 0:
#         basket_detections = detections[detections[:, 5] == 1]  
#         backboard_detections = detections[detections[:, 5] == 0] 

#         # Xử lý basket nếu có
#         if basket_detections.shape[0] > 0:
#             x1, y1, x2, y2, _, _ = map(int, basket_detections[0])
#             bbox = frame[y1:y2, x1:x2]
#             hsv_bbox = cv2.cvtColor(bbox, cv2.COLOR_BGR2HSV)
#             red_mask_result = red_mask(hsv_bbox)
            
#             if np.any(red_mask_result):  # Kiểm tra nếu có màu đỏ trong bbox
#                 track_result = tracker.update(basket_detections[:1].astype(np.float32))
#                 if track_result.shape[0] > 0:
#                     basket_detected = True
#                     basket_info = track_result[0]
#                     conf = int(basket_detections[0][4])

#         # Xử lý backboard nếu không có basket
#         elif backboard_detections.shape[0] > 0:
#             x1, y1, x2, y2, _, _ = map(int, backboard_detections[0])
#             bbox = frame[y1:y2, x1:x2]
#             hsv_bbox = cv2.cvtColor(bbox, cv2.COLOR_BGR2HSV)
#             red_mask_result = red_mask(hsv_bbox)
            
#             if np.any(red_mask_result):  # Kiểm tra nếu có màu đỏ trong bbox
#                 track_result = tracker.update(backboard_detections[:1].astype(np.float32))
#                 if track_result.shape[0] > 0:
#                     backboard_detected = True
#                     backboard_info = track_result[0]
#                     conf = int(backboard_detections[0][4])

#     return basket_detected, backboard_detected, basket_info, backboard_info, conf


## Nhan dien khong co dung mau

def process_detections(detections, tracker):
    """Xử lý detections và trả về thông tin đối tượng được phát hiện"""
    basket_detected = False
    backboard_detected = False
    basket_info = None
    backboard_info = None
    conf = None

    if detections.shape[0] > 0:
        basket_detections = detections[detections[:, 5] == 1]  
        backboard_detections = detections[detections[:, 5] == 0] 
        

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

def auto_drive(frame, offset_2, num_objects):
    """
    Điều khiển hướng di chuyển của robot
    Args:
        frame: Khung hình hiện tại
        offset_2: Độ lệch của vật thể (1-99: lệch trái, 100: chuẩn, 101-254: lệch phải)
        num_objects: Số lượng vật thể phát hiện được
    Returns:
        direction: 1-đi thẳng, 2-đi trái, 3-đi phải, 4-không có vật
    """
    if num_objects == 0:
        cv2.putText(frame, "Khong co vat", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
        return 4
        
    # Nếu vật ở giữa (độ lệch từ 98-102)
    if 98 <= offset_2 <= 102:
        cv2.putText(frame, "Di thang", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
        return 1
        
    # Nếu vật lệch trái (offset < 98)
    elif offset_2 < 98:
        cv2.putText(frame, "Di phai", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
        return 3
        
    # Nếu vật lệch phải (offset > 102) 
    else:
        cv2.putText(frame, "Di trai", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
        return 2



