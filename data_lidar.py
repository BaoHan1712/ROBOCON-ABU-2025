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

# Hàm tính khoảng cách (milimét) 
def convert_to_millimeters(value, scale_factor=1.0, offset=0.0):
    return (value * scale_factor) + offset

# Hàm kết nối tới lidar
def connect_lidar():
    """Kết nối tới radar và trả về socket đã kết nối."""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    print("Kết nối thành công!")
    return s

class LidarThread(threading.Thread):
    def __init__(self, socket):
        super().__init__()
        self.socket = socket
        self.buffer = deque(maxlen=10)
        self.min_distance = None  # Biến để lưu khoảng cách nhỏ nhất

    def run(self):
        self.calculator_to_radar(self.socket)

    def calculator_to_radar(self, s):
        s.sendall(b'\x02sEN LMDscandata 1\x03')

        while True:
            try:
                data = s.recv(4096)  # Nhận dữ liệu radar
                if data:
                    data_str = data.decode()  # Convert dữ liệu thành chuỗi 
                    
                    pattern = r"(DIST1\s3F800000\s00000000\sE7EF0\s2710\s9[\s0-9A-Fa-f]+)(RSSI1[\s0-9A-Fa-f]+)"
                    matches = re.findall(pattern, data_str)

                    for match in matches:
                        # Bỏ DIST1 và RSSI1
                        dist_to_rssi_data = match[0].split("3F800000 00000000 E7EF0 2710 9")[-1].strip()  
                        
                        # Tách các giá trị theo khoảng trắng
                        dist_values = dist_to_rssi_data.split()

                        # Convert giá trị hex -> decimal và sau đó tính khoảng cách
                        distances = [convert_to_millimeters(hex_to_decimal(val)) for val in dist_values]

                        self.min_distance = min(distances) 
                        print(f"😎😎Khoảng cách: {self.min_distance} mm")
            except Exception as e:
                print(f"Đã xảy ra lỗi: {e}")
                break 
    def get_min_distance(self):
        """Trả về khoảng cách nhỏ nhất đã được tính toán."""
        return self.min_distance if self.min_distance is not None else 0  # Hoặc giá trị mặc định khác