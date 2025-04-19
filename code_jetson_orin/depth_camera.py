import cv2
import pyrealsense2 as rs
import numpy as np
from ultralytics import YOLO
from cover.utils import *
import math
from cover.sort import Sort
# import serial
import time
# from collections import deque

# ser = serial.Serial('/dev/ttyUSB0', 115200)

model = YOLO("rim_robot.pt", task="detect")
tracker = Sort(max_age=30)

# Cấu hình Intel RealSense D435
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
profile = pipeline.start(config)

# config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
# depth_scale = profile.get_device().first_depth_sensor().get_depth_scale()

align_to = rs.stream.color
align = rs.align(align_to)

prev_time = time.time()

# depth_history = []


# # Cấu hình
# ALPHA = 0.1  # Hệ số lọc nhiễu (càng cao càng phản hồi nhanh)
# MAX_DEPTH = 6000  
# depth_history = deque(maxlen=20)  


# def get_distance(depth_frame, x, y):
#     """Lấy khoảng cách từ Depth Camera với bộ lọc mượt."""
#     depth = depth_frame.get_distance(x, y) * 1000  

#     if depth == 0 or depth > MAX_DEPTH:
#         return depth_history[-1] if depth_history else None

#     prev_depth = depth_history[-1] if depth_history else depth

#     # Bộ lọc trung bình động
#     smoothed_depth = ALPHA * depth + (1 - ALPHA) * prev_depth
#     depth_history.append(smoothed_depth)

#     return round(smoothed_depth)



def send_offset_stm(offset, ser):
    """Truyền dữ liệu khoảng cách và độ lệch xuống STM32"""
    if offset is not None:
        offset = int(offset) 
    else:
        offset = 10

    # if min_distance is None:
    #     min_distance = depth_history[-1] if depth_history else 11
    # distance = int(min_distance)
    create_stm32_message_1(offset, ser)


adjacent = 350  # cạnh kề
def calculate_angle(deviation_pixels):
   
    
    opposite = deviation_pixels  # cạnh đối

    # Tính cạnh huyền
    hypotenuse = math.sqrt(adjacent**2 + opposite**2)

    return hypotenuse

def visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf, distance):
    """Hiển thị kết quả phát hiện lên frame"""
    offset = None
    min_distance = None
    quydoi = None

    # Lấy kích thước frame
    frame_height, frame_width = frame.shape[:2]
    frame_center_x = frame_width // 2


    if basket_detected:
        x1, y1, x2, y2, id = map(int, basket_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
        
        # Lấy khoảng cách từ depth camera
        # min_distance = get_distance(distance, cx, cy)
        
        offset = calculator_offset_stm32(frame, cx, x1, y2)

        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
        cv2.putText(frame, f'basket {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    
    elif backboard_detected:
        x1, y1, x2, y2, id = map(int, backboard_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2

        # min_distance = get_distance(depth_frame, cx, cy)
        
        # offset = calculator_offset_stm32(frame, cx, x1, y2)


        # cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        # draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
        # cv2.putText(frame, f'backboard {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

        x1, y1, x2, y2, id = map(int, backboard_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
 
        # Tính offset 
        temp_offset = calculator_offset_stm32(frame, cx, x1, y2)
        canhdoi = temp_offset * 1.2
        
        # Điểm bắt đầu của đường thẳng đứng (cạnh đối)
        start_y = cy + 200
        
        # 1. Vẽ đường thẳng đứng đi qua tâm vật thể - cạnh đối (xanh dương)
        cv2.line(frame, (cx, cy), (cx, start_y), (255, 0, 0), 2)
        
        # 2. Vẽ đường ngang - cạnh kề (xanh lá)
        cv2.line(frame, (frame_center_x, cy), (cx, cy), (0, 255, 0), 2)
        
        # 3. Vẽ đường chéo - cạnh huyền (đỏ)
        cv2.line(frame, (cx, start_y), (frame_center_x, cy), (0, 0, 255), 2)
        
        # Vẽ góc vuông
        size = 10
        cv2.line(frame, (cx, cy), (cx, cy - size), (255, 255, 255), 1)
        cv2.line(frame, (cx, cy), (cx - size, cy), (255, 255, 255), 1)
        
        # Tính khoảng cách cho bảng
        if temp_offset is not None:

            hypotenuse = calculate_angle(canhdoi)
            quydoi = int(hypotenuse)-adjacent
            print(f"quydoi {quydoi}")
            cv2.putText(frame, f"Distance: {quydoi:.1f}cm", (cx + 10, cy + 20),
                       cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
                       
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
        cv2.putText(frame, f'backboard {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
        
    return quydoi

while True:


    frames = pipeline.wait_for_frames()
    aligned_frames = align.process(frames)
    # depth_frame = aligned_frames.get_depth_frame()
    color_frame = aligned_frames.get_color_frame()

    if not color_frame:
        continue

    frame = np.asanyarray(color_frame.get_data())
    detections = np.empty((0, 6))
    results = model.predict(source=frame, imgsz=640, conf=0.55, verbose=False, max_det=1)
    
    for info in results:
        boxes = info.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            conf = box.conf[0]
            classindex = box.cls[0]
            conf = math.ceil(conf * 100)
            classindex = int(classindex)
            
            if conf > 55:
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                new_detections = np.array([x1, y1, x2, y2, conf, classindex])
                detections = np.vstack((detections, new_detections))

    basket_detected, backboard_detected, basket_info, backboard_info, conf = process_detections(detections, tracker)

    offset_2 = visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf)
    # send_offset_stm(offset_2, min_distance,ser)


    cv2.imshow('Object Detection', frame)                        
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

pipeline.stop()
cv2.destroyAllWindows()
