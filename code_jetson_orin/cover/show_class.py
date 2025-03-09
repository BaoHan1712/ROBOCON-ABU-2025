from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO("yolo11s.pt")

print(model.names) 
# model.export(format="onnx", dynamic=True, simplify=True)