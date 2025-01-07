from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO("model/best.pt")

# model.export(format="engine",half = True, simplify=True, imgsz=640)

# # # # Run inference on 'bus.jpg' with arguments
model.predict(source="data\clip.mp4", imgsz=640,conf = 0.65, show = True)

