#include "types.h"

#include <chrono>
#include <iostream>
using namespace std;
using sec = chrono::duration<double>;

CascadeClassifier face_cascade("../../data/haarcascade_frontalface_default.xml");

vector<Rect> face_detector(const Mat& img) {
	Mat gray_img;
	vector<Rect> res;
	cvtColor(img, gray_img, COLOR_BGR2GRAY);
	equalizeHist(gray_img, gray_img);
	face_cascade.detectMultiScale(gray_img, res, 1.2, 3);
	return res;
}

int main() {
	VideoCapture camera;
	if (!camera.open(0)) {
		printf("Can't find a camera\n");
		return 1;
	};
	
	// Main loop
	Mat img;
	vector<Rect> faces;
	int frames = 0;
	sec duration;
	const auto start = chrono::system_clock::now();
	double currentThreshold = 2.0;
	for(;;) {
		camera >> img;
		if (!img.empty()) {
			faces = face_detector(img);
			for (const auto& face : faces)
				rectangle(img, face, CV_RGB(0, 0, 255));
			imshow("Camera", img);
			int key = waitKey(5);
			if (key == 27 || key == 'q') break;
			frames++;
			duration = chrono::system_clock::now() - start;
			if (duration >= (sec)currentThreshold) {
				cout << "FPS: " << frames / duration.count() << endl;
				currentThreshold += 2.0;
			}
		}
	}
	camera.release();
	return 0;
}
