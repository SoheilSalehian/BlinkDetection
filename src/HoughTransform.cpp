/*
 * DisplayImage.cpp
 *
 *  Created on: Aug 29, 2014
 *      Author: soheil
 */


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;

int main( int argc, char** argv )
{
  Mat image;
  char* imageName = "/home/soheil/Pictures/circle.jpg";
  image = imread( imageName, CV_LOAD_IMAGE_COLOR );

  if(!image.data)
    {
      std::cout << "No image data \n" << std::endl;
      return -1;
    }

  Mat imageGray;
  cv::cvtColor(image, imageGray, CV_BGR2GRAY);

  cv::GaussianBlur(imageGray, imageGray, Size(9,9), 2, 2);

  vector<Vec3f> circles;
  HoughCircles( imageGray, circles, CV_HOUGH_GRADIENT, 1, imageGray.rows/8, 200, 100, 100, 200 );

  std::cout << circles.size() << std::endl;

  for (int i=0; i < circles.size(); i++)
  {
	  Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	  int radius = cvRound(circles[i][2]);
	  circle(image, center, 3, Scalar(0,255,0), -1, 8 ,0);
	  circle(image, center, radius, Scalar(0,0,255), 3, 8, 0);
  }


  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
  imshow( "Display Image", image);

//  imshow( "Display Image", imageGray );
//  imshow("Hough", cv::HoughCircles(image, houghImage))

  waitKey(10000);

  return 0;
}



