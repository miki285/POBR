#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include <opencv/cv.h>
#include<list>
class Shape {
private:

	double area;

	

	double m01, m10, m02, m20, m03, m30, m11, m21, m12;

	double M01, M10, M02, M20, M03, M30, M11, M21, M12;
	
	double center_x, center_y;

	double  countGeometricMoments(int p, int q);
public:

	std::list<cv::Point2i> points;

	cv::Scalar color;

	double M1, M2, M3, M4 , M6 , M7, M8 , M9;

	Shape();
	Shape(cv::Scalar);

	void analysisShape();
	bool operator==(const Shape& q);
};

#endif // !SHAPE_H
