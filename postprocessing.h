#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H
#include <opencv/cv.h>
#include <iostream>
#include <list>
#include "ihls.h"
#include "nhs.h"
class Postprocessing {
private:

	cv::Mat newImage;
	cv::Mat binaryImage;
	cv::Mat oldImage;
	cv::Mat conturs;

	void filterMinMaxMed(int whatFilter);
	void findConturs();
	void floodFill(unsigned int startX, unsigned int startY, cv::Scalar color);
public:
	Postprocessing(cv::Mat image);
	void process();
	
};
cv::Mat filterMinMaxMed(cv::Mat image, int whatFilter);
cv::Mat findConturs(cv::Mat image);
cv::Mat floodFill(cv::Mat image, unsigned int startX, unsigned int startY, cv::Scalar color);
#endif // !POST_PROCESSING_H
#pragma once
