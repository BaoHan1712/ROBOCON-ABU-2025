import cv2
from ultralytics import YOLO
import math
import numpy as np
from cover.sort import Sort

cap = cv2.VideoCapture("data/clip.mp4")

# Đọc tên các lớp từ file
classnames = ['basket']  

model = YOLO("model\cnn2.onnx")
tracker = Sort(max_age=60)  
prev_frame_time = 0
new_frame_time = 0

last_positions = {}  # Lưu trữ vị trí cuối cùng
time_final = 1

while True:
    ret, frame = cap.read()
    if not ret:
        break

    detections = np.empty((0, 5))
    new_frame_time = cv2.getTickCount()
    frame = cv2.resize(frame, (1080, 720))
    results = model.predict(source=frame, imgsz=320, conf=0.65)
    
    for info in results:
        boxes = info.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            conf = box.conf[0]
            classindex = int(box.cls[0])
            conf = math.ceil(conf * 100)
            
            # Kiểm tra chỉ số lớp có hợp lệ không
            if classindex < len(classnames):
                objectdetect = classnames[classindex]

                if objectdetect == 'basket' and conf > 70:
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
    
    # Nếu mất vật thì dự đoán ra vật bị che khuất
    to_remove = []
    for id, pos in last_positions.items():
        x1, y1, x2, y2, frames_remaining = pos
        if frames_remaining > 0:
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.circle(frame, (cx, cy), 6, (0, 0, 255), -1)
            cv2.putText(frame, f'{objectdetect} {new_id} {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
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
