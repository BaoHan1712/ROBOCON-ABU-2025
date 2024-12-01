from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO("model\cnn2.onnx")

print(model.names) 