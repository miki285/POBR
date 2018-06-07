
#ifndef SIGN_Processing_H
#include "processing.h"
#include <opencv/cv.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "speedProcessing.h"
#include <algorithm>


class SignProcessing : public Processing {
public:

	bool isCircle(const Shape & shape);

	void checkIfShapeCircle();

	void process();

	SignProcessing(cv::Mat image);
	void CheckSpeed();
	void markSigns();;
};
#endif
#pragma once