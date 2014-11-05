/*
 * DetectEyes.cpp
 *
 *  Created on: Sep 29, 2014
 *      Author: soheil
 */

#include "DetectEyes.h"

int lowThreshold=99;
const int maxThreshold = 100;
char* window_name="Edge map";




int main()
{

	CvCapture* capture;
	Mat frame;

	if( !face_cascade.load( face_cascade_name ) ){ std::cout << "--(!)Error loading\n"<<std::endl;; return -1; };
	if( !eyes_cascade.load( eyes_cascade_name ) ){ std::cout<< "--(!)Error loading\n"<<std::endl; return -1; };

	// Capture from camera
	capture = cvCaptureFromCAM(0);
	if (capture)
	{
		while (true)
		{
			frame = cvQueryFrame (capture);
			if (!frame.empty())
			{
				detectAndDisplay(frame);
			}
			else
			{
				std::cout << "(ERR) No captured frame...quitting" << std::endl;
				break;
			}
		int c = waitKey(10);
		if ( (char) c == 'c')
			break;
		}
	}
	return 0;
}

int Frame::captureFrame(){

	CvCapture* capture;
	capture = cvCaptureFromCAM(0);

	if (capture)
		return 1;
	return 0;
}

void detectAndDisplay(Mat frame)
{
	std::vector<cv::Rect> faces, eyes;
	cv::Rect rectangle;
	cv::Mat frameROI;
	Mat grayFrame;
	std::vector<Mat> rgb;
//	cvtColor (frame, grayFrame, CV_BGR2GRAY);
	cv::split(frame, rgb);
	// Pick the green channel
	grayFrame = rgb[1];
	equalizeHist (grayFrame, grayFrame);


	// Detect faces
	face_cascade.detectMultiScale(grayFrame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE,Size(30,30));

	for (size_t i = 0; i < faces.size(); i++ )
	{
		// Find centroid for face ROI
		Point center (faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height*0.5);
		// Draw face ROI ellipse
		ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255,0,0 ), 1, 8, 0 );

		Mat faceROI = grayFrame(faces[i]);
		std::vector<Rect> eyes;

		// Detect eyes per face
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

		for (size_t j = 0; j < eyes.size(); j++)
		{

			rectangle = eyes[0] + cv::Point(faces[i].x, faces[i].y);
			frameROI = frame(rectangle);
			imshow("ROI",frameROI);

			houghTransform(cannyThreshold(frameROI));
			cv::rectangle(frame, rectangle, CV_RGB(0,255,0));
		}
	}

//	for (std::vector<Rect>::const_iterator i = eyes.begin(); i != eyes.end(); ++i)
//		std::cout << *i << std::endl;
}


Mat cannyThreshold(Mat frame)
{
  Mat detected_edges;
  Mat dst;
  /// Reduce noise with a kernel 3x3
  blur( frame, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*2, 3 );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  frame.copyTo(dst, detected_edges);
  return dst;
 }


int houghTransform(Mat grayFrame)
{
	cvtColor(grayFrame, grayFrame, CV_BGR2GRAY);

	vector<Vec3f> circles;
	HoughCircles(grayFrame, circles, CV_HOUGH_GRADIENT, 1, grayFrame.rows/8, 30, 60, 0, 0);

	std::cout << circles.size() << std::endl;

	for (int i=0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			circle(grayFrame, center, 3, Scalar(0,255,0), -1, 8 ,0);
			circle(grayFrame, center, radius, Scalar(0,0,255), 3, 8, 0);
		}


	  namedWindow( "Hough Transform", CV_WINDOW_AUTOSIZE );
	  imshow( "Hough Transform", grayFrame);


	  return 0;
}

void basicThreshold(Mat frame)
{
    Mat imageGray;
	cv::cvtColor(frame, imageGray, CV_BGR2GRAY);

//	cv::GaussianBlur(imageGray, imageGray, Size(9,9), 2, 2);
	imshow("gray", imageGray);
//	threshold(imageGray, frame, 16, 255, CV_THRESH_BINARY);
	threshold( imageGray, imageGray, 200, 240, CV_THRESH_BINARY);


//	imshow("threshold", imageGray);
}
