#include "types.h"

int main() {
	VideoCapture camera;
	if (!camera.open(0)) {
		printf("Can't find a camera\n");
		return 1;
	};
	Mat img;
	//cascade classifier
	std::vector<Rect> faces;
	CascadeClassifier faceCascade;
	faceCascade.load("D:/softwares-apps/Newfolder/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml");
	if (faceCascade.empty())
	{
		std::cout << "No XML file loaded" << std::endl;
	}

	int i = 0;
	int clockTracker = 2;
	time_t now, then;
	now = time(NULL);  // gets current time

	// Main loop
	for (;;) {
		camera.read(img); //captures a frame
		++i;

		faceCascade.detectMultiScale(img, faces);
		for (int i = 0; i < faces.size(); i++)
		{
			rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 0), 3);
		}

		imshow("Camera", img);
		int key = waitKey(5);
		then = time(NULL);
		double timeDifference = difftime(then, now);
		if (clockTracker == timeDifference)
		{
			clockTracker = clockTracker + 2;  //tracks the clock every two seconds
			std::cout << "Frame per second is: " << double(i) / (double)timeDifference << std::endl;
		}
		if (key == 27 || key == 'q') break;
	}
	camera.release(); // disconnects and releases camera ie it is nomore kept in use
	return 0;
}