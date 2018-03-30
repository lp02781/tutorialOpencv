
#include <cv.h>
#include <highgui.h>
using namespace std;

IplImage* imgTracking=0;

int lastX1 = -1;
int lastY1 = -1;

int lastX2 = -1;
int lastY2 = -1;

void trackObject(IplImage* imgThresh)
{
	CvSeq* contour;  //hold the pointer to a contour
    CvSeq* result;     //hold sequence of points of a contour
    CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours
 
    //finding all contours in the image
    cvFindContours(imgThresh, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
  
    //iterating through each contour
    while(contour)
    {
		//obtain a sequence of points of the countour, pointed by the variable 'countour'
        result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);
          
        //if there are 3 vertices  in the contour and the area of the triangle is more than 100 pixels
        if(result->total==3 && fabs(cvContourArea(result, CV_WHOLE_SEQ))>100 )
        {
              //iterating through each point
              CvPoint *pt[3];
              for(int i=0;i<3;i++)
              {
                   pt[i] = (CvPoint*)cvGetSeqElem(result, i);
              }
   
               int posX=( pt[0]->x + pt[1]->x + pt[2]->x )/3;
               int posY=( pt[0]->y + pt[1]->y + pt[2]->y )/3;

               if(posX > 360 )
               {
                   if(lastX1>=0 && lastY1>=0 && posX>=0 && posY>=0)
                   {
                       // Draw a red line from the previous point to the current point
                       cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX1, lastY1), cvScalar(0,0,255), 4);
                   }

                    lastX1 = posX;
                    lastY1 = posY;  
               }
               else
               {
                    if(lastX2>=0 && lastY2>=0 && posX>=0 && posY>=0)
                    {
                        // Draw a blue line from the previous point to the current point
                         cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX2, lastY2), cvScalar(255,0,0), 4);
                     }

                      lastX2 = posX;
                      lastY2 = posY; 
                 }
            }
  
            //obtain the next contour
            contour = contour->h_next; 
      }

      cvReleaseMemStorage(&storage);
}


int main()
{	
    //load the video file to the memory
    CvCapture *capture =     cvCaptureFromAVI("sample.mkv");

    if(!capture){
        printf("Capture failure\n");
        return -1;
    }
      
    IplImage* frame=0;
    frame = cvQueryFrame(capture);           
    if(!frame) return -1;
   
    //create a blank image and assigned to 'imgTracking' which has the same size of original video
    imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
    cvZero(imgTracking); //covert the image, 'imgTracking' to black

    cvNamedWindow("Video");     

    //iterate through each frames of the video     
    while(true)
    {

        frame = cvQueryFrame(capture);           
        if(!frame) break;
        frame=cvCloneImage(frame); 
         
        //smooth the original image using Gaussian kernel
        cvSmooth(frame, frame, CV_GAUSSIAN,3,3); 

        //converting the original image into grayscale
        IplImage* imgGrayScale = cvCreateImage(cvGetSize(frame), 8, 1); 
        cvCvtColor(frame,imgGrayScale,CV_BGR2GRAY);
          
        //thresholding the grayscale image to get better results
        cvThreshold(imgGrayScale,imgGrayScale,100,255,CV_THRESH_BINARY_INV);
            
        //track the possition of the ball
        trackObject(imgGrayScale);

        // Add the tracking image and the frame
        cvAdd(frame, imgTracking, frame);
             
        cvShowImage("Video", frame);
   
        //Clean up used images
        cvReleaseImage(&imgGrayScale);            
        cvReleaseImage(&frame);

        //Wait 10mS
        int c = cvWaitKey(10);
        //If 'ESC' is pressed, break the loop
        if((char)c==27 ) break;      
    }

    cvDestroyAllWindows();
    cvReleaseImage(&imgTracking);
    cvReleaseCapture(&capture);     

    return 0;
}
