/*
 * DetectEyes.cpp
 *
 *  Created on: Sep 29, 2014
 *      Author: soheil
 */

#include "DetectEyes.h"




int main()
{

	CvCapture* capture;
	Mat frame;

	if( !face_cascade.load( face_cascade_name ) ){ std::cout << "--(!)Error loading\n"<<std::endl;; return -1; };
	if( !eyes_cascade.load( eyes_cascade_name ) ){ std::cout<< "--(!)Error loading\n"<<std::endl; return -1; };

	// Capture from camera
	capture = cvCaptureFromCAM(1);
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
	capture = cvCaptureFromCAM(1);

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
	cvtColor (frame, grayFrame, CV_BGR2GRAY);
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
//			// Find centroid for eyes ROI
//			Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
//			// Find radius for eyes ROI
//			int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//			// Draw eyes ROI circle
//			circle( frame, center, radius, Scalar( 255, 0, 0 ), 3, 8, 0 );


			rectangle = eyes[0] + cv::Point(faces[i].x, faces[i].y);
			frameROI = frame(rectangle);
			imshow("ROI",frameROI);
			houghTransform(frameROI);
			cv::rectangle(frame, rectangle, CV_RGB(0,255,0));

//			rectangle = eyes[1] + cv::Point(faces[i].x, faces[i].y);
//			cv::rectangle(frame, rectangle, CV_RGB(0,255,0));
		}
	}
//	imshow("Eye detection", frame);

	for (std::vector<Rect>::const_iterator i = eyes.begin(); i != eyes.end(); ++i)
		std::cout << *i << std::endl;



}

void basicThreshold(Mat frame)
{
    Mat imageGray;
	cv::cvtColor(frame, imageGray, CV_BGR2GRAY);

//	cv::GaussianBlur(imageGray, imageGray, Size(9,9), 2, 2);
imshow("gray", imageGray);
	threshold( imageGray, imageGray, 100, 150,0);
	imshow("threshold", imageGray);
}

int houghTransform(Mat frame)
{
//	  Mat image;
//	  char* imageName = "/home/soheil/Pictures/circle.jpg";
//	  image = imread( imageName, CV_LOAD_IMAGE_COLOR );
//
//	  if(!image.data)
//	    {
//	      std::cout << "No image data \n" << std::endl;
//	      return -1;
//	    }

	  Mat imageGray;
	  cv::cvtColor(frame, imageGray, CV_BGR2GRAY);

//	  cv::GaussianBlur(imageGray, imageGray, Size(9,9), 2, 2);

	  vector<Vec3f> circles;
	  HoughCircles( imageGray, circles, CV_HOUGH_GRADIENT, 1, imageGray.rows/8, 200, 100, 0, 0);

	  std::cout << circles.size() << std::endl;

	  for (int i=0; i < circles.size(); i++)
	  {
		  Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		  int radius = cvRound(circles[i][2]);
		  circle(frame, center, 3, Scalar(0,255,0), -1, 8 ,0);
		  circle(frame, center, radius, Scalar(0,0,255), 3, 8, 0);
	  }


	  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
	  imshow( "Display Image", frame);


	//  imshow( "Display Image", imageGray );
	//  imshow("Hough", cv::HoughCircles(image, houghImage))


	  return 0;
}

