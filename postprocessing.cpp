#include "postprocessing.h"
#include <vector>
#include <algorithm> 
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <list>

cv::Mat filterMinMaxMed(cv::Mat image, int whatFilter)
{
	cv::Mat filtered(image.rows, image.cols, CV_8UC1);
	for (int x = 1; x < image.rows - 1; x++) {
		for (int y = 1; y < image.cols - 1; y++)
		{
			std::vector < uchar > filterScope;
			for (int boundX = -1; boundX < 2; boundX++)
			{
				for (int boundY = -1; boundY < 2; boundY++)
					filterScope.push_back(image.at<uchar>(x + boundX, y + boundY));
			}
			
			std::sort(filterScope.begin(), filterScope.end());
			switch (whatFilter) {
			case 0:
				filtered.at<uchar>(x, y) = filterScope.front();
				break;
			case 1:
				filtered.at<uchar>(x, y) = filterScope.back();
				break;
			case 2:
				filtered.at<uchar>(x, y) = filterScope.at(4);
				break;
			}
		}
	}
	return filtered;
}

cv::Mat findConturs(cv::Mat image){

	int filter[3][3] = { {0, -1, 0},{1, 5, -1},{0, -1, 0}};
	cv::Mat conturs(image.rows, image.cols, CV_8UC1);
	for (int x = 1; x < image.rows - 1; x++) {
		for (int y = 1; y < image.cols - 1; y++)
		{
			int filterValue = 0;
			for (int boundX = -1; boundX < 2; boundX++)
			{
				for (int boundY = -1; boundY < 2; boundY++)
					filterValue += filterValue + image.at<uchar>(x + boundX, y + boundY)*filter[boundX + 1][boundY + 1];
			}
		
			conturs.at<uchar>(x, y) = filterValue;
			if (conturs.at<uchar>(x, y) > 50 && conturs.at<uchar>(x, y) != 255)
				conturs.at<uchar>(x, y) = 255;
			else
				conturs.at<uchar>(x, y) = 0;
		}
	}
	return conturs;
}

cv::Mat findConturs(cv::Mat image, unsigned int x, unsigned int y)
{
	std::list<cv::Point2i> pointQueue;
	return cv::Mat();
}
