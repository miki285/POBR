#pragma once
#ifndef SPEED_Processing_H
#include "processing.h"
#include <opencv/cv.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class SpeedProcessing : public Processing {
public:


	bool containZero;
	bool containFive;
	bool containThree;
	bool containSeven;
	void process();
	bool checkIfSpeed();
	bool checkIfFive();
	bool checkIfThree();
	bool checkIfSeven();
	SpeedProcessing(cv::Mat image);
};
#endif