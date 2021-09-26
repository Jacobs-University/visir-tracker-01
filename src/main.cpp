#include "types.h"
#include <ctime>

int main() {
	VideoCapture camera;
	if (!camera.open(0)) {
		printf("Can't find a camera\n");
		return 1;
	};
	
	// Main loop
	Mat img;
	Mat conv_img;
	std::time_t start = std::time(nullptr);
	float diff = 0;
	int counter = 0;
	CascadeClassifier face_cascade;
	std::vector<Rect> faces;
	face_cascade.load("haarcascade_frontalface_default.xml");
	for(;;) {
		camera >> img;
		cvtColor( img, conv_img, COLOR_BGR2GRAY);
		face_cascade.detectMultiScale(img, faces);
		Point center( faces[0].x + faces[0].width/2, faces[0].y + faces[0].height/2 );
		ellipse( img, center, Size( faces[0].width/2, faces[0].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
		imshow("Camera", img);
		counter++;
		int key = waitKey(5);
		diff = std::time(nullptr) - start;
		if (diff >= 2){
			printf("fps: %f\n", counter/diff);
			counter=0;
			start = std::time(nullptr);
		}
		if (key == 27 || key == 'q') break;
	}
	camera.release();
	return 0;
}
