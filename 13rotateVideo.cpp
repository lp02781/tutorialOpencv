#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    // open the video file for reading
    VideoCapture cap("sample.mkv"); 

    // if not success, exit program
    if ( !cap.isOpened() )  
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    const char* pzOriginalWindowName = "Original Video";
    namedWindow(pzOriginalWindowName, CV_WINDOW_AUTOSIZE); 

    const char* pzRotatingWindowName = "Rotated Video";
    namedWindow( pzRotatingWindowName, CV_WINDOW_AUTOSIZE );

    int iAngle = 180;
    createTrackbar("Angle", pzRotatingWindowName, &iAngle, 360);


    while (true)
    {
        Mat matOriginalFrame;

 // read a new frame from video
        bool bSuccess = cap.read(matOriginalFrame); 

 //if not success, break loop
        if (!bSuccess) 
 {
                       cout << "Cannot read the frame from video file" << endl;
                       break;
        }

        imshow(pzOriginalWindowName, matOriginalFrame);

 //get the affine transformation matrix
 Mat matRotation = getRotationMatrix2D( Point(matOriginalFrame.cols / 2, matOriginalFrame.rows / 2), (iAngle - 180), 1 );
 
 // Rotate the image
 Mat matRotatedFrame;
 warpAffine( matOriginalFrame, matRotatedFrame, matRotation, matOriginalFrame.size() );

 imshow( pzRotatingWindowName, matRotatedFrame );

 //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        if (waitKey(30) == 27) 
 {
                cout << "esc key is pressed by user" << endl; 
                break; 
 }
    }

    return 0;
}
