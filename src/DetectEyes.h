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

const int contourThreshold = 5;
const int lowThreshold=65;
const int maxThreshold =130;
char* window_name="Edge map";
int previousHistBin = 0;


Mat openTemplate;

// Template Matching globals FIXME: globals
Mat frame, myTemplate, result;
int matchMethod = 0;
int maxTrackbar = 5;

int templateCapture = 1;

void detectAndDisplay(Mat frame);
int houghTransform(Mat frame);
Mat basicThreshold(Mat frame);
Mat cannyThreshold(Mat frame);
int contourAnalysis(Mat edgeDetectorOutput);
void momentAnalysis(Mat edgeDetectorOutput);
int showHistogram(Mat frame);
void MatchingMethod(Mat frame);
int pixelNumberAnalysis(Mat frame);
bool eyeOpenOrClosed(int histogramResult, int contourResult);


class Frame
{
public:
	int captureFrame();
private:
	int frameID;
};



#endif /* DETECTEYES_H_ */
