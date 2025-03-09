import pyrealsense2 as rs

# Khởi tạo pipeline
pipeline = rs.pipeline()
config = rs.config()

# Lấy thông tin của tất cả các thiết bị được kết nối
ctx = rs.context()
devices = ctx.query_devices()

if len(devices) == 0:
    print("Không tìm thấy thiết bị RealSense nào được kết nối!")
    exit()

# Lấy thiết bị đầu tiên
device = devices[0]

# In thông tin cơ bản của thiết bị
print("\nThông tin thiết bị:")
print(f"Tên: {device.get_info(rs.camera_info.name)}")
print(f"Serial number: {device.get_info(rs.camera_info.serial_number)}")
print(f"Firmware version: {device.get_info(rs.camera_info.firmware_version)}")

# Lấy danh sách sensors
sensors = device.query_sensors()

print("\nDanh sách cấu hình được hỗ trợ:")
for sensor in sensors:
    if sensor.is_depth_sensor():
        sensor.set_option(rs.option.enable_auto_exposure, 0)  # Tắt auto-exposure

    print(f"\nSensor {sensor.get_info(rs.camera_info.name)}:")
    
    # Lấy danh sách các profiles được hỗ trợ
    profiles = sensor.get_stream_profiles()
    
    for profile in profiles:
        # Lấy thông tin về video stream
        video_profile = profile.as_video_stream_profile()
        format = profile.format()
        fps = video_profile.fps()
        width = video_profile.width()
        height = video_profile.height()
        
        # Xác định loại stream
        stream_type = "Màu" if profile.stream_type() == rs.stream.color else "Depth" if profile.stream_type() == rs.stream.depth else "Khác"
        
        print(f"- {stream_type}: {width}x{height} @ {fps}fps - Format: {format}")

try:
    # Thử kết nối với cấu hình mặc định
    pipeline_profile = pipeline.start(config)
    print("\nĐã kết nối thành công với cấu hình mặc định!")
    
    # In thông tin về cấu hình đang hoạt động
    active_profile = pipeline_profile.get_stream(rs.stream.depth).as_video_stream_profile()
    print(f"\nCấu hình depth đang sử dụng: {active_profile.width()}x{active_profile.height()} @ {active_profile.fps()}fps")
    
    active_profile = pipeline_profile.get_stream(rs.stream.color).as_video_stream_profile()
    print(f"Cấu hình màu đang sử dụng: {active_profile.width()}x{active_profile.height()} @ {active_profile.fps()}fps")

except Exception as e:
    print(f"Lỗi khi khởi động camera: {e}")
    exit()