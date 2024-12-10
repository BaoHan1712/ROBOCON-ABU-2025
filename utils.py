import cv2 
import math

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
    
    # Xác định vùng trung tâm (độ rộng 20 pixel)
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

# Tính khoảng cách trung bình
def get_average_distance(depth_frame, x, y, kernel_size=20):
    try:
        total_dist = 0
        valid_points = 0
        half_kernel = kernel_size // 2
        
        for i in range(-half_kernel, half_kernel + 1):
            for j in range(-half_kernel, half_kernel + 1):
                try:
                    dist = depth_frame.get_distance(int(x + i), int(y + j))
                    if dist > 0:  
                        total_dist += dist
                        valid_points += 1
                except:
                    continue
                    
        if valid_points > 0:
            avg_dist = total_dist / valid_points
            return round(avg_dist * 100, 2)  # Chuyển sang cm
        return 0
        
    except Exception as e:
        print(f"Lỗi khi tính khoảng cách trung bình: {str(e)}")
        return 0
    
def calculator_distance(frame,x1,y2,depth_frame,depth_x,depth_y):
    # Trong vòng lặp chính
    distance_cm = get_average_distance(depth_frame, depth_x, depth_y, kernel_size=20)
    if distance_cm > 0:
        cv2.putText(frame, f'Kc: {distance_cm} cm', (x1, y2 + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)
        
