import socket
import re
import threading
from collections import deque

# Cấu hình kết nối radar
HOST = "169.254.222.142" 
PORT = 2112

def hex_to_decimal(hex_value):
    """Chuyển đổi giá trị hex sang decimal"""
    return int(hex_value, 16)

def convert_to_meters(value, scale_factor=0.001):
    """Chuyển đổi giá trị decimal sang khoảng cách (mét)"""
    return value * scale_factor

def connect_lidar():
    """Khởi tạo kết nối tới radar"""
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST, PORT))
        print("Kết nối thành công!")
        return s
    except Exception as e:
        print(f"Lỗi kết nối: {e}")
        return None

def process_distances(dist_values):
    """Xử lý và tính toán khoảng cách từ dữ liệu radar"""
    try:
        distances = [convert_to_meters(hex_to_decimal(val)) for val in dist_values]
        min_dist = min(distances)
        return round((min_dist + 0.245) * 1000, 2)
    except Exception as e:
        print(f"Lỗi xử lý khoảng cách: {e}")
        return 0

class LidarThread(threading.Thread):
    def __init__(self, socket):
        super().__init__()
        self.socket = socket
        self.buffer = deque(maxlen=10)
        self.min_distance = None
        self._running = True

    def run(self):
        """Khởi chạy thread xử lý dữ liệu radar"""
        self.calculator_to_radar(self.socket)

    def stop(self):
        """Dừng thread an toàn"""
        self._running = False

    def calculator_to_radar(self, s):
        """Xử lý dữ liệu từ radar"""
        # Gửi lệnh bắt đầu quét
        s.sendall(b'\x02sEN LMDscandata 1\x03')

        # Pattern để tách dữ liệu khoảng cách
        pattern = r"DIST1\s3F800000\s00000000\sE09C0\s2710\s9([\s0-9A-Fa-f]+)RSSI1"

        while self._running:
            try:
                data = s.recv(4096)
                if not data:
                    break

                data_str = data.decode()
                match = re.search(pattern, data_str)

                if match:
                    dist_values = match.group(1).strip().split()
                    distance = process_distances(dist_values)
                    print(f"Khoảng cách: {distance} mm")
                    self.min_distance = distance
                    self.buffer.append(distance)

            except Exception as e:
                print(f"Lỗi xử lý dữ liệu: {e}")
                break

    def get_min_distance(self):
        """Lấy khoảng cách nhỏ nhất hiện tại"""
        return self.min_distance if self.min_distance is not None else 0