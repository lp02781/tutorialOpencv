//preprocessor
#include <iostream>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/highgui/highgui_c.h>
//namespace
using namespace cv;
using namespace std;

//matrix
Mat imgOriginal, imgHSV, imgThresholded, imgErode, imgDilate;

//hue
int LowH = 170;
int HighH = 179;

//saturation
int LowS = 150; 
int HighS = 255;

//value
int LowV = 60;
int HighV = 255;

//posisi awal dan akhir
int a = 0;
int b = 0;

//minimal pixel dan box
int minPix = 10;
int minBox = 100;

double posisiAkhir = 320;//posisi akhir

int mx,my;//nilai koordinat
int mflags=0;//0

int main( int argc, char** argv )
{
	VideoCapture cap(0);//webcam
	//VideoCapture cap("http://192.168.43.74:4747/video?x.mjpeg");//webcam
	//cap.set(CAP_PROP_FOURCC, VideoWriter_fourcc('M', 'J', 'P', 'G'))
	//cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G') );
	
	if ( !cap.isOpened() )  // jika tidak terbaca, exit
    {
         cout << "webcam tidak terbaca" << endl;
         return -1;
    }
	
	cap.read(imgOriginal);//capture di imgOriginal
    namedWindow("ColourTracking", CV_BGR2HSV);//nama window
	
	//trackbar
	createTrackbar("LowH", "ColourTracking", &LowH, 255); //Hue (0 - 255)
	createTrackbar("HighH", "ColourTracking", &HighH, 255);
	createTrackbar("LowS", "ColourTracking", &LowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "ColourTracking", &HighS, 255);
	createTrackbar("LowV", "ColourTracking", &LowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "ColourTracking", &HighV, 255);
	printf("Hue values of basic colors\n Orange  0-22\n Yellow 22- 38\n Green 38-75\n Blue 75-130\n Violet 130-160\n Red 160-179\n");
	
	while (true)//loop sampai exit
    {
		bool bSuccess = cap.read(imgOriginal); // membaca frame baru dari video
		if (!bSuccess) //jika tidak sukses, menghentikan loop
        {
             cout << "tidak bisa membaca frame dari video" << endl;
             break;
        }
		
		cvCvtColor(imgOriginal, imgHSV, CV_BGR2HSV); //mengubah frame BGR jadi HSV
		inRange(imgHSV, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), imgThresholded);//range threshold
		
		//hapus objek kecil
		cvErode(imgThresholded, imgThresholded, cvReleaseStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		cvErode( imgThresholded, imgThresholded, cvReleaseStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		
		//menghapus hole
		cvErode( imgThresholded, imgThresholded, cvReleaseStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		cvErode(imgThresholded, imgThresholded, cvReleaseStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		
		//menampilkan image
		imshow("Threshold", imgThresholded);
		imshow("Original", imgOriginal); 
        
        if (waitKey(30) == 27) //exit esc
        {
            cout << "esc di tekan" << endl;
            break; 
        }
	}
	return 0;
}
