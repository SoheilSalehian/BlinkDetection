/*
 * DetectEyes.cpp
 *
 *  Created on: Sep 29, 2014
 *      Author: soheil
 */

#include "DetectEyes.h"




//int main()
//{
//
//	CvCapture* capture;
//	Mat frame;
//
//	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
//	if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
//
//	// Capture from camera
//	capture = cvCaptureFromCAM(-1);
//	if (capture)
//	{
//		while (true)
//		{
//			frame = cvQueryFrame (capture);
//			if (!frame.empty())
//			{
//				detectAndDisplay(frame);
//			}
//			else
//			{
//				std::cout << "(ERR) No captured frame...quitting" << std::endl;
//				break;
//			}
//		int c = waitKey(10);
//		if ( (char) c == 'c')
//			break;
//		}
//	}
//	return 0;
//}

int Frame::captureFrame(){

	CvCapture* capture;
	capture = cvCaptureFromCAM(-1);

	if (capture)
		return 1;
	return 0;
}

//void detectAndDisplay(Mat frame)
//{
//	std::vector<cv::Rect> faces, eyes;
//	cv::Rect rectangle;
//	Mat grayFrame;
//	cvtColor (frame, grayFrame, CV_BGR2GRAY);
//	equalizeHist (grayFrame, grayFrame);
//
//	// Detect faces
//	face_cascade.detectMultiScale(grayFrame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE,Size(30,30));
//
//	for (size_t i = 0; i < faces.size(); i++ )
//	{
//		// Find centroid for face ROI
//		Point center (faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height*0.5);
//		// Draw face ROI ellipse
//		ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 0, 255, 0 ), 1, 8, 0 );
//
//		Mat faceROI = grayFrame(faces[i]);
//		std::vector<Rect> eyes;
//
//		// Detect eyes per face
//		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
//
//		for (size_t j = 0; j < eyes.size(); j++)
//		{
//			// Find centroid for eyes ROI
//			Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
//			// Find radius for eyes ROI
//			int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//			// Draw eyes ROI circle
//			circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
//
////			rectangle = eyes[0] + cv::Point(faces[i].x, faces[i].y);
////			cv::rectangle(frame, rectangle, CV_RGB(0,0,255));
//		}
//	}
//	imshow("Eye detection", frame);
//
//}

