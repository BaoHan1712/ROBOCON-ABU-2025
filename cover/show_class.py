from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO("best.pt")

print(model.names) 