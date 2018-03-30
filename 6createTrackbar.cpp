#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
int main(){
	VideoCapture cap(0);
	Mat Original, Gray, Threshold;
	int Low = 233, High = 255;
	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
	createTrackbar("Low","MyWindow",&Low,255);//make trackbar
	createTrackbar("High","MyWindow",&High,255);
	while(1){
		cap.read(Original);
		cvtColor(Original, Gray, COLOR_BGR2GRAY);
		inRange(Gray,Low,High,Threshold);//convert based on trackbar
		imshow("Threshold",Threshold);
		waitKey(30);
	}}
