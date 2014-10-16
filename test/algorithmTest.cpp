/*
 * algorithmTest.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: soheil
 */


#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../src/DetectEyes.h"


using namespace testing;

class BlinkDetection: public testing::Test{
public:
	Frame myFrame;
};


TEST_F(BlinkDetection, CameraCaptureWorks){
	ASSERT_THAT(myFrame.captureFrame(), Eq(1));
}
