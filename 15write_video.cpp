
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
int main(){
	VideoCapture cap(0);
	Size framesize(640,480);
	VideoWriter oVideoWriter("MyVideo.avi",CV_FOURCC('P','I','M','1'),20, framesize);
	while(1){
		Mat frame;
		cap.read(frame);
		oVideoWriter.write(frame);
		waitKey(10);
	}
}
