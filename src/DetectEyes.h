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

//String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
//String eyes_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
//CascadeClassifier face_cascade;
//CascadeClassifier eyes_cascade;

void detectAndDisplay(Mat frame);

class Frame
{
public:
	int captureFrame();
private:
	int frameID;
};



#endif /* DETECTEYES_H_ */
