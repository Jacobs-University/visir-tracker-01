#include "types.h"
#include <time.h>>
#include <chrono>

Mat detect(Mat img);

using namespace cv;

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main(int argc, const char** argv) {

	// Set up parser
	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{face_cascade|data/haarcascades/haarcascade_frontalface_alt.xml|Path to face cascade.}"
		"{eyes_cascade|data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|Path to eyes cascade.}"
		"{camera|0|Camera device number.}");

	parser.about("\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face + eyes) in a video stream.\n"
		"You can use Haar or LBP features.\n\n");
	parser.printMessage();

	// Set up cascades
	String face_cascade_name = samples::findFile(parser.get<String>("face_cascade"));
	String eyes_cascade_name = samples::findFile(parser.get<String>("eyes_cascade"));

	// Load the cascades
	if (!face_cascade.load(face_cascade_name))
	{
		std::cout << "--(!)Error loading face cascade\n";
		return -1;
	};
	if (!eyes_cascade.load(eyes_cascade_name))
	{
		std::cout << "--(!)Error loading eyes cascade\n";
		return -1;
	};

	// Check if camera is available
	VideoCapture camera;
	if (!camera.open(0)) {
		printf("Can't find a camera\n");
		return 1;
	};
	
	// Number of frames to capture
	int num_frames = 1;
	double fps;

	// Start and end times for FPS estimation
	time_t start, end;

	// Start and end timers for elapsed time estiamtion
	time_t timerStart, timerEnd;

	// Main loop
	Mat img;
	
	// Start timer for elapsed time estimation
	timerStart = clock();
	for(;;) 
	{
		// Start clock for FPS estimation
		// Happens before image processing begins
		start = clock();

		// Get image from camera
		camera >> img;

		// Detect face and draw recognition
		img = detect(img);

		// Show the image with facial regonition features
		imshow("Camera", img);

		// End clock for FPS estimation
		// Happens after image processing ends
		end = clock();

		// Detect quit key
		int key = waitKey(5);
		if (key == 27 || key == 'q') break;

		// Calculate number of seconds elapsed during image processing
		double seconds = (double(end) - double(start)) / double(CLOCKS_PER_SEC);

		// Calculate average FPS
		fps = double(num_frames) / double(seconds);

		// Take current clock value
		timerEnd = clock();

		// Recalculate number of seconds to estimate elapsed time so far
		seconds = (double(timerEnd) - double(timerStart)) / double(CLOCKS_PER_SEC);

		// If elapsed time greater than 2 seconds, print FPS and reset timer 
		if (seconds >+ 2)
		{
			timerStart = clock();
			std::cout << "Average FPS: " << fps << "\n";
		}
	}

	camera.release();
	return 0;
}

// Function to detect face and eyes
Mat detect(Mat img)
{
	// Initialize blank mat for gray image
	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	equalizeHist(img_gray, img_gray);

	// Initialize vector for faces
	std::vector<Rect> faces;
	face_cascade.detectMultiScale(img_gray, faces);

	// Process and recognize face and eyes
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(img, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);

		Mat faceROI = img_gray(faces[i]);

		// In each face, detect eyes
		std::vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes);

		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
			circle(img, eye_center, radius, Scalar(255, 0, 0), 4);
		}
	}

	// Return image with detection circles
	return img;
}
