import serial
import re
import threading
import queue
import time
from cover.utils import *


# Queue để chia sẻ dữ liệu giữa 2 luồng
distance_queue = queue.Queue()

# Khởi tạo 2 cổng Serial
ser_esp32 = serial.Serial('COM4', 115200, timeout=1)  # Cổng nhận từ ESP32
ser_stm32 = serial.Serial('COM8', 115200)  # Cổng gửi xuống STM32


def receive_thread():
    """Luồng nhận dữ liệu từ ESP32"""
    print("⏳ Đang chờ dữ liệu từ ESP32...\n")
    while True:
        try:
            line = ser_esp32.readline()
            if line:
                line = line.strip().decode('utf-8')
                match = re.search(r'\d+', line)
                
                if match:
                    distance = int(match.group(0))
                    print(f"✅ Khoảng cách nhận được: {distance} mm")
                    # Đưa distance vào queue để luồng gửi xử lý
                    distance_queue.put(distance)
                else:
                    print(f"❌ Dữ liệu không hợp lệ: {line}")
                    
        except Exception as e:
            print(f"Lỗi ở luồng nhận: {e}")
            time.sleep(0.1)

def transmit_thread():
    """Luồng gửi dữ liệu xuống STM32"""
    print("⏳ Bắt đầu gửi dữ liệu xuống STM32...\n")
    last_sent = 0
    
    while True:
        try:
            # Lấy distance từ queue với timeout
            try:
                distance = distance_queue.get(timeout=0.1)  
                
                # Tránh gửi lại giá trị cũ
                if distance != last_sent:
                    # Gửi gói tin
                    create_stm32_message_1(1, distance, ser_stm32)
                    print(f"📤 Đã gửi xuống STM32: {distance} mm")
                    last_sent = distance
                
            except queue.Empty:
                # Timeout khi đợi dữ liệu mới
                continue
            
        except Exception as e:
            print(f"Lỗi ở luồng gửi: {e}")


def main():
    print("🚀 Khởi động chương trình...")
    print(f"COM ESP32: {ser_esp32.port}, COM STM32: {ser_stm32.port}")
    
    # Tạo và khởi động các luồng
    receiver = threading.Thread(target=receive_thread)
    transmitter = threading.Thread(target=transmit_thread)
    
    # Đặt các luồng là daemon
    receiver.daemon = True
    transmitter.daemon = True
    
    # Khởi động các luồng
    receiver.start()
    transmitter.start()
    
    try:
        # Giữ chương trình chạy
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n🛑 Đang dừng chương trình...")
    finally:
        # Đóng các cổng Serial
        ser_esp32.close()
        ser_stm32.close()
        print("📍 Đã đóng các cổng Serial")

if __name__ == "__main__":
    main()