#include "types.h"

int main() {
	VideoCapture camera;
	if (!camera.open(1)) {
		printf("Can't find a camera\n");
		return 1;
	};
	
	// Main loop
	Mat img;
	double tframe = 0;
	double frame = 0;

	for(;;) {
		double t = cv::getTickCount();

		camera >> img;
		imshow("Camera", img);
		int key = waitKey(5);	
		if (key == 27 || key == 'q') break;

		tframe += (cv::getTickCount() - t) / cv::getTickFrequency();
		frame++; 
		if (tframe >= 2.0) {
			std::cout << "FPS: " <<  frame / tframe << std::endl;
			tframe = 0;
			frame = 0;
		}
		

	}
	camera.release();
	return 0;
}
