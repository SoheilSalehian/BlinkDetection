/*
 * DetectEyes.h
 *
 *  Created on: Oct 15, 2014
 *      Author: soheil
 */

#ifndef DETECTEYES_H_
#define DETECTEYES_H_

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>


using namespace cv;

String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_mcs_eyepair_big.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int contourThreshold = 2;
int lowThreshold=100/2;
const int maxThreshold = 200/2;
char* window_name="Edge map";

// Template Matching globals FIXME: globals
Mat frame, myTemplate, result;
int matchMethod;
int maxTrackbar = 5;

void detectAndDisplay(Mat frame);
int houghTransform(Mat frame);
void basicThreshold(Mat frame);
Mat cannyThreshold(Mat frame);
void contourAnalysis(Mat edgeDetectorOutput);
void showHistogram(Mat frame);
void MatchingMethod(int, void*);

class Frame
{
public:
	int captureFrame();
private:
	int frameID;
};



#endif /* DETECTEYES_H_ */
