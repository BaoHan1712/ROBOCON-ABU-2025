import cv2
# import pyrealsense2 as rs
import numpy as np
from ultralytics import YOLO
from cover.utils import *
import math
from cover.sort import Sort
# import serial
import time

# ser = serial.Serial('/dev/ttyUSB0', 115200)

model = YOLO(r"yolo11n.pt", task="detect")
tracker = Sort(max_age=30)
cap = cv2.VideoCapture(0)

prev_time = time.time()

depth_history = []
import numpy as np
from collections import deque

# Cấu hình
ALPHA = 0.1  # Hệ số lọc nhiễu (càng cao càng phản hồi nhanh)
MAX_DEPTH = 6000  
depth_history = deque(maxlen=20)  


def get_distance(depth_frame, x, y):
    """Lấy khoảng cách từ Depth Camera với bộ lọc mượt."""
    depth = depth_frame.get_distance(x, y) * 1000  

    if depth == 0 or depth > MAX_DEPTH:
        return depth_history[-1] if depth_history else None

    prev_depth = depth_history[-1] if depth_history else depth

    # Bộ lọc trung bình động
    smoothed_depth = ALPHA * depth + (1 - ALPHA) * prev_depth
    depth_history.append(smoothed_depth)

    return round(smoothed_depth)



def send_offset_stm(offset, min_distance,position, ser):
    """Truyền dữ liệu khoảng cách và độ lệch xuống STM32"""
    if offset is not None:
        offset = int(offset) 
    else:
        offset = 100

    if min_distance is None:
        min_distance = depth_history[-1] if depth_history else 11

    distance = int(min_distance)
    create_stm32_message_1(offset, distance,position, ser)

def visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf, distance):
    """Hiển thị kết quả phát hiện lên frame"""
    offset = None
    # position = None
    min_distance = None
    direction = None

    if basket_detected:
        x1, y1, x2, y2, id = map(int, basket_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
        
        # Lấy khoảng cách từ depth camera
        min_distance = get_distance(distance, cx, cy)
        
        # Lấy offset và position
        offset = calculator_offset_stm32(frame, cx, x1, y2)
        print(offset)
        # position = calculate_position(frame, cx)
        direction = auto_drive(frame, offset, 2)

        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
        cv2.putText(frame, f'basket {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    
    elif backboard_detected:
        x1, y1, x2, y2, id = map(int, backboard_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2

        
        offset = calculator_offset_stm32(frame, cx, x1, y2)
        # position = calculate_position(frame, cx)
        direction = auto_drive(frame, offset, 2)
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
        cv2.putText(frame, f'backboard {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
        
    return offset, direction, min_distance
    # return offset, position, min_distance

while True:
    ret, frame = cap.read()
    current_time = time.time()
    fps = 1 / (current_time - prev_time)
    prev_time = current_time

    detections = np.empty((0, 6))
    results = model.predict(source=frame, imgsz=640, conf=0.75, verbose=False, max_det=1)
    
    for info in results:
        boxes = info.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            conf = box.conf[0]
            classindex = box.cls[0]
            conf = math.ceil(conf * 100)
            classindex = int(classindex)
            
            if conf > 75:
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                new_detections = np.array([x1, y1, x2, y2, conf, classindex])
                detections = np.vstack((detections, new_detections))

    basket_detected, backboard_detected, basket_info, backboard_info, conf = process_detections(detections, tracker)

    # Bỏ tham số depth_frame vì không có camera độ sâu
    offset_2, direction, min_distance = visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf, None)
    cv2.putText(frame, f'offset: {offset_2}', (10, 150), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
    send_offset_stm(offset_2, min_distance, direction, 1)

    cv2.putText(frame, f'FPS: {fps:.2f}', (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv2.imshow('Object Detection', frame)                        

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()
