# Practical Assignment 1
**Dealine**: 26.02.2021

Please put your name here:  
**Name:** Erlisa Kulla
## Problem 1.1
### Calculate Frames-per-Second (FPS) (Points 30)
1. Fork the current repository
2. Study the new framework-code of 
    - main.cpp
3. Check that the code is running correctly: it should show the video stream from the web-camera of your laptop.
4. Calculate average fps and print it to console every 2 seconds. Compare Debug and Release versions.
### Note
MacOS users may need to launch the application with the administrator rights, to grant access to the web-camera.
### Answer
In both Release and Debug versions my console displays and average of 30 fps.

## Problem 1.2
### Face detection (Points 70)
1. Read the OpenCV documentation about Viola-Jones face detector: [Cascade Classifier](https://docs.opencv.org/4.2.0/db/d28/tutorial_cascade_classifier.html)  
2. Implement face detection for the video stream from the web-camera using the ```cv::CascadeClassifier``` class.
3. Measure the FPS one more time. How FPS changed after incorporating the face detection into the framework?
### Note
Please do not copy-paste the example code from the OpenCV documentation, but try to understand the example code and implement the solution to the problem by yourself.
### Answer
Compared to the initial calculation of the fps, it is way lower. I implemented a variable ```scale``` which helps in making scaling the video output.
The higher this value is, the less lag there will be, therefore the higher the fps will be. At a scale 4, the fps reaches around 14-15.5 with slight variations, while with scale 3 it is even lower.
This is due to the implementing the face detection algorithm which takes a lot of time and processing power to compare frames to given samples and constantly produce an output.

## Submission
Please submit the assignment by making a pull request.
**Important** : Please make sure that
- No _extra files_ are submitted (except those, which were mentioned in the assignment)
- The changes were made _only_ in those files where you were asked to write your code
- The Continiouse Integration system (appVeyor) can build the submitted code
- The rendered images are also submitted in the folder "renders" 

