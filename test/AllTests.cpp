/*
 * AllTests.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: soheil
 */



#include "gmock/gmock.h"
#include "gtest/gtest.h"

int main(int argc, char** argv) {
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
