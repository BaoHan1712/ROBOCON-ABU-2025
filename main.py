import cv2
from ultralytics import YOLO
from utils import calculator_offset_stm32, create_stm32_message
import math
import time
import numpy as np
from cover.sort import Sort
from data_lidar import LidarThread, connect_lidar
import serial

# Tên nhãn
classnames = ['basket']

model = YOLO("model\cnn2.engine", task="detect")
tracker = Sort(max_age=40)

prev_frame_time = 0
new_frame_time = 0

last_send_time = 0 
SEND_DELAY = 0.5  # Thời gian gửi 1 lần xuống

last_positions = {}
time_final = 1

cap = cv2.VideoCapture(0)
lidar_socket = connect_lidar()
lidar_thread = LidarThread(lidar_socket)
lidar_thread.start()

# Kết nối tới STM32
ser = serial.Serial('COM4', 115200, timeout=1)  # Thay đổi COM4 thành cổng thực tế


# Hàm truyền data độ lệch xuống STM32
def send_offset_stm(frame, cx, x1, y2, lidar_thread):
    """Truyền dữ liệu khoảng cách và độ lệch xuống stm32"""
 
    offset = calculator_offset_stm32(frame, cx, x1, y2)

    # Lấy khoảng cách từ lidar thread
    if hasattr(lidar_thread, 'buffer') and lidar_thread.buffer:
        distance = lidar_thread.buffer[-1]  # Lấy giá trị mới nhất từ buffer

    # Tạo tin nhắn gửi xuống STM32
    create_stm32_message(offset, distance, ser)
    print(f"Offset: {offset}, khoang cach:{distance}")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    detections = np.empty((0, 5))
    new_frame_time = cv2.getTickCount()
    frame = cv2.resize(frame, (1080, 720))

    results = model.predict(source=frame, imgsz=640, conf=0.5, verbose=False)

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

            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.circle(frame, (cx, cy), 6, (0, 0, 255), -1)
            cv2.putText(frame, f'{objectdetect} {id} {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

            # Truyền tham số xuống STM32
            send_offset_stm(frame, cx, x1, y2, lidar_thread)

    fps = cv2.getTickFrequency() / (new_frame_time - prev_frame_time)
    prev_frame_time = new_frame_time
    fps_text = f'FPS: {int(fps)}'
    cv2.putText(frame, fps_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)
    cv2.imshow('Object Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
