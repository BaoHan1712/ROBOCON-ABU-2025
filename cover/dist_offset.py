import cv2
from ultralytics import YOLO
from cover.utils_main import process_distance, offset_backboard
# import serial
import time  

cap = cv2.VideoCapture('data/o.MOV')

# ser = serial.Serial('COM4', 115200, timeout=1)

class_name = ['backboard', 'ball', 'basket']
model = YOLO("model/cnn.onnx")

################# Sửa lại thông số trong này  ##############
KNOWN_DISTANCE_BACKBOARD = 500  # Khoảng cách từ camera tới backboard
KNOWN_HEIGHT_BACKBOARD = 240  # Chiều cao của backboard 

KNOWN_DISTANCE_BASKET = 50  # Khoảng cách từ camera tới rổ
KNOWN_HEIGHT_BASKET = 30  # Chiều cao của rổ

prev_frame_time = 0
new_frame_time = 0

send_once = False
last_send_time = 0  # Thời điểm gửi tín hiệu gần nhất
SEND_DELAY = 1  # Thời gian delay giữa các lần gửi tín hiệu (1 giây)

def calculator_offset(frame, cx):
    global send_once, last_send_time
    
    offset = offset_backboard(frame, cx)
    cv2.putText(frame, f'lech: {abs(offset)} px', (120, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (125, 55, 155), 2)
    current_time = time.time()  
    return offset


        
        # # Chỉ gửi nếu chưa gửi và đã qua thời gian delay
        # if not send_once and (current_time - last_send_time >= SEND_DELAY):
        #     ser.write(b'1')  
        #     print("Da gui")
        #     send_once = True  
        #     last_send_time = current_time  

while True:
    ret, frame = cap.read()
    if not ret:
        break

    new_frame_time = cv2.getTickCount()
    frame = cv2.resize(frame, (1080, 720))

    results = model.predict(source=frame, imgsz=320, conf=0.65)

    for result in results:
        boxes = result.boxes
        for box in boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0])
            cls = int(box.cls[0])

            if cls == 0:  
                label = f'{class_name[cls]}'

                # Vẽ đường tròn tại trung tâm
                cx, cy = (x1 + x2) // 2, (y1 + y2) // 2
                cv2.circle(frame, (cx, cy), 6, (0, 0, 255), -1)
                calculator_offset(frame, cx)

                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

            elif cls == 2:  
                label = f'{class_name[cls]}'

                process_distance(frame, x1, y1, y2, KNOWN_HEIGHT_BASKET, KNOWN_DISTANCE_BASKET)
                cx, cy = (x1 + x2) // 2, (y1 + y2) // 2
                cy -= 110 
                cv2.circle(frame, (cx, cy), 6, (124, 0, 125), -1)

                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

    fps = cv2.getTickFrequency() / (new_frame_time - prev_frame_time)
    prev_frame_time = new_frame_time

    fps_text = f'FPS: {int(fps)}'
    cv2.putText(frame, fps_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)

    cv2.imshow('Object Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
