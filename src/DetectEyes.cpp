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

void detectAndDisplay(cv::Mat frame)
{
	std::vector<cv::Rect> faces, eyes;
	cv::Rect rectangle;
	cv::Mat frameROI;
	cv::Mat grayFrame;
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


//			houghTransform(cannyThreshold(frameROI));
//			showHistogram(frameROI);

			contourAnalysis(cannyThreshold(frameROI));
			cv::rectangle(frame, rectangle, CV_RGB(0,255,0));
		}
	}

//	for (std::vector<Rect>::const_iterator i = eyes.begin(); i != eyes.end(); ++i)
//		std::cout << *i << std::endl;
}


Mat cannyThreshold(Mat frame)
{
  Mat detectedEdges;
  Mat dst;
  /// Reduce noise with a kernel 3x3
  blur( frame, detectedEdges, Size(3,3) );

  /// Canny detector
  Canny( detectedEdges, detectedEdges, lowThreshold, maxThreshold, 3 );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  frame.copyTo(dst, detectedEdges);
  imshow("canny", dst);
  return detectedEdges;
}

void contourAnalysis(Mat edgeDetectorOutput)
{
//	edgeDetectorOutput.convertTo(edgeDetectorOutput, CV_8UC1);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours( edgeDetectorOutput, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Draw contours
	Mat drawing = Mat::zeros( edgeDetectorOutput.size(), CV_8UC3 );

	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( 255, 0, 0);
	    drawContours( drawing, contours, i, 255, 1, 8, hierarchy, 0, Point() );

	}

	// Show in a window
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );
	if(hierarchy.size() <= contourThreshold)
		std::cout << hierarchy.size() << "...Closed..." << std::endl;
	else
		std::cout << hierarchy.size() << "...Open..." << std::endl;

}


int houghTransform(Mat frame)
{
	Mat grayFrame;
	cvtColor(frame, grayFrame, CV_BGR2GRAY);

	vector<Vec3f> circles;
	HoughCircles(grayFrame, circles, CV_HOUGH_GRADIENT, 1, grayFrame.rows/4, 20, 10, 3, 5);

	std::cout << circles.size() << std::endl;

	for (int i=0; i < (int)circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle(frame, center, 3, Scalar(0,255,0), -1, 8 ,0);
		circle(frame, center, radius, Scalar(0,0,255), 3, 8, 0);
	}


	namedWindow( "Hough Transform", CV_WINDOW_AUTOSIZE );
	imshow( "Hough Transform", frame);

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

	imshow("threshold", imageGray);
}

void showHistogram(Mat frame)
{
	// Initialization for the calculation section
	MatND hist;
	int histSize = 256;
	float range[] = {0,256};
	const float* histRange = {range};
	bool uniform = true;
	bool accumulate = false;

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	calcHist(&frame, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0,0,0));
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
						 Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
						 Scalar( 255, 0, 0), 2, 8, 0  );
	}

	/// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
	imshow("calcHist Demo", histImage );

	waitKey(0);

	return;
}

//int main()
//{
//
//	frame = imread("/home/soheil/Pictures/Webcam/20.jpg");
//	myTemplate = imread("/home/soheil/Pictures/Webcam/templateClosed.jpg");
//
//
//
//	namedWindow("image window", CV_WINDOW_AUTOSIZE);
//	namedWindow("result window", CV_WINDOW_AUTOSIZE);
//
//	char* trackbarLabel = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
//	createTrackbar( trackbarLabel, "image window", &matchMethod, maxTrackbar, MatchingMethod );
//
//	MatchingMethod(0,0);
//
//	return 0;
//}

void MatchingMethod(int, void*)
{

	Mat imageDisplay;
	frame.copyTo(imageDisplay);

	int resultColumns = frame.cols - myTemplate.cols + 1;
	int resultRows = frame.rows - myTemplate.rows + 1;

	result.create(resultColumns, resultRows, CV_32FC1);

	// Do the matching and normalization
	matchTemplate(frame, myTemplate, result, matchMethod);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal, maxVal;
	Point minLoc, maxLoc;
	Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	if (matchMethod == CV_TM_SQDIFF || matchMethod == CV_TM_SQDIFF_NORMED)
		matchLoc = minLoc;
	else
		matchLoc = maxLoc;

	rectangle( imageDisplay, matchLoc, Point( matchLoc.x + myTemplate.cols , matchLoc.y + myTemplate.rows ), Scalar::all(0), 2, 8, 0 );
	rectangle( result, matchLoc, Point( matchLoc.x + myTemplate.cols , matchLoc.y + myTemplate.rows ), Scalar::all(0), 2, 8, 0 );

	imshow("Template Matching", imageDisplay);
	imshow("Results of Template", result);
}

