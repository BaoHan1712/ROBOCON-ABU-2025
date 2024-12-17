import serial
from utils import create_stm32_message
import time

last_send_time = 0 
SEND_DELAY = 0.7


ser = serial.Serial('COM4', 115200, timeout=1) 
while True:
    current_time = time.time()

    if current_time - last_send_time >= SEND_DELAY:
        create_stm32_message(200,900, ser)
        last_send_time = current_time
        
