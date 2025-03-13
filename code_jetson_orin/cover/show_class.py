from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO(r"model\2clss_yolov8_ver1.pt")

print(model.names) 
# model.export(format="onnx", dynamic=True, simplify=True)