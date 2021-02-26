#include "types.h"
#include "opencv2/opencv.hpp"
#include <time.h>
#include <vector>

using namespace cv;
using namespace std;

int main() {
	int scale = 4; //scaling down the video frame to make it more efficient

	CascadeClassifier face_detection;
	//loading face detection training files 
	face_detection.load("C:\\opencv-4.5.1\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalcatface.xml"); //alt

	VideoCapture camera;
	if (!camera.open(0)) {
		printf("Can't find a camera\n");
		return 1;
	};

	double num_frames = 0;
	int sec_count = 2;
	time_t start, end;
	
	// Main loop
	Mat img;

	time(&start);

	for(;;) {
		camera >> img;
		num_frames++;

		//1----calculating FPS every 2 seconds
		time(&end);
		double time = difftime(end, start);
		if (sec_count == time) {
			sec_count = sec_count + 2;
			cout << "FPS : " << num_frames / time << endl;
		}

		//2----detecting face
		Mat grayscale;
		//translating video frames to grayscale since the training pictures are grayscale
		cvtColor(img, grayscale, COLOR_BGR2GRAY); 

		//resizing video frame
		resize(grayscale, grayscale, Size(grayscale.size().width / scale, grayscale.size().height / scale));

		vector<Rect> faces; //vector of faces
		face_detection.detectMultiScale(grayscale, faces, 1.1, 3, 0, Size(30, 30));

		//drawing a rectangle around the detected face(s)
		for (Rect area : faces) {
			Scalar drawColor = Scalar(0, 0, 255);
			//first point argument is left top edge and the second
			//point argument is the right bottom edge
			rectangle(img, Point(cvRound(area.x*scale), cvRound(area.y*scale)), 
				Point(cvRound((area.x + area.width)*scale), cvRound((area.y + area.height + 10)*scale)), drawColor, 1.5);
		}

		imshow("Camera", img);
		int key = waitKey(5);

		if (key == 27 || key == 'q') break;
	}
	camera.release();

    return 0;
}
