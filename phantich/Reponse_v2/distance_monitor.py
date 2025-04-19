import serial
import serial.tools.list_ports
import pandas as pd
from datetime import datetime
import time

def find_arduino_port():
    """Tìm cổng COM đang kết nối với ESP32"""
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        print(f"Tìm thấy cổng: {port.device}")
    
    if not ports:
        raise Exception("Không tìm thấy cổng COM nào!")
    
    return ports[0].device

class DataLogger:
    def __init__(self):
        # Tìm và kết nối cổng COM
        try:
            port = find_arduino_port()
            print(f"Đang kết nối với {port}...")
            self.ser = serial.Serial(port, 115200, timeout=1)
            time.sleep(2)  # Đợi ESP32 khởi động lại
            print("Đã kết nối thành công!")
        except Exception as e:
            print(f"Lỗi khi kết nối: {str(e)}")
            print("Vui lòng kiểm tra:")
            print("1. ESP32 đã được kết nối chưa?")
            print("2. Đã tắt Serial Monitor trong Arduino IDE chưa?")
            print("3. Không có chương trình nào khác đang sử dụng cổng COM?")
            raise

        # Tạo file Excel để lưu dữ liệu
        self.excel_filename = f'distance_data_{datetime.now().strftime("%Y%m%d_%H%M%S")}.xlsx'
        self.df = pd.DataFrame(columns=['Thời gian', 'Khoảng cách (mm)'])
        print(f"Đã tạo file {self.excel_filename} để lưu dữ liệu")
        print("Đang ghi dữ liệu... Nhấn Ctrl+C để dừng")

    def read_serial_data(self):
        """Đọc dữ liệu từ serial port một cách đơn giản"""
        try:
            # Đọc một dòng hoàn chỉnh với timeout
            line = self.ser.readline().decode('ascii').strip()
            if line.startswith('DIST:'):
                return line
        except:
            # Xóa buffer nếu có lỗi
            self.ser.reset_input_buffer()
        return None

    def run(self):
        save_counter = 0
        error_count = 0
        last_save_time = time.time()

        while True:
            try:
                # Đọc dữ liệu
                line = self.read_serial_data()
                
                if line:
                    try:
                        # Lấy giá trị khoảng cách
                        distance = float(line.split(':')[1].split(',')[0])
                        timestamp = datetime.now()
                        
                        # Thêm vào DataFrame
                        self.df.loc[len(self.df)] = {
                            'Thời gian': timestamp,
                            'Khoảng cách (mm)': distance
                        }
                        
                        # In ra màn hình
                        print(f"Thời gian: {timestamp.strftime('%H:%M:%S')}, Khoảng cách: {distance}mm")
                        
                        # Reset bộ đếm lỗi
                        error_count = 0
                        
                        # Lưu vào Excel sau mỗi 100 mẫu hoặc sau 5 phút
                        save_counter += 1
                        current_time = time.time()
                        if save_counter >= 100 or (current_time - last_save_time) >= 300:
                            self.df.to_excel(self.excel_filename, index=False)
                            save_counter = 0
                            last_save_time = current_time
                            print(f"Đã lưu {len(self.df)} mẫu vào file Excel")
                    
                    except ValueError as e:
                        print(f"Lỗi khi xử lý dữ liệu: {str(e)}")
                        error_count += 1
                
                # Thêm delay nhỏ để tránh CPU quá tải
                time.sleep(0.01)
                
                # Nếu có quá nhiều lỗi liên tiếp
                if error_count > 5:
                    print("Đang reset buffer...")
                    self.ser.reset_input_buffer()
                    error_count = 0
                    time.sleep(0.5)
                
            except KeyboardInterrupt:
                raise
            except Exception as e:
                print(f"Lỗi: {str(e)}")
                # Reset buffer và đợi
                self.ser.reset_input_buffer()
                time.sleep(0.5)

    def close(self):
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()
        # Lưu dữ liệu cuối cùng vào Excel
        if not self.df.empty:
            self.df.to_excel(self.excel_filename, index=False)
            print(f"\nĐã lưu {len(self.df)} mẫu vào {self.excel_filename}")

if __name__ == "__main__":
    logger = None
    try:
        logger = DataLogger()
        logger.run()
    except KeyboardInterrupt:
        print("\nĐang dừng chương trình...")
    except Exception as e:
        print(f"\nLỗi: {str(e)}")
    finally:
        if logger:
            logger.close() 