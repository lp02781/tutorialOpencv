import cv2
import numpy as np

#camera = cv2.VideoCapture(0)
img = cv2.imread('wkwk.png')

hL = 16;
hH = 243;

sL = 176; 
sH = 255;

vL = 255;
vH = 255;

while(1):
    #_,img = camera.read()
    
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    cnt = img.copy()
    LowerRegion = np.array([hL,sL,vL],np.uint8)
    upperRegion = np.array([hH,sH,vH],np.uint8)

    colour_filter = cv2.inRange(hsv,LowerRegion,upperRegion)
    
    contours, hierarchy = cv2.findContours(
    colour_filter, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    cv2.drawContours(cnt, contours, -1, (0,255,0), 3)

    cv2.imshow("real ",img)
    cv2.imshow("threshold ",colour_filter)
    cv2.imshow("countour ",cnt)

    if cv2.waitKey(10) & 0xFF == ord('q'):
        cv2.destroyAllWindows()
        break
