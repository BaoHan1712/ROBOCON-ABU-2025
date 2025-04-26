import cv2
import pyrealsense2 as rs
import numpy as np
from ultralytics import YOLO
from cover.utils import *
import math
from cover.sort import Sort
import serial
import queue
import time
import re
import threading
from collections import deque


MAX_QUEUE_SIZE = 100
SERIAL_TIMEOUT = 0.1

# Khởi tạo biến global để theo dõi trạng thái kết nối
ser_stm32 = None
ser_esp32 = None
is_esp32_connected = False
is_stm32_connected = False

def init_serial_connections():
    """Khởi tạo kết nối serial với xử lý lỗi"""
    global ser_stm32, ser_esp32, is_esp32_connected, is_stm32_connected
    
    # Thử kết nối với STM32
    try:
        ser_stm32 = serial.Serial('/dev/ttyUSB0', 115200)
        is_stm32_connected = True
        print("✅ Đã kết nối với STM32")
    except serial.SerialException as e:
        print(f"⚠️ Không thể kết nối với STM32: {e}")
        is_stm32_connected = False
    
    # Thử kết nối với ESP32
    try:
        ser_esp32 = serial.Serial('/dev/ttyUSB1', 115200, timeout=1)
        is_esp32_connected = True
        print("✅ Đã kết nối với ESP32")
    except serial.SerialException as e:
        print(f"⚠️ Không thể kết nối với ESP32: {e}")
        is_esp32_connected = False


# Queue và cache
distance_queue = queue.Queue(maxsize=MAX_QUEUE_SIZE)
detection_cache = deque(maxlen=5)  # Cache lưu 5 detection gần nhất

# Khởi tạo model và tracker
model = YOLO(r"/home/phongvu/Downloads/robocon/model/rim_robot.engine", task="detect")
tracker = Sort(max_age=50)

# Cấu hình Intel RealSense D435
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.color, 848, 480, rs.format.bgr8, 60)
profile = pipeline.start(config)

align_to = rs.stream.color
align = rs.align(align_to)

def receive_thread():
    """Luồng nhận dữ liệu từ ESP32"""
    global is_esp32_connected
    
    while True:
        if not is_esp32_connected:
            print("⏳ Đang thử kết nối lại với ESP32...")
            try:
                init_serial_connections()
                time.sleep(5)  # Đợi 5 giây trước khi thử lại
                continue
            except:
                time.sleep(5)
                continue
        
        try:
            if ser_esp32.in_waiting:
                line = ser_esp32.readline()
                if line:
                    line = line.strip().decode('utf-8')
                    match = re.search(r'\d+', line)
                    if match:
                        distance = int(match.group(0))
                        print(f"✅ Khoảng cách nhận được: {distance} mm")
                        distance_queue.put(distance)
                    
        except serial.SerialException:
            print("❌ Mất kết nối với ESP32")
            is_esp32_connected = False
            time.sleep(1)
        except Exception as e:
            print(f"Lỗi ở luồng nhận: {e}")
            time.sleep(0.1)

def send_offset_stm(offset, distance, ser):
    """Truyền dữ liệu với kiểm tra kết nối"""
    global is_stm32_connected
    
    if not is_stm32_connected:
        return
        
    try:
        offset = int(offset) if offset is not None else 1
        # Nếu không có distance thì gửi giá trị mặc định là 1
        distance = int(distance) if distance is not None else 1
        create_stm32_message_1(offset, distance, ser)
    except serial.SerialException:
        print("❌ Mất kết nối với STM32")
        is_stm32_connected = False
    except Exception as e:
        print(f"Lỗi gửi dữ liệu: {e}")

# Tính khoảng cách bù trừ
adjacent = 350
def calculate_angle(deviation_pixels):
    """Tính góc với numpy vectorization"""
    
    return np.sqrt(adjacent**2 + deviation_pixels**2)

def visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf):
    """Hiển thị kết quả với ROI và caching"""
    quydoi = 0
    frame_height, frame_width = frame.shape[:2]
    frame_center_x = frame_width // 2

    if basket_detected:
        x1, y1, x2, y2, id = map(int, basket_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
        
        # Lấy offset và position
        offset = calculator_offset_stm32(frame, cx, x1, y2)
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
        draw_plus_sign(frame, (cx,cy), 5, (0,255,0), 1)
        cv2.putText(frame, f'basket {conf}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    
    elif backboard_detected:
        x1, y1, x2, y2, id = map(int, backboard_info)
        w, h = x2 - x1, y2 - y1
        cx, cy = x1 + w // 2, y1 + h // 2
        
        temp_offset = calculator_offset_stm32(frame, cx, x1, y2)
        canhdoi = temp_offset * 2.8
        
        # Điểm bắt đầu của đường thẳng đứng (cạnh đối)
        start_y = cy + 200
        
        # 1. Vẽ đường thẳng đứng đi qua tâm vật thể - cạnh đối (xanh dương)
        cv2.line(frame, (cx, cy), (cx, start_y), (255, 0, 0), 1)
        
        # 2. Vẽ đường ngang - cạnh kề (xanh lá)
        cv2.line(frame, (frame_center_x, cy), (cx, cy), (0, 255, 0), 1)
        
        # 3. Vẽ đường chéo - cạnh huyền (đỏ)
        cv2.line(frame, (cx, start_y), (frame_center_x, cy), (0, 0, 255), 1)
        
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
       
        cv2.putText(frame, f'backboard {conf} ', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
        
    return quydoi

def main():
    prev_time = time.time()
    
    # Chỉ khởi động thread nhận nếu có kết nối ESP32
    if is_esp32_connected:
        receiver = threading.Thread(target=receive_thread)
        receiver.daemon = True
        receiver.start()
    
    last_distance = None

    try:
        while True:
            current_time = time.time()
            fps = 1 / (current_time - prev_time)
            prev_time = current_time

            frames = pipeline.wait_for_frames()
            aligned_frames = align.process(frames)
            color_frame = aligned_frames.get_color_frame()

            if not color_frame:
                continue

            frame = np.asanyarray(color_frame.get_data())
            detections = np.empty((0, 6))
            results = model.predict(source=frame, imgsz=480, conf=0.50, verbose=False, max_det=1)
            
            # Xử lý YOLO detection
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

            basket_detected, backboard_detected, basket_info, backboard_info, conf = process_detections(detections, tracker)

            # Chỉ thử đọc khoảng cách nếu có kết nối ESP32
            if is_esp32_connected:
                try:
                    distance = distance_queue.get_nowait()
                    last_distance = distance
                except queue.Empty:
                    distance = last_distance
            else:
                distance = None

            offset_2 = visualize_detections(frame, basket_detected, backboard_detected, basket_info, backboard_info, conf)
            
            # Luôn gửi dữ liệu khi có offset, bất kể có distance hay không
            if offset_2 is not None and is_stm32_connected:
                send_offset_stm(offset_2, distance, ser_stm32)

            # Hiển thị trạng thái kết nối và distance
            status_text = []
            if not is_esp32_connected:
                status_text.append("ESP32: Disconnected")
            if not is_stm32_connected:
                status_text.append("STM32: Disconnected")
            
            cv2.putText(frame, f'FPS: {fps:.2f}', (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            
            # Hiển thị trạng thái kết nối
            y_pos = 80
            for text in status_text:
                cv2.putText(frame, text, (20, y_pos), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
                y_pos += 30

            cv2.imshow('Object Detection', frame)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    except KeyboardInterrupt:
        print("\nĐang dừng chương trình...")
    finally:
        pipeline.stop()
        cv2.destroyAllWindows()
        if is_stm32_connected and ser_stm32:
            ser_stm32.close()
        if is_esp32_connected and ser_esp32:
            ser_esp32.close()

if __name__ == "__main__":
    main()