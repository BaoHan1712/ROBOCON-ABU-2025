import cv2
from ultralytics import YOLO
from cover.utils import *
import math
import serial
import numpy as np
from cover.sort import Sort
from lidar_code.data_lidar import LidarThread,  connect_lidar

classnames = ['basket']

ser = serial.Serial('/dev/ttyUSB0', 115200 )

model = YOLO("model/cnn2.engine", task="detect")

tracker = Sort(max_age=40)

prev_frame_time = 0
new_frame_time = 0

last_positions = {}
time_final = 1

cap = cv2.VideoCapture(0)

lidar_socket = connect_lidar()
lidar_thread = LidarThread(lidar_socket)
lidar_thread.start()

def send_offset_stm(frame, cx, x1, y2, min_distance):
    """Truyền dữ liệu khoảng cách và độ lệch xuống stm32"""
    offset = calculator_offset_stm32(frame, cx, x1, y2)
    distance = int(min_distance) if min_distance is not None else 0  
    create_stm32_message_1(offset, distance, ser)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    detections = np.empty((0, 5))
    new_frame_time = cv2.getTickCount()
    frame = cv2.resize(frame, (740, 640))
    results = model.predict(source=frame, imgsz=640, conf = 0.5, verbose=False)
        
    min_distance = lidar_thread.get_min_distance()

    for info in results:
        boxes = info.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            conf = box.conf[0]
            classindex = box.cls[0]
            conf = math.ceil(conf * 100)
            classindex = int(classindex)
            objectdetect = classnames[classindex]
            
            if conf > 50:
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                new_detections = np.array([x1, y1, x2, y2, conf])
                detections = np.vstack((detections, new_detections))

    # Chỉ giữ lại 1 vật thể 
    if detections.shape[0] > 0:
        track_result = tracker.update(detections[:1])
        for results in track_result:
            x1, y1, x2, y2, id = results
            x1, y1, x2, y2, id = int(x1), int(y1), int(x2), int(y2), int(id)
            w, h = x2 - x1, y2 - y1
            cx, cy = x1 + w // 2, y1 + h // 2
            
            send_offset_stm(frame, cx, x1, y2, min_distance)
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            draw_plus_sign(frame,(cx,cy),5,(0,255,0),1)
            cv2.putText(frame, f'{objectdetect} {id} {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

    fps = cv2.getTickFrequency() / (new_frame_time - prev_frame_time)
    prev_frame_time = new_frame_time
    fps_text = f'FPS: {int(fps)}'
    cv2.putText(frame, fps_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)
    cv2.imshow('Object Detection', frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
