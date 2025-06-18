import math

# Nhập AB = a, AC = c, BC = b, ED = h
a = 35
c =  230
b =  260
h =  35

# Tính tọa độ điểm C
x = (c**2 - b**2 + a**2) / (2 * a)
y_squared = c**2 - x**2

if y_squared < 0:
    print("Lỗi: Tọa độ y không thực. Kiểm tra lại dữ liệu.")
else:
    y = math.sqrt(y_squared)

    # Tính khoảng cách từ C đến D
    CD = math.sqrt((x - a/2)**2 + (y + h)**2)
    print(f"Giá trị của CD là: {CD}")
