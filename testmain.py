import cv2
from ultralytics import YOLO
from utils import *
import math
import serial
import numpy as np
from cover.sort import Sort
# from data_lidar import LidarThread,  connect_lidar

# ser = serial.Serial('/dev/ttyUSB0', 115200)

classnames = ['backboard', 'basket']

model = YOLO("model\cnn_2cls_ver2.engine", task="detect")

tracker = Sort(max_age=90)

prev_frame_time = 0
new_frame_time = 0

cap = cv2.VideoCapture("data\che.mp4")

# lidar_socket = connect_lidar()
# lidar_thread = LidarThread(lidar_socket)
# lidar_thread.start()

# def send_offset_stm(frame, cx, x1, y2, min_distance):
#     """Truyền dữ liệu khoảng cách và độ lệch xuống stm32"""
#     # offset = calculator_offset_stm32(frame, cx, x1, y2)
#     # distance = int(min_distance) if min_distance is not None else 0  
#     # create_stm32_message_1(offset, distance, ser)



def visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf):
    """Hiển thị kết quả phát hiện lên frame"""
    if basket_detected:
        x1, y1, x2, y2, id = map(int, basket_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
        
        # send_offset_stm(frame, cx, x1, y2, 24)
        print("đã truyền basket")
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.circle(frame, (cx, cy), 6, (0, 0, 255), -1)
        cv2.putText(frame, f'basket {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    
    elif backboard_detected:
        x1, y1, x2, y2, id = map(int, backboard_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
        
        # send_offset_stm(frame, cx, x1, y2, 24)
        print("đã truyền backboard")
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.circle(frame, (cx, cy), 6, (0, 0, 255), -1)
        cv2.putText(frame, f'backboard {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    detections = np.empty((0, 6))
    new_frame_time = cv2.getTickCount()
    frame = cv2.resize(frame, (740, 640))
    results = model.predict(source=frame, imgsz=640, conf=0.5, verbose=False)
    
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
    visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf)

    fps = cv2.getTickFrequency() / (new_frame_time - prev_frame_time)
    prev_frame_time = new_frame_time
    fps_text = f'FPS: {int(fps)}'
    cv2.putText(frame, fps_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)
    cv2.imshow('Object Detection', frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
