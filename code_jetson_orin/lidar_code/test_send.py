from data_lidar import LidarThread,  connect_lidar
import serial
from cover.utils import *


lidar_socket = connect_lidar()
lidar_thread = LidarThread(lidar_socket)
lidar_thread.start()
ser = serial.Serial('/dev/ttyUSB0', 115200)

while True:
    min_distance = lidar_thread.get_min_distance()
    create_stm32_message_1(10, min_distance, ser)
