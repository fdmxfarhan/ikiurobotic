import cv2
import requests
# from ultralytics import YOLO
# from ultralytics.models.yolo.detect.predict import DetectionPredictor

# Distance constants
KNOWN_DISTANCE = 40.0 #Cm
MOBILE_WIDTH = 8.0 #Cm

# Object detector constant
CONFIDENCE_THRESHOLD = 0.4
NMS_THRESHOLD = 0.3

# colors for object detection and texts
COLORS = [(255,0,0),(255,0,255),(0, 255, 255), (255, 255, 0), (0, 255, 0), (255, 0, 0)]
RED = (0,0,255)
GREEN = (0,255,0)
BLACK =(0,0,0)
FONTS = cv2.FONT_HERSHEY_COMPLEX

# getting class names from classes.txt file
with open("classes.txt", "r") as f:
    class_names = [cname.strip() for cname in f.readlines()]

# setting up opencv net with YOLOv4
yoloNet = cv2.dnn.readNet('yolov4-tiny.weights', 'yolov4-tiny.cfg')
yoloNet.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
yoloNet.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA_FP16)
model = cv2.dnn_DetectionModel(yoloNet)
model.setInputParams(size=(416, 416), scale=1/255, swapRB=True)

# object detector function
def object_detector(image):
    classes, scores, boxes = model.detect(image, CONFIDENCE_THRESHOLD, NMS_THRESHOLD)
    data_list = []
    for (classid, score, box) in zip(classes, scores, boxes):
        # define color of each, object based on its class id
        color = COLORS[int(classid) % len(COLORS)]
        label = "%s : %f" % (class_names[classid], score)

        # draw rectangle and label on object
        cv2.rectangle(image, box, color, 1)
        cv2.putText(image, label, (box[0], box[1] - 14), FONTS, 0.5, color, 2)

        # adding class id
        # 1: class name  2: object width in pixels  3: position where to draw distance
        if classid == 67: # phone class id
            data_list.append([class_names[classid], box[2], (box[0], box[1] - 2)])

    return data_list

# focal length finder function
def focal_length_finder (measured_distance, real_width, width_in_rf):
    focal_length = (width_in_rf * measured_distance) / real_width
    return focal_length

# distance finder function
def distance_finder(focal_length, real_object_width, width_in_frmae):
    distance = (real_object_width * focal_length) / width_in_frmae
    return distance

# reading the reference image and finging focal length
ref_mobile = cv2.imread('RefImages/image3.png')
mobile_data = object_detector(ref_mobile)
mobile_width_in_rf = mobile_data[0][1]
print(f"mobile width in pixel: {mobile_width_in_rf}")
focal_mobile = focal_length_finder(KNOWN_DISTANCE, MOBILE_WIDTH, mobile_width_in_rf)

# Connection to Ip webcam and
url_cap = "http://192.168.62.215:8080"
frame_cap = cv2.VideoCapture(url_cap + "/video")
# frame_cap = cv2.VideoCapture(0)
while True:
    ret, frame = frame_cap.read()
    data = object_detector(frame)
    for d in data:
        if d[0] == 'cell phone':
            distance = distance_finder(focal_mobile, MOBILE_WIDTH, d[1])
            x, y = d[2]

        cv2.rectangle(frame, (x, y - 3), (x + 150, y + 23), BLACK, -1)
        cv2.putText(frame, f'Dis: {round(distance, 2)} Cm', (x + 5, y + 13), FONTS, 0.38, RED, 1)

    cv2.imshow("frame", frame)

    if cv2.waitKey(1) == ord('q'):
        break
        
cv2.destroyAllWindows()
frame_cap.release()
