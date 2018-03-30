#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
Mat Original, Gray, HSV, Threshold, Erode, Dilate;
int main()
{
	VideoCapture cap(0);
	int Low=233, High=255;
	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
	createTrackbar("Low","MyWindow",&Low,255);
	createTrackbar("High","MyWindow",&High,255);
	while(1)
	{
		cap.read(Original);
		cvtColor(Original, Gray, COLOR_BGR2GRAY);
		cvtColor(Original, HSV, COLOR_BGR2HSV);
		inRange(Gray,Low,High,Threshold);
		erode(Original, Erode, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
		dilate(Original, Dilate, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
		imshow("Original", Original);
		imshow("Erode",Erode);
		imshow("Dilate",Dilate);
		imshow("HSV", HSV);
		imshow("Grayscale", Gray);
		imshow("Threshold Image",Threshold);
		waitKey(30);
	}
}
