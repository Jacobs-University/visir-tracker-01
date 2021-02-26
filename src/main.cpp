#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <time.h>
#include <vector>

#include "types.h"

using namespace cv;
using namespace std;

int main(){
    double scale = 3.0;
    
    CascadeClassifier faceCascade;
    faceCascade.load("/usr/local/Cellar/opencv/4.5.1_2/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml"); //REPLACE path with your own path to "haarcascade_frontalface_alt.xml" file
    
    VideoCapture camera;
    if (!camera.open(0)) {
        printf("Can't find a camera\n");
        return 1;
    };
    
    int num_frames = 0;
    time_t start=NULL, end=NULL;
    
    time(&start);
    for(;; num_frames++){
        Mat img;
        camera >> img;
        
        time(&end);
        
        Mat grayscale;
        cvtColor(img, grayscale, COLOR_BGR2GRAY); //FUNCTION to convert color image to gray scale
        resize(grayscale, grayscale, Size(grayscale.size().width / scale, grayscale.size().height / scale)); //FUNCTION to resize the image from the webcam
        
        vector<Rect> faces;
        faceCascade.detectMultiScale(grayscale, faces, 1.1, 3, 0, Size(30,30));
        
        for (Rect area : faces){
            Scalar drawColor = Scalar(255, 0, 0); //Defining color for the rectangle
            rectangle(img, Point(cvRound(area.x * scale), cvRound(area.y * scale)),
                     Point(cvRound((area.x + area.width - 1) * scale), cvRound((area.y + area.height - 1) * scale)), drawColor);
        }
        
        //double currentTime = difftime(end, start);
        if(difftime(end, start) >= 2)//check if 2 second passed
                {
                    // Time elapsed
                    double seconds = difftime(end, start);
                    std::cout << "Time taken : " << seconds << " seconds" << std::endl;
        
                    // Calculate frames per second
                    double fps = num_frames / seconds;
                    std::cout << "Estimated frames per second : " << fps << std::endl;
        
                    //Reset the timer and frames.
                    time(&start);
                    num_frames = 0;
                }
        
        imshow("Camera", img);
                int key = waitKey(5);
                if (key == 27 || key == 'q') break;
        }
    camera.release();
    return 0;
}
