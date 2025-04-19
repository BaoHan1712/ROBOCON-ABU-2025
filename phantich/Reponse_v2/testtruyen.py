import serial
import time

# Định nghĩa các byte đặc biệt
START_BYTE = 0xAA  # Byte bắt đầu
END_BYTE = 0x55    # Byte kết thúc

class UARTProtocol:
    def __init__(self, port='COM4', baudrate=115200):
        """Khởi tạo kết nối UART"""
        self.serial = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)  # Đợi để kết nối ổn định
        
    def calculate_checksum(self, high_byte, low_byte):
        """Tính checksum bằng cách XOR tất cả các byte"""
        return START_BYTE ^ high_byte ^ low_byte ^ END_BYTE
    
    def send_data(self, value):
        """Gửi dữ liệu theo cấu trúc gói tin an toàn
        Args:
            value (int): Giá trị cần gửi (16-bit)
        """
        # Tách thành byte cao và byte thấp
        high_byte = (value >> 8) & 0xFF
        low_byte = value & 0xFF
        
        # Tính checksum
        checksum = self.calculate_checksum(high_byte, low_byte)
        
        # Đóng gói dữ liệu
        packet = bytearray([
            START_BYTE,
            high_byte,
            low_byte,
            checksum,
            END_BYTE
        ])
        
        # Gửi gói tin
        self.serial.write(packet)
        
    def receive_data(self):
        """Nhận và kiểm tra gói tin
        Returns:
            int: Giá trị nhận được nếu gói tin hợp lệ
            None: Nếu gói tin không hợp lệ
        """
        # Đợi byte bắt đầu
        while True:
            if self.serial.read(1) == bytes([START_BYTE]):
                break
        
        # Đọc phần còn lại của gói tin
        high_byte = int.from_bytes(self.serial.read(1), 'big')
        low_byte = int.from_bytes(self.serial.read(1), 'big')
        checksum = int.from_bytes(self.serial.read(1), 'big')
        end_byte = int.from_bytes(self.serial.read(1), 'big')
        
        # Kiểm tra byte kết thúc
        if end_byte != END_BYTE:
            return None
            
        # Kiểm tra checksum
        if checksum != self.calculate_checksum(high_byte, low_byte):
            return None
            
        # Ghép byte cao và byte thấp
        value = (high_byte << 8) | low_byte
        return value
    
    def close(self):
        """Đóng kết nối UART"""
        if self.serial.is_open:
            self.serial.close()

# Ví dụ sử dụng
if __name__ == "__main__":
    uart = UARTProtocol()
    try:
        # Gửi một số giá trị test
        test_values = [1000, 2000, 3000]
        for value in test_values:
            print(f"Đang gửi: {value}")
            uart.send_data(value)
            time.sleep(0.1)
            
        # Nhận dữ liệu
        print("\nĐang đợi dữ liệu...")
        while True:
            received = uart.receive_data()
            if received is not None:
                print(f"Đã nhận: {received}")
            time.sleep(0.1)
            
    except KeyboardInterrupt:
        print("\nĐang dừng chương trình...")
    finally:
        uart.close()