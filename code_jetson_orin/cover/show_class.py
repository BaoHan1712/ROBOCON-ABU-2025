from ultralytics import YOLO

# Load a pretrained YOLO11n model
model = YOLO(r"rim_robot.pt")

print(model.names) 
# model.export(format="onnx", dynamic=True, simplify=True)