<div align="center">
  
# 🤖 ROBOCON ABU 2025
  
<img src="https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=500&size=40&pause=1000&color=2ED573&center=true&vCenter=true&width=435&lines=Computer+Vision;Deep+Learning;Robotics" alt="Typing SVG" />

[![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)](https://www.python.org/)
[![OpenCV](https://img.shields.io/badge/OpenCV-27338e?style=for-the-badge&logo=OpenCV&logoColor=white)](https://opencv.org/)
[![PyTorch](https://img.shields.io/badge/PyTorch-EE4C2C?style=for-the-badge&logo=pytorch&logoColor=white)](https://pytorch.org/)
[![TensorRT](https://img.shields.io/badge/TensorRT-76B900?style=for-the-badge&logo=nvidia&logoColor=white)](https://developer.nvidia.com/tensorrt)

</div>

<div align="center">
  <h3>
    <a href="#overview">Overview</a>
    <span> | </span>
    <a href="#features">Features</a>
    <span> | </span>
    <a href="#technical-details">Technical Details</a>
    <span> | </span>
    <a href="#installation">Installation</a>
  </h3>
</div>

<div id="overview">

## 🎯 TỔNG QUAN

Hệ thống thị giác máy tính cho robot bóng rổ, sử dụng deep learning và xử lý ảnh để:
- Nhận diện và tracking vành rổ 
- Tính toán độ lệch góc và khoảng cách
- Điều khiển robot thông qua giao tiếp UART

<p align="center">
  <img src="system_overview.png" width="70%"/>
</p>

### Mục tiêu của dự án:
- Tạo ra một hệ thống tự động giúp robot có thể nhận diện và điều chỉnh vị trí để ném bóng vào rổ một cách chính xác.
- Tối ưu hóa quy trình xử lý hình ảnh và dữ liệu từ cảm biến để đảm bảo độ chính xác và tốc độ cao.

</div>

<div id="features">

## 🚀 CHỨC NĂNG

1. **Nhận diện đối tượng:**
   - Sử dụng mô hình CNN để nhận diện bóng rổ, bảng rổ và vành rổ.
   - Độ tin cậy (confidence) > 50% mới được xử lý.
   - Xử lý được các trường hợp nhiễu như: che khuất một phần, nhiều đối tượng trong cùng frame.

2. **Tracking đối tượng:**
   - Sử dụng thuật toán SORT (Simple Online Realtime Tracking).
   - Duy trì ID cho đối tượng kể cả khi bị che khuất tạm thời.
   - Chống nhiễu khi có nhiều đối tượng tương tự xuất hiện.

3. **Tính toán độ lệch:**
   - Tính độ lệch giữa tâm đối tượng và tâm khung hình.
   - Map độ lệch sang thang đo phù hợp (1-99: lệch trái, 100: chuẩn, 101-254: lệch phải).
   - Hiển thị thông tin độ lệch trực quan trên frame.

4. **Đo khoảng cách:**
   - Sử dụng cảm biến Lidar để đo khoảng cách chính xác.
   - Lọc nhiễu bằng buffer và ngưỡng chênh lệch.
   - Độ chính xác đến mm.

5. **Truyền dữ liệu:**
   - Giao tiếp với STM32 qua UART.
   - Gói tin có cấu trúc: Start byte (0x02) + Offset (1 byte) + Distance (2 bytes) + Checksum + End byte (0x03).
   - Tần suất gửi: 0.5s/lần.

</div>

<div id="technical-details">

## 📋 CHI TIẾT KỸ THUẬT

### 1. Thuật toán tính tâm và độ lệch:
- **Xác định tâm đối tượng:** Sử dụng bounding box (x1,y1,x2,y2) từ YOLO để tính tọa độ tâm.
- **Tính độ lệch:** Độ lệch = cx - frame_center_x, với cx là tọa độ x của tâm đối tượng.
- **Bộ lọc nhiễu Kalman:** Sử dụng trong SORT tracking để duy trì độ chính xác trong việc theo dõi đối tượng.

### 2. Cấu trúc gói tin truyền xuống STM32:

a) Header (1 byte):
- Start byte: 0x02
- Đánh dấu bắt đầu gói tin mới

b) Payload (3 bytes):
- Byte 1: Độ lệch (offset)
  + [1-99]: Lệch trái
  + 100: Chuẩn tâm  
  + [101-254]: Lệch phải
- Byte 2-3: Khoảng cách (16-bit)
  + LSB first
  + Đơn vị: mm
  + Phạm vi: 0-65535mm

c) Checksum (1 byte):
- Tổng modulo 256 của payload
- Kiểm tra tính toàn vẹn dữ liệu

d) Footer (1 byte): 
- End byte: 0x03
- Đánh dấu kết thúc gói tin

Ví dụ gói tin:
```
0x02 | 0x64 0x00 0xFA | 0x5E | 0x03
STX  | Offset Dist    | CSum | ETX
```

### 3. Xử lý tín hiệu Lidar:

a) Thu thập dữ liệu:
- Tốc độ quét: 15Hz
- Góc quét: 180°
- 1081 điểm đo/vòng quét

b) Lọc nhiễu:
- Buffer size: 10 mẫu
- Lọc trung vị trong cửa sổ trượt
- Loại bỏ giá trị ngoại lai (>3σ)

c) Tính khoảng cách:
- Trung bình cộng sau lọc
- Độ chính xác: ±2mm
- Tần suất cập nhật: 0.5s

<h3>4. Tối ưu hiệu năng:</h3>

a) YOLO inference:
- Batch size: 1
- Input size: 640x640
- TensorRT engine
- FP16 precision

b) Tracking:
- Max age: 40 frames
- Min hits: 3 frames
- IOU threshold: 0.3

c) Xử lý song song:
- Thread 1: Camera + YOLO
- Thread 2: Lidar
- Thread 3: UART transmission

<h3>5. Yêu cầu hệ thống:</h3>

a) Độ trễ hệ thống:
- Camera to detection: 33ms
- Lidar to distance: 66ms  
- Total latency: <100ms

b) Độ chính xác:
- Object detection: >95% mAP
- Tracking: >90% MOTA
- Distance: ±2mm
- Offset: ±1°

</div>

<div id="performance">

## ⚡ HIỆU NĂNG

<table>
<tr>
<td>

### 🎯 Độ chính xác

- Object Detection: 95.2% mAP
- Tracking: 90.5% MOTA  
- Distance: ±2mm
- Offset: ±1°

</td>
<td>

### ⏱️ Độ trễ

- Camera → Detection: 33ms
- Lidar → Distance: 66ms
- Total latency: <100ms

</td>
</tr>
</table>

</div>

<div id="demo">


## 👥 CONTRIBUTORS

<a href="https://github.com/BaoHan1712">
  <img src="https://github.com/username.png" width="50px" alt=""/>
</a>

</div>

<div id="license">

## 📝 LICENSE

MIT License - Copyright (c) 2024 [BaoHan1712]

</div>

<div id="flowchart">

## 📊 FLOWCHART CỦA HỆ THỐNG

```mermaid
flowchart TD
    A[Camera Input] --> B[Frame Preprocessing]
    B --> C[YOLO Detection]
    C --> D[Confidence Filtering]
    D --> E[SORT Tracking]
    
    F[Lidar Input] --> G[Distance Measurement]
    G --> H[Noise Filtering]
    
    E --> I[Offset Calculation]
    H --> I
    I --> J[Data Packaging]
    J --> K[UART Transmission]
    K --> L[STM32 Control]
    
    M[Display Output] --> N[Show Frame]
    I --> M
    H --> M
```

### Mô tả flowchart:
1. **Camera Input:** Nhận dữ liệu từ camera.
2. **Frame Preprocessing:** Tiền xử lý khung hình để chuẩn bị cho việc nhận diện.
3. **YOLO Detection:** Sử dụng mô hình YOLO để phát hiện đối tượng trong khung hình.
4. **Confidence Filtering:** Lọc các đối tượng dựa trên độ tin cậy.
5. **SORT Tracking:** Theo dõi các đối tượng đã phát hiện.
6. **Lidar Input:** Nhận dữ liệu từ cảm biến Lidar.
7. **Distance Measurement:** Đo khoảng cách từ cảm biến.
8. **Noise Filtering:** Lọc nhiễu từ dữ liệu đo.
9. **Offset Calculation:** Tính toán độ lệch giữa tâm đối tượng và tâm khung hình.
10. **Data Packaging:** Đóng gói dữ liệu để gửi xuống STM32.
11. **UART Transmission:** Gửi dữ liệu qua giao tiếp UART.
12. **STM32 Control:** Điều khiển robot dựa trên dữ liệu nhận được.
13. **Display Output:** Hiển thị kết quả trên màn hình.

</div>

<div align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=gradient&height=100&section=footer"/>
</div>



