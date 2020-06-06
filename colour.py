import cv2
import numpy as np

camera = cv2.VideoCapture(0)

def nothing(x):
    pass

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

while(1):
    _,img = camera.read()
    img = cv2.flip(img,1)

    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

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
    cv2.circle(img, (cX, cY), 5, (255, 255, 255), -1)
    cv2.putText(img, "centroid", (cX - 25, cY - 25),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
	
    cv2.imshow("Masking ",red)
    cv2.imshow("rgb ",img)

    if cv2.waitKey(10) & 0xFF == ord('q'):
        camera.release()
        cv2.destroyAllWindows()
        break
