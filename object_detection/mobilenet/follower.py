import cv2
import numpy as np 

def nothing(x):
    pass
 
neural_network = cv2.dnn.readNetFromCaffe('MobileNetSSD_deploy.prototxt.txt', 
        'MobileNetSSD_deploy.caffemodel')
 
categories = { 0: 'background', 1: 'aeroplane', 2: 'bicycle', 3: 'bird', 
               4: 'boat', 5: 'bottle', 6: 'bus', 7: 'car', 8: 'cat', 
               9: 'chair', 10: 'cow', 11: 'diningtable', 12: 'dog', 
              13: 'horse', 14: 'motorbike', 15: 'person', 
              16: 'pottedplant', 17: 'sheep', 18: 'sofa', 
              19: 'train', 20: 'tvmonitor'}
 
classes =  ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", 
            "bus", "car", "cat", "chair", "cow", 
           "diningtable",  "dog", "horse", "motorbike", "person", 
           "pottedplant", "sheep", "sofa", "train", "tvmonitor"]
                      
bbox_colors = np.random.uniform(255, 0, size=(len(categories), 3))

hL = 0;
hH = 39;
sL = 155; 
sH = 219;
vL = 91;
vH = 255;

cv2.namedWindow('marking')
cv2.createTrackbar('H Lower','marking',hL,255,nothing)
cv2.createTrackbar('H Higher','marking',hH,255,nothing)
cv2.createTrackbar('S Lower','marking',sL,255,nothing)
cv2.createTrackbar('S Higher','marking',sH,255,nothing)
cv2.createTrackbar('V Lower','marking',vL,255,nothing)
cv2.createTrackbar('V Higher','marking',vH,255,nothing)

camera_number = 0
cap = cv2.VideoCapture(camera_number)
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

RESIZED_DIMENSIONS = (width, height) 
IMG_NORM_RATIO = 0.007843 

def dnn_follow():
	frame_blob = cv2.dnn.blobFromImage(cv2.resize(frame, RESIZED_DIMENSIONS), 
			IMG_NORM_RATIO, RESIZED_DIMENSIONS, 127.5)
	neural_network.setInput(frame_blob)
	neural_network_output = neural_network.forward()

	for i in np.arange(0, neural_network_output.shape[2]):  
		confidence = neural_network_output[0, 0, i, 2]
		if confidence > 0.50:
			idx = int(neural_network_output[0, 0, i, 1])
			bounding_box = neural_network_output[0, 0, i, 3:7] * np.array(
				[w, h, w, h])
 
 			(startX, startY, endX, endY) = bounding_box.astype("int")
 
			label = "{}: {:.2f}%".format(classes[idx], confidence * 100) 
         
			cv2.rectangle(frame, (startX, startY), (
				endX, endY), bbox_colors[idx], 2)     
                         
			y = startY - 15 if startY - 15 > 15 else startY + 15    
 
			cv2.putText(frame, label, (startX, y),cv2.FONT_HERSHEY_SIMPLEX, 
				0.5, bbox_colors[idx], 2)

def line_follow():
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

	hL = cv2.getTrackbarPos('H Lower','marking')
	hH = cv2.getTrackbarPos('H Higher','marking')
	sL = cv2.getTrackbarPos('S Lower','marking')
	sH = cv2.getTrackbarPos('S Higher','marking')
	vL = cv2.getTrackbarPos('V Lower','marking')
	vH = cv2.getTrackbarPos('V Higher','marking')

	LowerRegion = np.array([hL,sL,vL],np.uint8)
	upperRegion = np.array([hH,sH,vH],np.uint8)

	redObject = cv2.inRange(hsv,LowerRegion,upperRegion)

	kernal = np.ones((1,1),"uint8")

	red = cv2.morphologyEx(redObject,cv2.MORPH_OPEN,kernal)
	red = cv2.erode(red,kernal,iterations=1)
	red = cv2.dilate(red,kernal,iterations=1)
	red = cv2.dilate(red,kernal,iterations=1)
	red = cv2.erode(red,kernal,iterations=1)
	M = cv2.moments(red)
	if M["m00"] == 0:	
		cX = 0
		cY = 0
	else: 
		cX = int(M["m10"] / M["m00"])
		cY = int(M["m01"] / M["m00"])
	
	start_point = (cX, 0)
	end_point = (cX, height)
	color = (0, 255, 0)
	thickness = 2
	image = cv2.line(frame, start_point, end_point, color, thickness)
	
while cap.isOpened():
	success, frame = cap.read() 
	if success:
		(h, w) = frame.shape[:2]
		
		line_follow()
		dnn_follow()
        
		cv2.imshow('Frame',frame)   
		if cv2.waitKey(25) & 0xFF == ord('q'):
			break
	else:
		break
             
cap.release()
result.release()
