import cv2 
import math
import numpy as np




def offset_backboard(frame, cx):
    """
    Tính toán độ lệch giữa tâm vật thể và tâm khung hình
    offset: Độ lệch tính bằng pixel (âm là lệch trái, dương là lệch phải)
    is_centered: True nếu vật nằm trong vùng trung tâm
    """
    # Lấy kích thước khung hình
    frame_height, frame_width = frame.shape[:2]
    
    # Tính tọa độ tâm khung hình 
    frame_center_x = frame_width // 2
    
    # Tính độ lệch
    offset = cx - frame_center_x
    
    center_threshold = 20
    is_centered = abs(offset) <= center_threshold
    
    # Vẽ vùng trung tâm


    cv2.line(frame, (frame_center_x - center_threshold, 0), 
             (frame_center_x - center_threshold, frame_height), (0, 255, 0), 1)
    cv2.line(frame, (frame_center_x + center_threshold, 0),
             (frame_center_x + center_threshold, frame_height), (0, 255, 0), 1)
    
    return offset, is_centered

def calculator_offset(frame, cx, x1, y2):
    offset, is_centered = offset_backboard(frame, cx)
    if is_centered:
        cv2.putText(frame, "Chuan", (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
    else:
        direction = "trai" if offset < 0 else "phai"
        cv2.putText(frame, f"Lech {direction}: {abs(offset)} px", (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
    return offset


from collections import deque

distance_buffer = deque(maxlen=5)  # Lưu tối đa 5 giá trị

def calculator_distance(frame, x1, y2, depth_frame, depth_x, depth_y):
    """
    Hàm tính khoảng cách và hiển thị trên khung hình.
    """
    distance_cm = get_average_distance(depth_frame, depth_x, depth_y, kernel_size=30)
    
    # Lọc giá trị qua buffer để làm mượt
    if distance_cm > 0:
        distance_buffer.append(distance_cm)
    elif distance_buffer:  
        distance_cm = np.mean(distance_buffer)
    
    if distance_cm > 0:
        cv2.putText(frame, f'Kc: {distance_cm:.1f} cm', (x1, y2 + 20),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
    

def get_average_distance(depth_frame, cx, cy, kernel_size=30):
    
    
    """
    Lấy khoảng cách trung bình trong vùng lân cận, có lọc nhiễu.
    """
    width = depth_frame.get_width()
    height = depth_frame.get_height()
    
    if cx < 0 or cx >= width or cy < 0 or cy >= height:
        return 0.0

    # Tăng kích thước kernel cho khoảng cách xa
    if kernel_size < 50:
        kernel_size = 50  # Tăng kích thước vùng lấy mẫu

    # Lấy vùng lân cận xung quanh điểm (cx, cy)
    x_start = max(0, cx - kernel_size // 2)
    x_end = min(width, cx + kernel_size // 2 + 1)
    y_start = max(0, cy - kernel_size // 2)
    y_end = min(height, cy + kernel_size // 2 + 1)

    # Thu thập các giá trị độ sâu hợp lệ trong vùng
    depth_values = []
    for py in range(y_start, y_end):
        for px in range(x_start, x_end):
            depth = depth_frame.get_distance(px, py)
            if 0 < depth < 15:  # Tăng giới hạn khoảng cách lên 15m
                depth_values.append(depth)

    if not depth_values:
        return 0.0

    # Lọc nhiễu bằng phương pháp trung vị
    depth_values = np.array(depth_values)
    median_depth = np.median(depth_values)
    
    # Chỉ lấy các giá trị trong khoảng ±15% của trung vị
    valid_mask = np.abs(depth_values - median_depth) <= 0.15 * median_depth
    filtered_values = depth_values[valid_mask]

    if len(filtered_values) == 0:
        return 0.0

    # Tính trung bình sau khi đã lọc
    filtered_depth = np.mean(filtered_values)
    
    # Làm tròn đến 1 chữ số thập phân
    return round(filtered_depth * 100, 1)  # Chuyển đổi sang cm


        
