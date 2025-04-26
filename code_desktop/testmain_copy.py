import cv2
import pyrealsense2 as rs
import numpy as np
from ultralytics import YOLO
from cover.utils import *
import math
from cover.sort import Sort
import serial
from collections import deque
import time
import threading

# Khởi tạo hàng đợi để lưu dữ liệu khoảng cách
distance_queue = deque(maxlen=10)  

# Khởi tạo Serial cho ESP32
esp32_serial = serial.Serial('/dev/ttyUSB1', 115200, timeout=1)  

def read_esp32_data():
    """Đọc dữ liệu từ ESP32 và lưu vào hàng đợi"""
    while True:
        try:
            if esp32_serial.in_waiting:
                data = esp32_serial.readline().decode('utf-8').strip()
                try:
                    distance = int(data)
                    if 0 <= distance <= 5000: 
                        distance_queue.append(distance)
                        # print(f"Received distance: {distance}mm")
                except ValueError:
                    pass  
        except Exception as e:
            print(f"Error reading ESP32 data: {e}")
            time.sleep(1)

# Khởi động thread đọc dữ liệu ESP32
esp32_thread = threading.Thread(target=read_esp32_data, daemon=True)
esp32_thread.start()

ser = serial.Serial('/dev/ttyUSB0', 115200)

model = YOLO(r"/home/phongvu/Downloads/robocon/model/rim_robot.engine", task="detect")
tracker = Sort(max_age=50)

# Cấu hình Intel RealSense D435
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.color, 848, 480, rs.format.bgr8, 60)
profile = pipeline.start(config)

align_to = rs.stream.color
align = rs.align(align_to)

prev_time = time.time()
last_send_time = time.time()  # Track the last send time
send_interval = 0.05  # 50 ms


def send_offset_stm(offset, distance, ser):
    """Truyền dữ liệu khoảng cách và độ lệch xuống STM32"""
    if offset is not None:
        offset = int(offset) 

    create_stm32_message_1(offset,distance,ser)



adjacent = 350  # cạnh kề
def calculate_angle(deviation_pixels):
   
    
    opposite = deviation_pixels  # cạnh đối

    # Tính cạnh huyền
    hypotenuse = math.sqrt(adjacent**2 + opposite**2)

    return hypotenuse

def visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf):
    """Hiển thị kết quả phát hiện lên frame"""
    quydoi = 0

    # Lấy kích thước frame
    frame_height, frame_width = frame.shape[:2]
    frame_center_x = frame_width // 2

    if basket_detected:
        x1, y1, x2, y2, id = map(int, basket_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
        
        # Lấy offset và position
        offset = calculator_offset_stm32(frame, cx, x1, y2)

        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
        cv2.putText(frame, f'basket {conf} ', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    
    elif backboard_detected:
        x1, y1, x2, y2, id = map(int, backboard_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2

        x1, y1, x2, y2, id = map(int, backboard_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
 
        # Tính offset cho bảng (nhưng không trả về)
        temp_offset = calculator_offset_stm32(frame, cx, x1, y2)
        canhdoi = temp_offset * 2.8
        
        # Điểm bắt đầu của đường thẳng đứng (cạnh đối)
        start_y = cy + 200
        
        # 1. Vẽ đường thẳng đứng đi qua tâm vật thể - cạnh đối (xanh dương)
        cv2.line(frame, (cx, cy), (cx, start_y), (255, 0, 0), 1)
        
        # 2. Vẽ đường ngang - cạnh kề (xanh lá)
        cv2.line(frame, (frame_center_x, cy), (cx, cy), (0, 255, 0), 1)
        
        # 3. Vẽ đường chéo - cạnh huyền (đỏ)
        cv2.line(frame, (cx, start_y), (frame_center_x, cy), (0, 0, 255), 1)
        
        # Vẽ góc vuông
        size = 10
        cv2.line(frame, (cx, cy), (cx, cy - size), (255, 255, 255), 1)
        cv2.line(frame, (cx, cy), (cx - size, cy), (255, 255, 255), 1)
        
        # Tính khoảng cách cho bảng
        if temp_offset is not None:

            hypotenuse = calculate_angle(canhdoi)
            quydoi = int(hypotenuse)-adjacent
            print(f"quydoi {quydoi}")
            cv2.putText(frame, f"Distance: {quydoi:.1f}mm", (cx + 10, cy + 20),
                       cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
                       
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
       
        cv2.putText(frame, f'backboard {conf} ', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
        
    return quydoi

while True:
    current_time = time.time()
    fps = 1 / (current_time - prev_time)
    prev_time = current_time

    frames = pipeline.wait_for_frames()
    aligned_frames = align.process(frames)
    color_frame = aligned_frames.get_color_frame()

    if distance_queue:
        latest_distance = distance_queue[-1]

    if not color_frame:
        continue

    frame = np.asanyarray(color_frame.get_data())
    detections = np.empty((0, 6))
    results = model.predict(source=frame, imgsz=480, conf=0.50, verbose=False, max_det=1)
    
    for info in results:
        boxes = info.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            conf = box.conf[0]
            classindex = box.cls[0]
            conf = math.ceil(conf * 100)
            classindex = int(classindex)
            
            if conf > 50:
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                new_detections = np.array([x1, y1, x2, y2, conf, classindex])
                detections = np.vstack((detections, new_detections))

    basket_detected, backboard_detected, basket_info, backboard_info, conf = process_detections(detections, tracker)

    offset_2= visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf)
    if current_time - last_send_time >= send_interval:
        send_offset_stm(offset_2, latest_distance, ser)
        last_send_time = current_time

    cv2.putText(frame, f'FPS: {fps:.2f}', (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    cv2.imshow('Object Detection', frame)                        
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

pipeline.stop()
cv2.destroyAllWindows()