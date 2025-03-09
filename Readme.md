# HỆ THỐNG NHẬN DIỆN VÀ ĐIỀU KHIỂN ROBOT ROBOCON 2025

## Mô tả hệ thống

Hệ thống bao gồm các thành phần chính:

1. Camera (Intel RealSense D435 hoặc USB Camera)
2. LIDAR (Tùy chọn)
3. Jetson Orin
4. Raspberry Pi 5 AI KIT
5. STM32 
6. Động cơ điều khiển robot

## Phần mềm

1. Thuật toán xử lý ảnh:

- YOLOv11 tối ưu cho Jetson
- 2 class: basket & backboard
- Confidence threshold: 50-75%
- FPS trung bình: 30-45
- ByteTrack tracking
- Max age: 30-90 frames
- Min hits: 3
- IOU threshold: 0.3

2. Xử lý thông tin:
- Depth camera
- Bộ lọc trung bình động (α=0.1)
- Giới hạn khoảng cách: 0.2-6m
- Tính toán độ lệch
- Tâm: 100 ±2
- Lệch trái: 1-98
- Lệch phải: 102-254

### Luồng xử lý

%%{init: { 'theme': 'base', 'themeVariables': { 'primaryColor': '#32CD32', 'edgeLabelBackground':'#fff' } } }%%
flowchart TD
    subgraph Sensors ["Cảm biến"]
        A[Intel RealSense D435] --> |30-60 FPS| B[Frame ảnh]
        C[LIDAR] --> |5-15Hz| D[Khoảng cách]
        style A fill:#90EE90
        style B fill:#98FB98
        style C fill:#90EE90 
        style D fill:#98FB98
    end

    subgraph Processing ["Xử lý & Phân tích"]
        B --> E[YOLOv11]
        E --> F[Object Detection]
        F --> G[ByteTrack]
        G --> H{Phân loại}
        H --> |Class 1| I[Basket]
        H --> |Class 2| J[Backboard]
        
        style E fill:#87CEEB
        style F fill:#87CEEB
        style G fill:#DDA0DD
        style H fill:#FFB6C1
        style I fill:#98FB98
        style J fill:#98FB98
    end

    subgraph Analysis ["Phân tích dữ liệu"]
        I & J --> K[Bounding Box]
        K --> L[Color Detection]
        K --> M[Depth Analysis]
        D --> M
        L & M --> N[Object Info]
        
        style K fill:#FFE4B5
        style L fill:#F0E68C
        style M fill:#F0E68C
        style N fill:#DEB887
    end

    subgraph Control ["Điều khiển Robot"]
        N --> O[Tính độ lệch]
        O --> P[PID Controller]
        P --> Q[STM32F407]
        Q --> R[4 DC Servo]
        Q --> S[2 Xi lanh khí nén]
        Q --> T[LED Status]
        
        style O fill:#FFA07A
        style P fill:#FFA07A
        style Q fill:#CD853F
        style R fill:#CD853F
        style S fill:#CD853F
        style T fill:#CD853F
    end

    %% Kết nối giữa các subgraph
    classDef default fill:#fff,stroke:#333,stroke-width:2px;
    classDef subgraph fill:#fff,stroke:#333,stroke-width:2px;
    class Sensors,Processing,Analysis,Control subgraph;

    %% Animation
    linkStyle default stroke-width:2px,fill:none,stroke-dasharray: 5 5;

### Quy trình hoạt động

1. **Thu nhận hình ảnh**
   - Camera thu nhận frame ảnh liên tục
   - Tốc độ xử lý: 30-60 FPS tùy phần cứng

2. **Nhận diện vật thể** 
   - Sử dụng mô hình YOLO được tối ưu
   - Phát hiện 2 class: basket (rổ) và backboard (bảng rổ)
   - Tracking ID để theo dõi vật thể qua thời gian

3. **Xử lý độ lệch**
   - Tính toán độ lệch của vật thể so với tâm khung hình
   - Map độ lệch thành các giá trị:
     * 1-99: Lệch trái
     * 100: Chuẩn tâm  
     * 101-254: Lệch phải

4. **Điều khiển hướng tự động né khi tấn công**
   - Dựa vào độ lệch để quyết định hướng di chuyển:
     * 1: Đi thẳng (độ lệch 98-102)
     * 2: Rẽ trái (độ lệch > 102)
     * 3: Rẽ phải (độ lệch < 98)
     * 4: Không có vật

5. **Giao tiếp với STM32**
   - Gửi gói tin chứa:
     * Offset (1 byte)
     * Distance (2 bytes)
     * Position (2 bytes có dấu)
   - Tốc độ baudrate: 115200

### Ưu điểm

- Hệ thống hoạt động ổn định trong nhiều điều kiện ánh sáng
- Tracking ID giúp theo dõi vật thể liên tục
- Tích hợp được nhiều sensor (Camera + LIDAR + Depth Camera + STM32)
- Xử lý được các trường hợp nhiễu và che khuất
- Ưu tiên xử lý ảnh real-time với FPS cao
- Tracking ổn định qua nhiều frame
- Tích hợp nhiều cảm biến dự phòng
- PID controller cho chuyển động mượt
- Tự động né tránh chướng ngại vật
- LED báo trạng thái trực quan

### Nhược điểm

- Yêu cầu phần cứng mạnh để đạt FPS cao

## Cài đặt và sử dụng

1. Cài đặt các thư viện:
```python
pip install ultralytics opencv-python pyrealsense2 numpy
```

2. Kết nối phần cứng:
- Camera qua cổng USB/CSI
- STM32 qua UART (ttyUSB0)
- LIDAR (nếu có)
- Depth Camera (nếu có)

3. Chạy chương trình:
```bash
python depth_camera.py
```

## Tham khảo

- Mã nguồn: [BaoHan1712](https://github.com/BaoHan1712)
- Tài liệu YOLO: [Ultralytics Docs](https://docs.ultralytics.com/)
- Intel RealSense: [D435 Documentation](https://github.com/IntelRealSense/realsense-ros)


