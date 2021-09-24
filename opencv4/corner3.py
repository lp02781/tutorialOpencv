import numpy as np
import cv2
from matplotlib import pyplot as plt
  
  
# read the image
img = cv2.imread('hehe.jpg')
  
# convert image to gray scale image
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
canny = cv2.Canny(gray, 120, 255, 1)
  
# detect corners with the goodFeaturesToTrack function.
corners = cv2.goodFeaturesToTrack(gray, 27, 0.01, 10)
corners = np.int0(corners)

count = 0
  
# we iterate through each corner, 
# making a circle at each point that we think is a corner.
for i in corners:
    x, y = i.ravel()
    cv2.circle(img, (x, y), 3, 255, -1)
    count+=1

print(count)  
plt.imshow(img), plt.show()


