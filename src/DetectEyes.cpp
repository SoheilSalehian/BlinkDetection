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


void detectAndDisplay(cv::Mat frame)
{
	std::vector<cv::Rect> faces, eyes;
	cv::Rect rectangle;
	cv::Mat frameROI;
	cv::Mat grayFrame;
	std::vector<Mat> rgb;
//	cvtColor(frame,frame,cv::COLOR_BGR2YUV);
	cv::split(frame, rgb);

	// Pick the green channel
	grayFrame = rgb[1];
	equalizeHist (grayFrame, grayFrame);
//	bitwise_not(grayFrame, grayFrame);

	imshow("original Image", grayFrame);
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
			imshow("ROI_COLOR", frameROI);
			bitwise_not(frameROI, frameROI);

			imshow("ROI",frameROI);

//			pixelNumberAnalysis(basicThreshold(frameROI));
//			houghTransform(basicThreshold(frameROI));
			eyeOpenOrClosed(showHistogram(basicThreshold(frameROI)), contourAnalysis(cannyThreshold(frameROI)));
//			momentAnalysis(cannyThreshold(frameROI));
			cv::rectangle(frame, rectangle, CV_RGB(0,255,0));
		}
	}
}



Mat cannyThreshold(Mat frame)
{
  Mat detectedEdges;
  Mat dst;
  /// Reduce noise with a kernel 3x3
  blur( frame, detectedEdges, Size(3,3) );

  /// Canny detector
  Canny( detectedEdges, detectedEdges, lowThreshold, maxThreshold, 3 , false);

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  frame.copyTo(dst, detectedEdges);
  cv::cvtColor(dst, dst, CV_BGR2GRAY);
  imshow("canny", dst);

  return dst;
}


void momentAnalysis(Mat edgeDetectorOutput)
{
	Mat resultMoment;
	const Moments& moment = moments(edgeDetectorOutput);
	HuMoments(moment, resultMoment);
	imshow("moment", resultMoment);
}

// comparison function object
bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i < j );
}

int pixelNumberAnalysis(Mat frame)
{
	int whitePixel = cv::countNonZero(frame);
	std::cout << whitePixel << std::endl;
	return whitePixel;
}

int contourAnalysis(Mat edgeDetectorOutput)
{
	edgeDetectorOutput.convertTo(edgeDetectorOutput, CV_8UC1);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours( edgeDetectorOutput, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


	Mat drawing = Mat::zeros( edgeDetectorOutput.size(), CV_8UC3);
	int numberOfBoxes = 0;


	for( int i = 0; i< contours.size(); i++ )
	{
        size_t count = contours[i].size();
        if( count < 6 or contourArea(contours[i]) <= 10)
            continue;
		Scalar color = Scalar( 255, 255, 255);
        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);

        if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 )
        	continue;
        drawContours(drawing, contours, (int)i, Scalar::all(255), 1, 8);

        ellipse(drawing, box, Scalar(0,0,255), 1, CV_AA);
        ellipse(drawing, box.center, box.size*0.5f, box.angle, 0, 360, Scalar(0,255,255), 1, CV_AA);
        Point2f vtx[4];
        box.points(vtx);
        for( int j = 0; j < 4; j++ )
        	line(drawing, vtx[j], vtx[(j+1)%4], Scalar(0,255,0), 1, CV_AA);

        numberOfBoxes++;

	}

//	std::cout << numberOfBoxes << std::endl;
	// Show in a window
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );


	if(numberOfBoxes <= 1)
		return 1;
//		std::cout << numberOfBoxes << "..............Closed..." << std::endl;
	else
		return 0;
//		std::cout << numberOfBoxes << "...Open..." << std::endl;
}



int houghTransform(Mat frame)
{
	Mat grayFrame;
	grayFrame = frame;
//	cvtColor(frame, grayFrame, CV_BGR2GRAY);
//	grayFrame = frame;

	vector<Vec3f> circles;
	HoughCircles(grayFrame, circles, CV_HOUGH_GRADIENT, 1, grayFrame.rows/2, 40, 20, 10, 20);

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

Mat basicThreshold(Mat frame)
{
    Mat imageGray;
	cv::cvtColor(frame, imageGray, CV_BGR2GRAY);

//	cv::GaussianBlur(imageGray, imageGray, Size(9,9), 2, 2);
	imshow("gray", imageGray);
//	threshold(imageGray, frame, 16, 255, CV_THRESH_BINARY);
	threshold( imageGray, imageGray, 180, 255, CV_THRESH_BINARY);

	imshow("threshold", imageGray);
	return imageGray;
}

int showHistogram(Mat frame)
{
	  Mat src, dst;
	  bool result;
	  /// Load image
	  src = frame;
	  imshow("pre-histogram", src);
	  if( !src.data )
	    { return 0; }

	  /// Separate the image in 3 places ( B, G and R )
	  vector<Mat> bgr_planes;
	  split( src, bgr_planes );

	  /// Establish the number of bins
	  int histSize = 256;

	  /// Set the ranges ( for B,G,R) )
	  float range[] = { 0, 256 } ;
	  const float* histRange = { range };

	  bool uniform = true; bool accumulate = true;

	  Mat b_hist, g_hist, r_hist;

	  /// Compute the histograms:
	  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );

	  // Draw the histograms for B, G and R
	  int hist_w = 512; int hist_h = 400;
	  int bin_w = cvRound( (double) hist_w/histSize );

	  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	  /// Normalize the result to [ 0, histImage.rows ]
	  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );


	  /// Draw for each channel
	  for( int i = 1; i < histSize; i++ )
	  {
	      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
	                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
	                       Scalar( 255, 255, 255), 2, 8, 0  );
	  }

	  /// Display
	  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
	  imshow("calcHist Demo", histImage );
	  float binVal = b_hist.at<float>(255);
//	  std::cout << binVal << "..vs.." << previousHistBin << std::endl;
	  if (binVal <= previousHistBin-10)
	  {
//		  std::cout << "closed" << std::endl;
		  result = true;
	  }
	  else
		  result = false;

	previousHistBin = binVal;


	return result;
}




void MatchingMethod(Mat frame)
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

bool eyeOpenOrClosed(int histogramResults, int contourResults)
{
	if(histogramResults and contourResults)
	{
		std::cout << "........Closed......" << std::endl;
		return true;
	}
	else
	{
		std::cout << "...Open...." << std::endl;
		return false;
	}
}

