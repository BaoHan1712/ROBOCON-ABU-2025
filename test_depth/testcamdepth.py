import cv2
from ultralytics import YOLO
from utils import calculator_offset, calculator_distance
import math
# import serial
import time
import numpy as np
from cover.sort import Sort
import pyrealsense2 as rs


# Khởi tạo pipeline
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.color,1280, 720, rs.format.bgr8, 30)
config.enable_stream(rs.stream.depth, 1280, 720, rs.format.z16, 30)

# # Lấy thông tin về các chế độ được hỗ trợ
pipeline_profile = pipeline.start(config)



# Tạo align object để căn chỉnh depth với màu
align_to = rs.stream.color
align = rs.align(align_to)

# Tên nhãn
classnames = ['basketball']

model = YOLO("model\cnn2.engine")

tracker = Sort(max_age=60)

prev_frame_time = 0
new_frame_time = 0

last_send_time = 0  # Thời gian bắt đầu
SEND_DELAY =  0.5 # Thời gian gửi 1 lần xuống

last_positions = {}
time_final = 1


# Hàm truyền data độ lệch xuống STM32
def calculator_offset_stm(frame, cx, x1, y2):
    global last_send_time
    offset = calculator_offset(frame, cx, x1, y2)
    
    # Tính toán độ lệch và kiểm tra vị trí trung tâm
    current_time = time.time()
    

    if current_time - last_send_time >= SEND_DELAY:
        # Giới hạn offset trong khoảng [0, 255]
        offset_byte = max(1, min(255, abs(int(offset))))
        
        # Thêm bit dấu (0: lệch trái, 1: lệch phải)
        offset_with_sign = offset_byte if offset >= 0 else offset_byte + 128
        
        # Chuyển thành bytes
        # offset_bytes = offset_with_sign.to_bytes(1, byteorder='little', signed=False)
        last_send_time = current_time
        
        # return offset_bytes
    
    return None

try:
    while True:
        # Đợi frame mới
        frames = pipeline.wait_for_frames()
        
        # Căn chỉnh frame
        aligned_frames = align.process(frames)
        
        # Lấy frame màu và depth đã căn chỉnh
        color_frame = aligned_frames.get_color_frame()
        depth_frame = aligned_frames.get_depth_frame()
        
        if not color_frame or not depth_frame:
            continue

        # Chuyển đổi frame thành numpy array
        frame = np.asanyarray(color_frame.get_data())
        depth_image = np.asanyarray(depth_frame.get_data())

        detections = np.empty((0, 5))

        new_frame_time = cv2.getTickCount()
        frame = cv2.resize(frame, (1080, 720))

        results = model.predict(source=frame, imgsz=640, conf = 0.55, verbose=False)

        for info in results:
            boxes = info.boxes
            for box in boxes:
                x1, y1, x2, y2 = box.xyxy[0]
                conf = box.conf[0]
                classindex = box.cls[0]
                conf = math.ceil(conf * 100)
                classindex = int(classindex)
                objectdetect = classnames[classindex]
                
                if conf > 35:
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
               
                calculator_distance(frame,x1,y2,depth_frame,cx,cy)
               
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

finally:
    pipeline.stop()
    cv2.destroyAllWindows()
