import pyrealsense2 as rs
import numpy as np
import cv2
import time
import os

# Tạo thư mục 'save' nếu chưa tồn tại
save_folder = "save"
if not os.path.exists(save_folder):
    os.makedirs(save_folder)

# Khởi tạo pipeline và cấu hình camera
pipeline = rs.pipeline()
config = rs.config()

# Bật luồng camera độ sâu và màu
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

# Khởi động camera
pipeline.start(config)

chup_anh = False  # Biến để kiểm soát việc chụp ảnh
count = 0  # Đếm số ảnh đã chụp

try:
    while True:
        # Lấy khung hình từ camera
        frames = pipeline.wait_for_frames()
        color_frame = frames.get_color_frame()

        if not color_frame:

            continue

        # Chuyển đổi dữ liệu thành mảng numpy
        color_image = np.asanyarray(color_frame.get_data())

        # Hiển thị hình ảnh
        cv2.imshow("Color Image", color_image)

        key = cv2.waitKey(1) & 0xFF

        if key == ord("c"):  # Nhấn 'c' để bắt đầu chụp ảnh
            chup_anh = True
            print("Bắt đầu chụp ảnh mỗi 1 giây...")

        elif key == ord("q"):  # Nhấn 'q' để thoát
            print("Dừng chụp và thoát...")
            break

        if chup_anh:
            time.sleep(1)  # Chờ 1 giây trước khi chụp ảnh tiếp theo
            count += 1
            filename = os.path.join(save_folder, f"save_img_{count}.png")
            cv2.imwrite(filename, color_image)
            print(f"Ảnh đã được lưu: {filename}")

finally:
    pipeline.stop()
    cv2.destroyAllWindows()
