import cv2
from ultralytics import YOLO
from utils import offset_backboard, process_distance
import math
import serial
import time
import numpy as np
from cover.sort import Sort


cap = cv2.VideoCapture('data\o.MOV')

ser = serial.Serial('COM8', 115200, timeout=1)

# Tên nhãn
classnames = []
with open('class.txt', 'r') as f:
    classnames = f.read().splitlines()

model = YOLO("model\cnn2.onnx")

tracker = Sort(max_age=60)

prev_frame_time = 0
new_frame_time = 0

last_send_time = 0  # Thời gian bắt đầu
SEND_DELAY =  0.5 # Thời gian gửi 1 lần xuống

last_positions = {}
time_final = 1

############## Tham số tính khoảng cách ###############
KNOWN_DISTANCE_BASKET = 5     # Khoảng cách từ camera tới rổ (m)
KNOWN_HEIGHT_BASKET = 2.5   # Chiều cao chuẩn của rổ (m)
CAMERA_HEIGHT = 1   


# Hàm truyền data độ lệch xuống STM32
def calculator_offset_stm(frame, cx, x1, y2):
    global last_send_time

    # Tính toán độ lệch của backboard
    offset = offset_backboard(frame, cx)
    current_time = time.time()  
    if offset < 98:
        cv2.putText(frame, f'lech trai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (125, 55, 155), 2)
    elif offset > 102:
        cv2.putText(frame, f'lech phai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (125, 55, 155), 2)
    else :
       cv2.putText(frame, f'chuan', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (125, 55, 155), 2) 

    if current_time - last_send_time >= SEND_DELAY:
         # Giới hạn cua 1 byte
        if offset < 2:
            offset = 0
        elif offset > 254:
            offset = 255

        # Chuyển đổi offset thành 1 byte không dấu
        offset_bytes = offset.to_bytes(1, byteorder='little', signed=False)

        
        ser.write(offset_bytes)
        print(f"Đã gửi: {offset_bytes.hex()}")
        last_send_time = current_time  

        

while True:
    ret, frame = cap.read()
    if not ret:
        break
    detections = np.empty((0, 5))

    new_frame_time = cv2.getTickCount()
    frame = cv2.resize(frame, (1080, 720))

    results = model.predict(source=frame, imgsz=320, conf = 0.7)

    for info in results:
        boxes = info.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            conf = box.conf[0]
            classindex = box.cls[0]
            conf = math.ceil(conf * 100)
            classindex = int(classindex)
            objectdetect = classnames[classindex]
            
            if conf > 70:
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                new_detections = np.array([x1, y1, x2, y2, conf])
                detections = np.vstack((detections, new_detections))

# Chỉ giữ lại 1 vật thể 
    if detections.shape[0] > 0:
        track_result = tracker.update(detections[:1])
        for results in track_result:
            x1, y1, x2, y2, id = results
            x1, y1, x2, y2, id = int(x1), int(y1), int(x2), int(y2), int(id)
            new_id = 1
            w, h = x2 - x1, y2 - y1
            cx, cy = x1 + w // 2, y1 + h // 2

# Cập nhật vị trí cuối cùng và khung hình còn lại
            last_positions[id] = [x1, y1, x2, y2, time_final] 
    
    to_remove = []
    for id, pos in last_positions.items():
        x1, y1, x2, y2, frames_remaining = pos
        if frames_remaining > 0:
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.circle(frame, (cx, cy), 6, (0, 0, 255), -1)
            cv2.putText(frame, f'{objectdetect} {id} {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
            
        # Truyền tham số xuống stm32
            calculator_offset_stm(frame,cx,x1,y2)
            last_positions[id][4] -= 1
        else:
            to_remove.append(id)
    
    # Xóa các bbox đã hết thời gian
    for id in to_remove:
        del last_positions[id]
    
    fps = cv2.getTickFrequency() / (new_frame_time - prev_frame_time)
    prev_frame_time = new_frame_time
    fps_text = f'FPS: {int(fps)}'
    cv2.putText(frame, fps_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)
    cv2.imshow('Object Detection', frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
