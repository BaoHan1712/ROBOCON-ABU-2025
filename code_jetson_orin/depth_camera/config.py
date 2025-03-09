import pyrealsense2 as rs

# Tạo đối tượng context để lấy danh sách thiết bị
context = rs.context()
devices = context.query_devices()

if len(devices) == 0:
    print("No Intel RealSense device detected!")
    exit()

for device in devices:
    print(f"\n📌 Device: {device.get_info(rs.camera_info.name)}")
    
    # Liệt kê các cảm biến (depth, color, infrared, etc.)
    for sensor in device.sensors:
        print(f"\n🔹 Sensor: {sensor.get_info(rs.camera_info.name)}")
        
        # Liệt kê các stream profiles mà cảm biến hỗ trợ
        for profile in sensor.get_stream_profiles():
            stream_type = profile.stream_type()
            format_type = profile.format()
            fps = profile.fps()
            
            if profile.is_video_stream_profile():
                video_profile = profile.as_video_stream_profile()
                width, height = video_profile.width(), video_profile.height()
                print(f"   ✅ {stream_type} - {width}x{height} @ {fps} FPS [{format_type}]")
            else:
                print(f"   ✅ {stream_type} @ {fps} FPS [{format_type}]")
