#include "types.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

void setInterval(int seconds, std::atomic<int> &frames, std::atomic<bool> &on){
	int fps = 0;
	while (on){
		std::this_thread::sleep_for(std::chrono::milliseconds(seconds*1000));
		fps = frames/2;
		std::cout << "fps: " << fps << std::endl;
		frames = 0;
	}
}

int main() {
	std::atomic<bool> on {true};
	std::atomic<int> frames {0};

	VideoCapture camera;
	if (!camera.open(0)) {
		printf("Can't find a camera\n");
		return 1;
	};
	
	std::thread timer(setInterval, 2, std::ref(frames), std::ref(on));

	// Main loop
	Mat img;
	for(;;) {
		camera >> img;
		imshow("Camera", img);
		frames++;
		int key = waitKey(5);
		if (key == 27 || key == 'q') {
			on = false;
			break;
		};
		// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	on = false;
	timer.join();
	camera.release();
	return 0;
}
