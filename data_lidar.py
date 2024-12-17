import socket
import re
import threading
from collections import deque

# Địa chỉ IP của radar
HOST = "169.254.222.142"  
PORT = 2112  # Port SOPAS

# Hàm convert từ hex sang decimal
def hex_to_decimal(hex_value):
    return int(hex_value, 16)

# Hàm tính khoảng cách (mét) từ giá trị decimal, với độ chính xác cao hơn
def convert_to_meters(value, scale_factor=0.001, offset=0.0):  
    return (value * scale_factor) + offset

# Hàm kết nối tới lidar
def connect_lidar():
    """Kết nối tới radar và trả về socket đã kết nối."""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    print("Kết nối thành công!")
    return s

# Hàm tách và chuyển đổi dữ liệu khoảng cách
def process_distance_data(dist_to_rssi_data):
    dist_values = dist_to_rssi_data.split()
    distances_in_meters = [convert_to_meters(hex_to_decimal(val), scale_factor=0.001, offset=0.0) for val in dist_values]
    
    # Kiểm tra nếu không có giá trị nào hợp lệ
    if not distances_in_meters:
        return 0.0  # Trả về 0 nếu không có giá trị nào

    average_distance = sum(distances_in_meters) / len(distances_in_meters)
    convert_distance = round(average_distance * 1000, 2)  # Tính toán và làm tròn đến 2 chữ số thập phân
    return convert_distance

# Hàm chống nhiễu dựa trên buffer
def filter_noise_with_buffer(current_frame, buffer, threshold=500):
    """
    - current_frame: Khoảng cách hiện tại.
    - buffer: Bộ đệm lưu các giá trị trước đó.
    - threshold: Ngưỡng chênh lệch để chấp nhận giá trị mới.
    """
    if not buffer:
        # Nếu buffer rỗng, thêm giá trị đầu tiên và chấp nhận
        buffer.append(current_frame)
        return current_frame, True

    # Tính giá trị trung bình của buffer
    average = sum(buffer) / len(buffer)

    if abs(current_frame - average) > threshold:
        # Nếu chênh lệch lớn, bỏ qua giá trị mới
        return average, False
    
    # Nếu giá trị hợp lệ, thêm vào buffer
    buffer.append(current_frame)
    if len(buffer) > 10:  # Giới hạn buffer_size = 10
        buffer.popleft() 

    return current_frame, True

class LidarThread(threading.Thread):
    def __init__(self, socket):
        super().__init__()
        self.socket = socket
        self.buffer = deque(maxlen=10) 

    def run(self):
        self.calculator_to_radar(self.socket)

    def calculator_to_radar(self, s):
        s.sendall(b'\x02sEN LMDscandata 1\x03')
        while True:
            data = s.recv(4096) 
            if data:
                data_str = data.decode()  
                
                # Sử dụng regex để lọc ra đoạn dữ liệu từ DIST1 đến RSSI1
                pattern = r"(DIST1\s3F800000\s00000000\sDBBA0\s2710\s2[\s0-9A-Fa-f]+)(RSSI1[\s0-9A-Fa-f]+)"
                matches = re.findall(pattern, data_str)

                for match in matches:
                    dist_to_rssi_data = match[0].split("3F800000 00000000 DBBA0 2710 2")[-1].strip()  
                    convert_distance = process_distance_data(dist_to_rssi_data)
                    
                    filtered_distance, is_valid = filter_noise_with_buffer(convert_distance, self.buffer, threshold=500)
                    if is_valid:
                        print(f"Khoảng cách: {filtered_distance} mm")
                    else:
                        print(f"Giá trị bị bỏ qua. Sử dụng giá trị cũ: {filtered_distance} mm")
            return filtered_distance


