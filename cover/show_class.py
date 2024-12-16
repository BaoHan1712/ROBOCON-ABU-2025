from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO("model\yolo11n.pt")

print(model.names) 