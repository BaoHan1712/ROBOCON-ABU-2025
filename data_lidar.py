import socket
import re
import threading
from collections import deque

# Cấu hình kết nối radar
HOST = "169.254.222.142" 
PORT = 2112

# Giá trị khoảng cách mặc định ban đầu
DEFAULT_DISTANCE = 6000

# Ngưỡng kiểm tra dữ liệu mới
THRESHOLD = 2600

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
        # Chuyển đổi giá trị hex sang mét
        distances = [convert_to_meters(hex_to_decimal(val)) for val in dist_values]
        
        # Lọc các giá trị nhỏ hơn 8.0 (8000 mm)
        valid_distances = [dist for dist in distances if dist < 8.0]
        
        if not valid_distances:
            raise ValueError("Không có khoảng cách hợp lệ nhỏ hơn 8000 mm.")
        
        # Lấy giá trị lớn nhất trong các khoảng cách hợp lệ
        min_dist = max(valid_distances)
        
        # Tính toán và trả về khoảng cách sau khi đổi sang mm
        return round((min_dist - 0.325) * 1000, 2)  # Đổi sang mm
    except Exception as e:
        print(f"Lỗi xử lý khoảng cách: {e}")
        return DEFAULT_DISTANCE


class LidarThread(threading.Thread):
    def __init__(self, socket):
        super().__init__()
        self.socket = socket
        self.buffer = deque(maxlen=10)
        self.current_distance = DEFAULT_DISTANCE
        self.previous_distance = DEFAULT_DISTANCE
        self.last_3_values = deque(maxlen=3)
        self._running = True

    def run(self):
        """Khởi chạy thread xử lý dữ liệu radar"""
        self.calculator_to_radar(self.socket)

    def stop(self):
        """Dừng thread an toàn"""
        self._running = False

    def calculate_moving_average(self):
        """Tính trung bình cộng của 3 giá trị gần nhất"""
        if len(self.last_3_values) == 3:
            return sum(self.last_3_values) / 3
        return self.current_distance

    def calculator_to_radar(self, s):
        """Xử lý dữ liệu từ radar"""
        # Gửi lệnh bắt đầu quét
        s.sendall(b'\x02sEN LMDscandata 1\x03')

        # Pattern để tách dữ liệu khoảng cách
        pattern = r"DIST1\s3F800000\s00000000\sF9060\s2710\s3([\s0-9A-Fa-f]+)RSSI1"

        while self._running:
            try:
                data = s.recv(4096)
                if not data:
                    break

                data_str = data.decode()
                match = re.search(pattern, data_str)

                if match:
                    dist_values = match.group(1).strip().split()
                    new_distance = process_distances(dist_values)

                    # So sánh giá trị mới và cũ
                    if abs(new_distance - self.previous_distance) > THRESHOLD:
                        print(f"khác biệt quá lớn ({new_distance} mm), giữ giá trị cũ: {self.previous_distance} mm")
                    else:
                        self.previous_distance = self.current_distance
                        self.current_distance = new_distance
                        self.last_3_values.append(new_distance)
                        
                        # Tính và in trung bình cộng
                        moving_avg = self.calculate_moving_average()
                        print(f"Cập nhật khoảng cách: {self.current_distance} mm")
                        print(f"Trung bình cộng 3 giá trị gần nhất: {moving_avg:.2f} mm")

                    self.buffer.append(self.current_distance)

            except Exception as e:
                print(f"Lỗi xử lý dữ liệu: {e}")
                break

    def get_min_distance(self):
        """Lấy khoảng cách hiện tại"""
        return self.current_distance
