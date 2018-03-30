#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main () {
	
	// display the original image
	IplImage* img = cvLoadImage("iseng.jpg");
	cvNamedWindow("Original Image");
	cvShowImage("Original Image", img);
	
	// invert and display the inverted image
	cvNot(img, img);
	cvNamedWindow("Inverted Image");
	cvShowImage("Inverted Image", img);
	
	cvWaitKey(0);
	
	//cleaning up
	cvDestroyWindow("Original Image");
	cvDestroyWindow("Inverted Image");
	cvReleaseImage(&img);
	
	return 0;
	
}
