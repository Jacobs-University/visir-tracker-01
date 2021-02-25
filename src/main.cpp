#include <opencv2/videoio.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/core/mat.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include <time.h>

using namespace cv;
using namespace std;

Mat detect_face(Mat img, CascadeClassifier face_cascade, 
	CascadeClassifier eyes_cascade)
{
	/*
	Takes an image and detects and points out faces and eyes on that image with
	the provided Haar cascades

	args:

	img (Mat):						  the actual image
	face_cascade (CascadeClassifier): Haar cascade for detecting faces
	eyes_cascade (CascadeClassifier): Haar cascade for detecting eyes

	return:

	img (Mat): 						  the final image with the detected faces/
									  eyes pointed out
	*/

	// initialize variables
	std::vector<Rect> faces;
	Mat grayscale;

	// convert to grayscale and equalize histograms
	cvtColor(img, grayscale, COLOR_BGR2GRAY);
	equalizeHist(grayscale, grayscale);

	// detect faces in the image
	face_cascade.detectMultiScale(grayscale, faces);

	// for each face
	for (int i = 0; i < faces.size(); i++)
	{

		// get center of face
		Point center(faces[i].x + faces[i].width / 2, faces[i].y +
			faces[i].height / 2);
		
		// draw an ellipse on the image around the face
		ellipse(img, center, Size(faces[i].width / 2, faces[i].height / 2),
			0, 0, 360, Scalar(255, 0, 255), 4);
		
		// detect eyes on face
		Mat face = grayscale(faces[i]);
		std::vector<Rect> eyes;
		eyes_cascade.detectMultiScale(face, eyes);

		// for each eye
		for (int j = 0; j < eyes.size(); j++)
		{

			// get center of eye and build circle around it
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, 
				faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
			circle(img, eye_center, radius, Scalar(255, 0, 0), 4);
		}
	}

	// return final frame
	return img;
}

int main()
{

	// create cascades
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;

	VideoCapture camera;
	if (!camera.open(0))
	{
		printf("Can't find a camera\n");
		return 1;
	};

	String face_xml_path = samples::findFile(
		"haarcascades/haarcascade_frontalface_alt.xml");
	String eyes_xml_path = samples::findFile(
		"haarcascades/haarcascade_eye_tree_eyeglasses.xml");

	if (!face_cascade.load(face_xml_path))
	{
		cout << "Can't load face cascade XML\n";
		return 2;
	};
	if (!eyes_cascade.load(eyes_xml_path))
	{
		cout << "Can't load eyes cascade XML\n";
		return 3;
	};
	// Main loop
	Mat img;
	time_t start, end;
	while (1)
	{

		// start time
		time(&start);
		for (int i = 0; i < 60; i++)
		{
			camera >> img;

			// detect face/eyes in img
			img = detect_face(img, face_cascade, eyes_cascade);
			imshow("Camera", img);
			int key = waitKey(5);
			if (key == 27 || key == 'q')
				break;
		}

		// find difference in time and calculate and display FPS
		time(&end);
		cout << "FPS = " << 60 / difftime(end, start) << endl;
	}
	camera.release();
	return 0;
}
