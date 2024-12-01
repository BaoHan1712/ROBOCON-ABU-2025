import cv2 

send_once = False

def offset_backboard(frame_2,cx):
# Tính toán trung tâm khung hình
    frame_center_x = frame_2.shape[1] // 2
    cv2.line(frame_2, (frame_center_x, 0), (frame_center_x, frame_2.shape[0]), (255, 0, 0), 1)
    offset = cx - frame_center_x
    return offset

def calculator_offset(frame, cx, x1, y2 , ser):
    global send_once 
    
    # Gọi hàm offset
    offset = offset_backboard(frame, cx)

    if offset < -5:
        cv2.putText(frame, f'lech trai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (125, 55, 155), 2)
        send_once = False  
    elif offset > 5:
        cv2.putText(frame, f'lech phai: {abs(offset)} px', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (125, 55, 155), 2)
        send_once = False  
    elif -5 < offset < 5:
        cv2.putText(frame, 'Dung tam', (x1, y2 + 40), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
        
        # Chỉ gửi nếu chưa gửi
        if not send_once:
            ser.write(b'1')  
            send_once = True  

    return offset

# Tính toán tiêu cự dựa trên khoảng cách và chiều cao thực tế
def calculate_focal_length(known_distance, real_height, pixel_height):
    return (pixel_height * known_distance) / real_height

# Tính khoảng cách từ cam đến vật thể
def calculate_distance(focal_length, real_height, pixel_height):
    return (real_height * focal_length) / pixel_height


FOCAL_LENGTH = None

# Hàm tính khoảng cách 
def process_distance(frame, x1, y1, y2, known_height,KNOWN_DISTANCE):

    global FOCAL_LENGTH

    # Tính chiều cao pixel của đối tượng
    pixel_height =  y2 - y1

    if FOCAL_LENGTH is None:
        FOCAL_LENGTH = calculate_focal_length(KNOWN_DISTANCE, known_height, pixel_height)

    distance = calculate_distance(FOCAL_LENGTH, known_height, pixel_height)
    distance = distance / 100
    cv2.putText(frame, f'Kc:{distance:.1f}m', (x1, y2 + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

    return distance

