from data_lidar import LidarThread,  connect_lidar
lidar_socket = connect_lidar()
lidar_thread = LidarThread(lidar_socket)
lidar_thread.start()

# while True:
#     min_distance = lidar_thread.get_min_distance()

min_distance = lidar_thread.get_min_distance()