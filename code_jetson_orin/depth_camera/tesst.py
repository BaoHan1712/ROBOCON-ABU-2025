import cv2
import pyrealsense2 as rs
import numpy as np
from ultralytics import YOLO
import math

# Khởi tạo mô hình YOLO
model = YOLO("model/ver4_2class.pt", task="detect")


prev_frame_time = 0 
new_frame_time = 0

# Cấu hình Intel RealSense D435
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
profile = pipeline.start(config)

# Tỉ lệ chuyển đổi từ đơn vị ảnh sang đơn vị thực tế
depth_scale = profile.get_device().first_depth_sensor().get_depth_scale()

# Căn chỉnh depth với ảnh màu
align_to = rs.stream.color
align = rs.align(align_to)


depth_history = {}

ALPHA = 0.1  # Trọng số bộ lọc trung bình động


def get_distance(depth_frame, x, y, obj_id, history_size=25):
    """Lấy khoảng cách từ Depth Camera với bộ lọc trung bình động."""
    depth = depth_frame.get_distance(x, y) * 1000  

    if depth == 0 or depth > 5000:  # Bỏ qua giá trị không hợp lệ (> 5m)
        return None  

    if obj_id not in depth_history:
        depth_history[obj_id] = []
    

    # Lấy giá trị trước đó
    prev_depth = depth_history[obj_id][-1] if depth_history[obj_id] else depth


    # Áp dụng bộ lọc trung bình động
    smoothed_depth = ALPHA * depth + (1 - ALPHA) * prev_depth
    depth_history[obj_id].append(smoothed_depth)

    # Giữ kích thước lịch sử cố định
    if len(depth_history[obj_id]) > history_size:
        depth_history[obj_id].pop(0)

    return round(smoothed_depth)


while True:
    frames = pipeline.wait_for_frames()
    aligned_frames = align.process(frames)
    depth_frame = aligned_frames.get_depth_frame()
    color_frame = aligned_frames.get_color_frame()

    if not depth_frame or not color_frame:
        continue

    frame = np.asanyarray(color_frame.get_data())
    detections = np.empty((0, 6))
    new_frame_time = cv2.getTickCount()
    results = model.predict(source=frame, imgsz=640, conf=0.6, verbose=False)
    
    for info in results:
        boxes = info.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            conf = box.conf[0]
            classindex = box.cls[0]
            conf = math.ceil(conf * 100)
            classindex = int(classindex)
            
            if conf > 60:
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)

                # Tính tọa độ trung tâm bbox
                cx, cy = (x1 + x2) // 2, (y1 + y2) // 2

                # Lấy ID từ tracker (nếu có) hoặc gán mặc định
                obj_id = f"{x1}-{y1}-{x2}-{y2}"

                # Lấy khoảng cách ổn định bằng bộ lọc trung vị
                distance = get_distance(depth_frame, cx, cy, obj_id)
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
                
                # Hiển thị khoảng cách trên khung hình
                if distance:
                    cv2.putText(frame, f'Dist: {distance}mm', (x1, y1 - 10), 
                                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 2)

                new_detections = np.array([x1, y1, x2, y2, conf, classindex])
                detections = np.vstack((detections, new_detections))

    fps = cv2.getTickFrequency() / (new_frame_time - prev_frame_time)
    prev_frame_time = new_frame_time
    fps_text = f'FPS: {int(fps)}'
    cv2.putText(frame, fps_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)
    cv2.imshow('Object Detection', frame)                        
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

pipeline.stop()
cv2.destroyAllWindows()
