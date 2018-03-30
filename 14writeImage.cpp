 #include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, const char** argv )
{
     Mat img(650, 600, CV_16UC3, Scalar(0,50000, 50000)); //create an image ( 3 channels, 16 bit image depth, 650 high, 600 wide, (0, 50000, 50000) assigned for Blue, Green and Red plane respectively. )

     if (img.empty()) //check whether the image is loaded or not
     {
          cout << "ERROR : Image cannot be loaded..!!" << endl;
          //system("pause"); //wait for a key press
          return -1;
     }

     vector<int> compression_params; //vector that stores the compression parameters of the image

     compression_params.push_back(CV_IMWRITE_JPEG_QUALITY); //specify the compression technique

     compression_params.push_back(98); //specify the compression quality



     bool bSuccess = imwrite("TestImage.jpg", img, compression_params); //write the image to file



     if ( !bSuccess )

    {

         cout << "ERROR : Failed to save the image" << endl;

         //system("pause"); //wait for a key press

    }

     namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("MyWindow", img); //display the image which is stored in the 'img' in the "MyWindow" window

     waitKey(0);  //wait for a keypress

     destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"

     return 0;
}
