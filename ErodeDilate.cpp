#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
Mat Original, Gray, Threshold, Erode, Dilate;
int main(){
	VideoCapture cap(0);
	int Low = 233, High = 255;
	while(1){
		cap.read(Original);
		cvtColor(Original, Gray, COLOR_BGR2GRAY);
		inRange(Gray, Low, High, Threshold);
	erode(Original, Erode, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
	dilate(Original, Dilate, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	imshow("Original", Original);
	imshow("Erode",Erode);
	imshow("Dilate",Dilate);
	waitKey(30);
}}
