#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include "opencv/cv.h"

using namespace std;
using namespace cv;

int tolV0=100; // value tolerance (huge, due to lighting variations)
int tolH0=10; // hue tolerance for red color
int tolS0=100; // saturation tolerance for red color

int tolV1=100;
int tolH1=21; // hue tolerance for green color
int tolS1=100; // saturation tolerance for green color

int p = 0; // p value
int q = 0; // q value

int targetH = 170;
int targetS = 200;
int targetV = 200;	
	
int min_pixels=10; // minimum detected size (box with this many pixels on a side)
int box_pixels=100; // ignore detected pixels farther than this from the initial center of mass

double lastPositionX = 320;	//lastposition x
double lastPositionY = 320; //lastposition y

int largest_area=0;
int largest_contour_index=0;
Rect bounding_rect;

static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


/* Return the current wall clock time, in seconds */
double time_in_seconds(void) 
{
	return getTickCount()/getTickFrequency();//tick persecond
}

class color_matcher // Search an image for a particular color, and return the coordinates of that color in the image.
{
	public:
		Mat threshed;  // thresholded image: black for no match; white for good match
	
		float posX,posY; // (x,y) pixel coordinates of matching pixels' center of mass
		float area; // pixels hit by center of mass
	
		// Look for this HSV color in this image.
		void match(const Mat &imgHSV,const Vec3b &target);//match to imgHSV
		public:
		/* Set our position from the center of mass of this image. */
		void calcMoments(const Mat &img) 
		{
			Moments mu=moments(img);
			area = mu.m00; // sum of zero'th moment is area
			posX = mu.m10/area; // center of mass = w*x/weight
			posY = mu.m01/area;// center of mass = w*y/high
			area /= 255; // scale from bytes to pixels
		}
};
	
void color_matcher::match(const Mat &imgHSV,const Vec3b &target)
{
	threshed=Mat(imgHSV.size(),1);
	inRange(imgHSV, 
		Scalar(target[0]-tolH0, target[1]-tolS0, target[2]-tolV0),//convert imgHSV to threshed
		Scalar(target[0]+tolH1, target[1]+tolS1, target[2]+tolV1),
		threshed);
	
	//erase small object
	erode(threshed, threshed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( threshed, threshed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//erase hole
	dilate( threshed, threshed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(threshed, threshed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	
	
	// Calculate the moments to estimate average position
	calcMoments(threshed);
}

int main(int argc,char *argv[])
{
	double pointReadX;
	double pointOutputX;
	double pointReadY;
	double pointOutputY;
	
	VideoCapture *cap=0;
	long framecount=0;
	double time_start=-1.0;
	
	// Initialize capturing live feed from the camera
	if (!cap) cap=new VideoCapture(0);

	// Couldn't get a device? Throw an error and quit
	if(!cap->isOpened())
	{
		cout<<"Could not initialize capturing"<<endl;
		return -1;
	}

	// Make the windows we'll be using
	namedWindow("control panel");
	createTrackbar("Hue 0","control panel",&tolH0,255,NULL);
	createTrackbar("Sat 0","control panel",&tolS0,255,NULL);
	createTrackbar("Val 0","control panel",&tolV0,255,NULL);
	
	createTrackbar("Hue 1","control panel",&tolH1,255,NULL);
	createTrackbar("Sat 1","control panel",&tolS1,255,NULL);
	createTrackbar("Val 1","control panel",&tolV1,255,NULL);
	
	createTrackbar("targetH","control panel",&targetH,255,NULL);
	createTrackbar("targetS","control panel",&targetS,255,NULL);
	createTrackbar("targetV","control panel",&targetV,255,NULL);
	
	createTrackbar("Dev(+)","control panel",&p,512,NULL);
	createTrackbar("Dev(-)","control panel",&q,512,NULL);
	
	createTrackbar("Area min","control panel",&min_pixels,200,NULL);
	createTrackbar("box pixel","control panel",&box_pixels,200,NULL);

	// This image holds the "scribble" data...
	// the tracked positions of the target
	Mat scribble;
	
	// Will hold a frame captured from the camera
	Mat frame, frameHSV;
		
	// Image color matching:
	enum {nMatchers=2};
	color_matcher matchers[nMatchers];
	
	Vec3b targets[nMatchers];
	targets[0]=Vec3b(targetH,targetS,targetV); // red color ->change this row to detect another color
	targets[1]=Vec3b(targetH,targetS,targetV); // red color ->change this row to detect another color


	// An infinite loop
	while(true)
	{
		(*cap)>>frame; // grab next frame from camera

		// If we couldn't grab a frame... quit
		if(frame.empty())
			break;
		
		Mat gray, bw, cp,colp;
		cp = frame.clone();
		colp = frame.clone();
		cvtColor(cp, gray, CV_BGR2GRAY);

		// Use Canny instead of threshold to catch squares with gradient shading
		Canny(gray, bw, 0, 50, 5);
		
		vector<vector<Point> > contours;//inisialisasi dari objek contour
		findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		vector<Point> approx;
		
		framecount++;
		if (time_start<0) time_start=time_in_seconds();
		
		if (scribble.empty())
		scribble = Mat::zeros(frame.size(),CV_8UC3);
		
		// Convert to HSV (for more reliable color matching)
		cvtColor(colp, frameHSV, CV_BGR2HSV);
		
		// Do color matching
		for (int m=0;m<nMatchers;m++)
		matchers[m].match(frameHSV,targets[m]);
		
		pointReadX = matchers[0].posX;
		pointReadY = matchers[0].posY; // read x coordinat
		pointOutputX = pointReadX + p - q; // x coordinat after deviation
		pointOutputY = pointReadY + p - q;
		
		for (int i = 0; i < contours.size(); i++)//loop sampe jumlah isi dari contours
		{
			// Approximate contour with accuracy proportional
			// to the contour perimeter
			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

			// Skip small or non-convex objects 
			if (fabs(contourArea(contours[i])) < 5000 || !isContourConvex(approx))
			continue;

			if (approx.size() == 3)
			{
				// NaN Handler
				// Last position handler
				if (pointReadX != pointReadX && pointReadY != pointReadY)
				{
					pointOutputX = lastPositionX;
					pointOutputY = lastPositionY;
			
					cout<<"error X Y -> last x : "<<lastPositionX<<" last y: "<<lastPositionY<<endl;
				}
				else
				{
			
					cout<<"current x : "<<lastPositionX<<" current y: "<<lastPositionY<<endl;
					lastPositionX = pointOutputX;
					lastPositionY = pointOutputY;
					fflush(stdout);
				}
			}
			else
			{
				cout<<"current x : "<<lastPositionX<<" current y: "<<lastPositionY<<endl;
				lastPositionX = pointOutputX;
				lastPositionY = pointOutputY;
				fflush(stdout);		
			}
		}
				
		// Mix with video
		frame=frame*0.5+scribble;
		//cv::imshow("video", frame);
		
		// Circle
		circle( frame, Point( matchers[0].posX, matchers[0].posY ), 8, Scalar( 0, 0, 255 ), 2);//red circle
		circle( frame, Point( pointOutputX, pointOutputY ), 10, Scalar( 0, 255, 255 ), 2);//yellow circle
				
		// Slowly scale back old scribbles to zero
		scribble=scribble*0.94-1.1;
		imshow("Video Window", frame);
		imshow("threshRed", matchers[0].threshed);

		// Wait for a keypress (for up to 1ms)
		int c = waitKey(1);
		if(c!=-1)
		{
			// If any key is pressed, break out of the loop
			break;
		}
	}
	return 0;
}	
