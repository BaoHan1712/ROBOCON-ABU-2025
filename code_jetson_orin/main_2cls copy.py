import cv2
from ultralytics import YOLO
from cover.utils import *
import math
# import serial
import numpy as np
from cover.sort import Sort


# ser = serial.Serial('/dev/ttyUSB0', 115200)


model = YOLO(r"model\cnn_2cls_ver2.engine", task="detect")

tracker = Sort(max_age=40)

prev_frame_time = 0 
new_frame_time = 0

# Thêm biến để kiểm soát vòng lặp video
video_path = 0
cap = cv2.VideoCapture(video_path)

ser = 1

def send_offset_stm( offset ,min_distance, direction):
    """Truyền dữ liệu khoảng cách và độ lệch xuống stm32"""
    if offset is not None:
        offset = int(offset) 
    else:
        offset = 100

    distance = int(min_distance) if min_distance is not None else 11
    create_stm32_message_1(offset, distance, direction, ser)

def visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf):
    """Hiển thị kết quả phát hiện lên frame"""
    offset = None
    min_distance = None
    quydoi = 0

    # Lấy kích thước frame
    frame_height, frame_width = frame.shape[:2]
    frame_center_x = frame_width // 2

    if basket_detected:
        x1, y1, x2, y2, id = map(int, basket_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
        
        # Lấy offset cho rổ
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
        canhdoi = temp_offset * 7
        
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
            cv2.putText(frame, f"Distance: {quydoi:.1f}mm", (cx + 10, cy + 20),
                       cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
                       
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
        cv2.putText(frame, f'backboard {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
        
    return quydoi

adjacent = 350  # cạnh kề
def calculate_angle(deviation_pixels):
   
    
    opposite = deviation_pixels  # cạnh đối

    # Tính cạnh huyền
    hypotenuse = math.sqrt(adjacent**2 + opposite**2)

    return hypotenuse

while True:
    ret, frame = cap.read()
    if not ret:
        cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
        ret, frame = cap.read()
        if not ret:
            break

    detections = np.empty((0, 6))
    new_frame_time = cv2.getTickCount()
    frame = cv2.resize(frame, (640, 480))
    results = model.predict(source=frame, imgsz=640, conf=0.5, verbose=False, max_det=2)
    
    ## lấy khoảng cách từ lidar
    # min_distance = lidar_thread.get_min_distance()
    
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

    # Xử lý vật thể
    basket_detected, backboard_detected, basket_info, backboard_info, conf = process_detections(detections, tracker)
    offset_2 = visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf)
    # send_offset_stm(offset_2, min_distance,quydoi,ser)
    
    fps = cv2.getTickFrequency() / (new_frame_time - prev_frame_time)
    prev_frame_time = new_frame_time
    fps_text = f'FPS: {int(fps)}'
    cv2.putText(frame, fps_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)
    cv2.imshow('Object Detection', frame)                       
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
