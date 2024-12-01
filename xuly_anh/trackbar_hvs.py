import cv2
import numpy as np

# Hàm để cập nhật trackbar
def nothing(x):
    pass

# Khởi tạo cửa sổ
cv2.namedWindow('Trackbars')
cv2.createTrackbar('Hue Min', 'Trackbars', 0, 179, nothing)  # Màu cam thường nằm trong khoảng này
cv2.createTrackbar('Hue Max', 'Trackbars', 15, 179, nothing)
cv2.createTrackbar('Sat Min', 'Trackbars', 90, 255, nothing)
cv2.createTrackbar('Sat Max', 'Trackbars', 255, 255, nothing)
cv2.createTrackbar('Val Min', 'Trackbars', 130, 255, nothing)
cv2.createTrackbar('Val Max', 'Trackbars', 255, 255, nothing)


# Khởi tạo camera
cap = cv2.VideoCapture("data\obot.mp4")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Lấy giá trị từ trackbar
    h_min = cv2.getTrackbarPos('Hue Min', 'Trackbars')
    h_max = cv2.getTrackbarPos('Hue Max', 'Trackbars')
    s_min = cv2.getTrackbarPos('Sat Min', 'Trackbars')
    s_max = cv2.getTrackbarPos('Sat Max', 'Trackbars')
    v_min = cv2.getTrackbarPos('Val Min', 'Trackbars')
    v_max = cv2.getTrackbarPos('Val Max', 'Trackbars')

    # Tạo mảng giới hạn cho màu cam
    lower_cam = np.array([h_min, s_min, v_min])
    upper_cam = np.array([h_max, s_max, v_max])

    # Tạo mặt nạ để nhận diện màu cam
    mask = cv2.inRange(hsv, lower_cam, upper_cam)

    # Áp dụng mặt nạ lên khung hình gốc
    result = cv2.bitwise_and(frame, frame, mask=mask)

    cv2.imshow('Original', frame)
    cv2.imshow('Mask', mask)
    cv2.imshow('Result', result)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
