from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO("rim_robot.pt")

# model.export(format="onnx",half = True, simplify=True, imgsz=640)

# # # # Run inference on 'bus.jpg' with arguments
model.predict(source=r"E:\AI_folder\xu_ly_anh\data\h2.mp4", imgsz=640,conf = 0.65, show = True)

