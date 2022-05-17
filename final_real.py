#https://pyefd.readthedocs.io/en/latest/#first
#https://github.com/lp02781/kapalTenggelam/blob/master/src/groundserver/src/identificationPlane.cpp

import cv2
import numpy
from skimage import color
import numpy as np
from pyefd import elliptic_fourier_descriptors
import pyefd
green_retangle=[61, 99, 220, 255, 128, 176]
blue_triangle= [43, 127, 0, 210, 125, 196]
orange_hexagon= [0, 61, 69, 210, 184, 251]
red_circle= [0, 12, 76, 255, 255, 255]
purple_square= [132, 173, 23, 238, 123, 251]
yellow_pentagon= [9, 182, 80, 255, 255, 255]
ball = [10, 73, 71, 255, 177, 255]
real = [156, 236, 172, 255, 172, 255]
color = real
hL = color[0];
hH = color[1];
sL = color[2]; 
sH = color[3];
vL = color[4];
vH = color[5];
def efd_feature(contour):
    coeffs = elliptic_fourier_descriptors(
        contour, order=10, normalize=True)
    return coeffs.flatten()[3:]
LowerRegion = np.array([hL,sL,vL],np.uint8)
upperRegion = np.array([hH,sH,vH],np.uint8)
img = cv2.imread('triangle.png')
haha = img
img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
img = cv2.inRange(img,LowerRegion,upperRegion)
M = cv2.moments(img)
if M["m00"] == 0:	
    cX = 0
    cY = 0
else: 
    cX = int(M["m10"] / M["m00"])
    cY = int(M["m01"] / M["m00"])
img = cv2.Canny(img,100,200,5)
cv2.imshow('raw', img)
contours, hierarchy = cv2.findContours(
    img, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
coeffs = np.empty([80])
for cnt in contours:
    #coeffs = []
    np.append(coeffs,(elliptic_fourier_descriptors(
        numpy.squeeze(cnt), order=10)))
    count = len(np.squeeze(cnt))
    epsilon1 = 0.01*cv2.arcLength(cnt, True)          
    approx1 = cv2.approxPolyDP(cnt, epsilon1, True)
    approx_len = len(np.squeeze(approx1))
    for i in range(approx_len):
        haha = cv2.circle(haha, np.squeeze(approx1)[i], 5, (255, 0, 0), 1) 
    print(approx_len)
    print(np.size(coeffs))
    if(approx_len==3):
        text = "Triangle"
    elif(approx_len==4):
        if(color[0]==green_retangle[0]):
            text="Rectangle"
        else:
            text = "Square"
    elif(approx_len==5):
        text="Pentagon"
    elif(approx_len==6):
        text="Hexagon"
    else:
        text="Circle"
    text = "Triangle"
#print(coeffs)
B = np.reshape(coeffs, (-1, 4))
print(B)
#bagi tiap 4 angka
cv2.circle(haha, (cX, cY), 5, (0, 0, 255), 1)
cv2.putText(haha, text, (cX - 25, cY - 25),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (100, 5, 100), 2)
cv2.imshow('process', haha)
cv2.waitKey(0)        
cv2.destroyAllWindows()
pyefd.plot_efd(B, locus=(0.0, 0.0), image=None, contour=None, n=300)
