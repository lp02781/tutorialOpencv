#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main (int argc, const char** argv) {
	
	Mat img = imread ("iseng.jpg", CV_LOAD_IMAGE_COLOR);//read image from directory
	
	if (img.empty()) {
		cout << "Image connot be loaded..!!"<< endl;
		return -1;
		}
	
	Mat imgH;
	img.convertTo(imgH, -1, 2, 0); // increase the contrast (double)
	
	Mat imgL;
	img.convertTo(imgL, -1, 0.5, 0); // decrease the contrast (halve)
	
	// create windows
	namedWindow("original image", CV_WINDOW_AUTOSIZE);
	namedWindow("high contrast", CV_WINDOW_AUTOSIZE);
	namedWindow("low contrast", CV_WINDOW_AUTOSIZE);
	
	// show the image
	imshow("original image", img);
	imshow("high contrast", imgH);
	imshow("low contrast", imgL);
	
	waitKey(0);
	
	destroyAllWindows();
	
	return 0;
	
}
