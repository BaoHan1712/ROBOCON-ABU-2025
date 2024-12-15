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
def get_average_distance(depth_frame, x, y, kernel_size=40, calibration_factor=1.0):
    try:
        # Static variables
        if not hasattr(get_average_distance, "prev_distance"):
            get_average_distance.prev_distance = 0
            get_average_distance.kalman_estimate = 0
            get_average_distance.kalman_variance = 1  # Variance of the Kalman filter

        distances = []
        half_kernel = kernel_size // 2

        # Lấy các điểm trong vùng lân cận
        for i in range(-half_kernel, half_kernel + 1):
            for j in range(-half_kernel, half_kernel + 1):
                try:
                    dist = depth_frame.get_distance(int(x + i), int(y + j))
                    if 0 < dist < 10:  # Giới hạn khoảng cách hợp lệ
                        distances.append(dist)
                except:
                    continue

        if len(distances) > 0:
            distances = np.array(distances)
            
            # Loại bỏ nhiễu bằng khoảng tứ phân vị (IQR)
            q1, q3 = np.percentile(distances, [25, 75])
            iqr = q3 - q1
            lower_bound = q1 - 1 * iqr
            upper_bound = q3 + 1 * iqr
            filtered_distances = distances[(distances >= lower_bound) & (distances <= upper_bound)]

            if len(filtered_distances) > 0:
                # Tính giá trị trung bình từ các điểm đã lọc
                current_dist = np.mean(filtered_distances) * calibration_factor

                # Áp dụng bộ lọc Kalman
                kalman_gain = get_average_distance.kalman_variance / (get_average_distance.kalman_variance + 1)
                kalman_estimate = get_average_distance.kalman_estimate + kalman_gain * (current_dist - get_average_distance.kalman_estimate)
                kalman_variance = (1 - kalman_gain) * get_average_distance.kalman_variance

                get_average_distance.kalman_estimate = kalman_estimate
                get_average_distance.kalman_variance = kalman_variance

                # Kiểm tra sự ổn định
                if get_average_distance.prev_distance != 0:
                    diff = abs(kalman_estimate - get_average_distance.prev_distance)
                    if diff < 0.05:  # Nếu thay đổi rất nhỏ
                        kalman_estimate = get_average_distance.prev_distance
                get_average_distance.prev_distance = kalman_estimate

                return round(kalman_estimate * 100, 1)  # Trả về giá trị chính xác
        return get_average_distance.prev_distance
    except Exception as e:
        print(f"Lỗi khi tính khoảng cách trung bình: {str(e)}")
        return get_average_distance.prev_distance if hasattr(get_average_distance, "prev_distance") else 0

    

def calculator_distance(frame, x1, y2, depth_frame, depth_x, depth_y):
    distance_cm = get_average_distance(depth_frame, depth_x, depth_y, kernel_size=40, calibration_factor=1.0)
    if distance_cm > 0:
        cv2.putText(frame, f'Kc: {distance_cm} cm', (x1, y2 + 20), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
        
