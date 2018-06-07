#pragma once
#ifndef SIGN_Processing_H
#include <opencv/cv.h>
#include <iostream>
#include <list>
#include "ihls.h"
#include "nhs.h"
#include "shape.h"

class Processing {
protected:
	cv::Scalar black = cv::Scalar(0, 0, 0);
	std::list<Shape> shapeList;
	cv::Mat newImage;
	cv::Mat binaryImage;
	cv::Mat oldImage;
	cv::Mat conturs;
	void filterMinMaxMed(int whatFilter);
	void findConturs();
	void extractShapes();
	void floodFill(unsigned int startX, unsigned int startY, cv::Scalar color);
public:
	Processing();
	Processing(cv::Mat image);
	virtual void process() = 0 ;

	void removeNoice();

	void makeBlackShape(Shape & shape);


};
#endif // !SIGN_Processing_H
