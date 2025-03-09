from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO("orange.pt")

print(model.names) 