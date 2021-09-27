#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

using namespace cv;
using namespace std;

void setInterval2(int seconds, atomic<int> &frames, atomic<bool> &on){
    int fps = 0;
    while (on){
        this_thread::sleep_for(chrono::milliseconds(seconds*1000));
        fps = frames/2;
        cout << "fps: " << fps << endl;
        frames = 0;
    }
}

CascadeClassifier face_cascade;
CascadeClassifier eye_cascade;

void detectFaces(Mat frame){
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    vector<Rect> faces;
    face_cascade.detectMultiScale(frame_gray, faces);

    for (size_t i = 0; i < faces.size(); i++){
        Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
        ellipse(frame, center, Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar(0, 64, 255), 2); 
        Mat faceROI = frame_gray(faces[i]);
        

        vector<Rect> eyes;
        eye_cascade.detectMultiScale(frame_gray, eyes);
        for (size_t i = 0; i < eyes.size(); i++){
            Point center_eye(eyes[i].x + eyes[i].width/2, eyes[i].y + eyes[i].height/2);
            ellipse(frame, center_eye, Size(eyes[i].width/2, eyes[i].height/2), 0, 0, 360, Scalar(100, 232, 124), 2);
            Mat eyeROI = frame_gray(eyes[i]);
        }
    }

    imshow("Face Detection for Skynet", frame);
}

int main(){
    std::atomic<bool> on {true};
    std::atomic<int> frames {0};

    VideoCapture camera;
    Mat frame;

    if (!face_cascade.load(samples::findFile("data/haarcascades/haarcascade_frontalface_alt.xml"))){
        cout << "Unable to load the face cascade!" << endl;   
        return -1;
    }

    if (!eye_cascade.load(samples::findFile("data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"))){
        cout << "Unable to load the eye cascade!" << endl;   
        return -1;
    }  

    camera.open(0);
    if (!camera.isOpened()) {
        printf("Can't find a camera\n");
        return 1;
    };

    thread timer(setInterval2, 2, ref(frames), ref(on));

    while (camera.read(frame)){
        if (frame.empty()){
            break;
        }

        frames++;
        detectFaces(frame);

        int key =  waitKey(5);
        if (key == 27 || key == 'q' || key == 'Q'){
            on = false;
            break;
        }
    }

    on = false;
    timer.join();
    camera.release();

    return 0;
}