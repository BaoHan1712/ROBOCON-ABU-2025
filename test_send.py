import serial
import random  
from utils import create_stm32_message_2, create_stm32_message_1 
# Khởi tạo kết nối serial
ser = serial.Serial('COM6', 115200)


while True:
    # Tạo giá trị offset ngẫu nhiên từ 1 đến 200
    random_offset = random.randint(1000, 2000)
    
    # Gọi hàm tạo gói tin và gửi dữ liệu
    # create_stm32_message_1(random_offset, ser)
    create_stm32_message_2(random_offset, ser)
    
    
    
    # Thêm độ trễ giữa các lần gửi dữ liệu
    # time.sleep(1)v  # Gửi dữ liệu mỗi 1 giây
