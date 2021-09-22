#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <numeric>

using namespace cv;


template <typename T>
T average(std::vector<T> const& v) {
	if (v.empty()) {
		return 0;
	}
	return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

int main() {
	VideoCapture camera;
	if (!camera.open(0)) {
		printf("Can't find a camera\n");
		return 1;
	};

	// Main loop
	Mat img;
	std::vector<long> fpsCont;

	long frameCounter = 0;

	std::time_t timeBegin = std::time(0);
	int tick = 0;
	
	for (;;) {
		camera >> img;
		imshow("Camera", img);
		
		int key = waitKey(5);
	
		frameCounter++;
		fpsCont.push_back(frameCounter);
		
		std::time_t timeNow = std::time(0) - timeBegin;
		if (timeNow - tick >= 1)
		{
			tick+=2;
			std::cout << "Average Frames per second: " << average(fpsCont) << std::endl;
			frameCounter = 0;
			fpsCont.clear();
		}


		if (key == 27 || key == 'q') break;
	}
	camera.release();
	return 0;
}
