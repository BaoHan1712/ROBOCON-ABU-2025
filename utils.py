import cv2 
import math

def offset_backboard(frame_2,cx):
# Tính toán trung tâm khung hình
    frame_center_x = frame_2.shape[1] // 2
    cv2.line(frame_2, (frame_center_x, 0), (frame_center_x, frame_2.shape[0]), (255, 0, 0), 1)
    offset = cx - frame_center_x
    return offset


# Tính toán tiêu cự dựa trên khoảng cách và chiều cao thực tế
def calculate_focal_length(known_distance, real_height, pixel_height):
    return (pixel_height * known_distance) / real_height

# Tính khoảng cách từ cam đến vật thể
def calculate_distance(focal_length, real_height, pixel_height):
    return (real_height * focal_length) / pixel_height

FOCAL_LENGTH = None # Tiêu cự của camera
# Tính khoảng cách thực từ camera đến rổ     
def calculate_real_distance(distance, CAMERA_HEIGHT, KNOWN_HEIGHT_BASKET):
    """
    Tính khoảng cách thực từ camera đến rổ dựa trên độ cao
    Args:
        distance: Khoảng cách phát hiện được từ camera
        camera_height: Độ cao của camera (m)
        basket_height: Độ cao của rổ (m)
    """
    height_diff = KNOWN_HEIGHT_BASKET - CAMERA_HEIGHT  # Độ chênh lệch độ cao
    real_distance = math.sqrt(distance**2 + height_diff**2)
    return real_distance

# Hàm tính khoảng cách 
def process_distance(frame, x1, y1, y2, KNOWN_DISTANCE, CAMERA_HEIGHT, KNOWN_HEIGHT_BASKET):

    global FOCAL_LENGTH

    # Tính chiều cao pixel của đối tượng
    pixel_height =  y2 - y1

    if FOCAL_LENGTH is None:
        FOCAL_LENGTH = calculate_focal_length(KNOWN_DISTANCE, KNOWN_HEIGHT_BASKET, pixel_height)

    # Tính khoảng cách thẳng từ camera
    straight_distance = calculate_distance(FOCAL_LENGTH, KNOWN_HEIGHT_BASKET, pixel_height)

    real_distance = calculate_real_distance(straight_distance, CAMERA_HEIGHT, KNOWN_HEIGHT_BASKET)
    distance_m = round(real_distance / 100, 1)

    cv2.putText(frame, f'Kc:{distance_m}m', (x1, y2 + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)
    
    return real_distance

# Tính toán góc bắn và lực bắn
# def calculate_shooting_params(distance, HEIGHT_camera, HEIGHT_basket):
#     if callable(distance):  # Kiểm tra xem distance có phải là hàm không
#         raise TypeError("Distance must be a number, not a function")
    
#     g = 980  # Gia tốc trọng trường
#     distance = (distance) / 100  
    
#     height_diff = float(HEIGHT_basket - HEIGHT_camera)
#     height_diff = float(height_diff / 100)
    
#     # Tính góc bắn 
#     angle = math.degrees(math.atan((4 * height_diff + math.sqrt(4 * height_diff**2 + distance**2)) / distance))
    
#     # Tính vận tốc ban đầu 

#     v0 = math.sqrt((g * distance**2) / (2 * math.cos(math.radians(angle))**2 * (distance * math.tan(math.radians(angle)) - height_diff)))
    
#     # Chuyển đổi vận tốc thành % lực bắn (giả sử vận tốc tối đa là 10 m/s)
#     max_velocity = 1000  # cm/s
#     power = min((v0 * 100) / max_velocity * 100, 100)
    
#     return angle, power

# # Vẽ thông số bắn lên frame
# def draw_shooting_params(frame, x1, y2, distance, HEIGHT_camera, HEIGHT_basket):
#     angle, power = calculate_shooting_params(distance, HEIGHT_camera, HEIGHT_basket)
    
#     cv2.putText(frame, f'Goc ban: {angle:.1f} do', (x1, y2 + 60), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
#     # cv2.putText(frame, f'Luc ban: {power:.1f}%', (x1, y2 + 80), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
    
#     return angle, power