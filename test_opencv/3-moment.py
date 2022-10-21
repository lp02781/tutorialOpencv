import cv2
import numpy as np

#camera = cv2.VideoCapture(0)
img = cv2.imread('stop.png')

hL = 0;
hH = 234;

sL = 169; 
sH = 255;

vL = 188;
vH = 255;

while(1):
    #_,img = camera.read()

    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    centeroid = img.copy()
    LowerRegion = np.array([hL,sL,vL],np.uint8)
    upperRegion = np.array([hH,sH,vH],np.uint8)

    redObject = cv2.inRange(hsv,LowerRegion,upperRegion)
    
    kernal = np.ones((5,5),"uint8")

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

    cv2.circle(centeroid, (cX, cY), 20, (0, 250, 150), 10)
    cv2.putText(centeroid, "centroid", (cX - 100, cY - 50),
          cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 250, 150), 5)

    cv2.imshow("real",img)
    cv2.imshow("Noise",redObject)
    cv2.imshow("Masking ",red)
    cv2.imshow("centeroid ",centeroid)

    if cv2.waitKey(10) & 0xFF == ord('q'):
        cv2.destroyAllWindows()
        break
