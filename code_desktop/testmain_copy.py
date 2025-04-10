import cv2
import numpy as np
import math
from ultralytics import YOLO
from cover.sort import Sort
from utils import draw_plus_sign, calculator_offset_stm32

# Khởi tạo model
model = YOLO("model/segment.pt", task="segment")
tracker = Sort(max_age=40)

# Mở video
cap = cv2.VideoCapture("data/chero.mp4")
prev_frame_time = 0

adjacent = 500  # Cạnh kề cố định cho tính góc

def calculate_angle(deviation_pixels):
    opposite = deviation_pixels
    hypotenuse = math.sqrt(adjacent ** 2 + opposite ** 2)
    return hypotenuse

def visualize_backboard_center(frame, detections, results):
    """Vẽ tâm của mask bảng rổ"""
    offset = None
    center_x, center_y = None, None
    frame_center_x = frame.shape[1] // 2

    if results[0].masks is None:
        return offset  # Không có segment nào

    boxes = results[0].boxes
    masks = results[0].masks.xy

    for box, seg in zip(boxes, masks):
        class_id = int(box.cls[0])
        conf = float(box.conf[0])

        if conf > 0.5 and class_id == 0:
            pts = np.array(seg, np.int32)
            M = cv2.moments(pts)

            if M["m00"] != 0:
                center_x = int(M["m10"] / M["m00"])
                center_y = int(M["m01"] / M["m00"])

                # Vẽ contour + tâm
                cv2.polylines(frame, [pts], True, (0, 255, 0), 2)
                draw_plus_sign(frame, (center_x, center_y), 6, (0, 255, 255), 2)
                cv2.circle(frame, (center_x, center_y), 8, (0, 0, 255), 2)

                # Vẽ tam giác vuông
                start_y = center_y + 200
                cv2.line(frame, (center_x, center_y), (center_x, start_y), (255, 0, 0), 2)  # cạnh đứng
                cv2.line(frame, (frame_center_x, center_y), (center_x, center_y), (0, 255, 0), 2)  # cạnh ngang
                cv2.line(frame, (center_x, start_y), (frame_center_x, center_y), (0, 0, 255), 2)  # cạnh huyền

                offset = calculator_offset_stm32(frame, center_x, frame_center_x, center_y)

                if offset is not None:
                    offset_mm = offset * 10
                    hypotenuse = calculate_angle(offset_mm)
                    canh_huyen = int(hypotenuse) - 500
                    cv2.putText(frame, f"Distance: {canh_huyen}mm", (center_x + 10, center_y - 20),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

    return offset

while True:
    ret, frame = cap.read()
    if not ret:
        cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
        continue

    new_frame_time = cv2.getTickCount()
    frame = cv2.resize(frame, (740, 640))
    detections = np.empty((0, 6))

    results = model.predict(source=frame, imgsz=640, conf=0.5, verbose=False, max_det=2)
    
    for info in results:
        boxes = info.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            conf = box.conf[0]
            classindex = box.cls[0]
            conf = math.ceil(conf * 100)
            classindex = int(classindex)
            if conf > 50:
                new_detections = np.array([int(x1), int(y1), int(x2), int(y2), conf, classindex])
                detections = np.vstack((detections, new_detections))

    offset = visualize_backboard_center(frame, detections, results)

    fps = cv2.getTickFrequency() / (new_frame_time - prev_frame_time)
    prev_frame_time = new_frame_time
    cv2.putText(frame, f'FPS: {int(fps)}', (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

    cv2.imshow('Segmented Backboard Detection', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
