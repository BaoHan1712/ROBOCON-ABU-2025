import socket
import re
import threading

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

# Hàm tách và chuyển đổi dữ liệu khoảng cách, với độ chính xác cao hơn
def process_distance_data(dist_to_rssi_data):
    dist_values = dist_to_rssi_data.split()
    distances_in_meters = [convert_to_meters(hex_to_decimal(val), scale_factor=0.001, offset=0.0) for val in dist_values]
    
    # Kiểm tra nếu không có giá trị nào hợp lệ
    if not distances_in_meters:
        return 0.0  # Trả về 0 nếu không có giá trị nào

    average_distance = sum(distances_in_meters) / len(distances_in_meters)
    convert_distance = round(average_distance * 1000, 2)  # Tính toán và làm tròn đến 2 chữ số thập phân
    return convert_distance

# Hàm chống nhiễu dữ liệu, với độ chính xác cao hơn
def filter_noise(current_frame, previous_frame):
    if previous_frame is None:
        previous_frame = 0.0  
    if abs(current_frame - previous_frame) > 9000:
        return previous_frame, False
    return current_frame, True

class LidarThread(threading.Thread):
    def __init__(self, socket):
        super().__init__()
        self.socket = socket
        self.previous_frame = None

    def run(self):
        self.calculator_to_radar(self.socket)

    def calculator_to_radar(self, s):
        s.sendall(b'\x02sEN LMDscandata 1\x03')
        while True:
            data = s.recv(4096)  # Nhận data từ radar
            if data:
                data_str = data.decode()  # Convert data thành chuỗi
                
                # Sử dụng regex để lọc ra đoạn dữ liệu từ DIST1 đến RSSI1
                pattern = r"(DIST1\s3F800000\s00000000\sDBBA0\s2710\s2[\s0-9A-Fa-f]+)(RSSI1[\s0-9A-Fa-f]+)"
                matches = re.findall(pattern, data_str)

                for match in matches:
                    dist_to_rssi_data = match[0].split("3F800000 00000000 DBBA0 2710 2")[-1].strip()  
                    convert_distance = process_distance_data(dist_to_rssi_data)
                    # Áp dụng hàm chống nhiễu
                    filtered_distance, is_valid = filter_noise(convert_distance, self.previous_frame)
                    if is_valid:
                        print(f"Khoảng cách: {filtered_distance} mm")
                        self.previous_frame = filtered_distance  # Cập nhật giá trị frame trước đó

# Sử dụng trong mã chính
