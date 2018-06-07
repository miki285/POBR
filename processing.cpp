#include "processing.h"
#include <vector>
#include <algorithm> 
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "utils.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

Processing::Processing()
{
}

Processing::Processing(cv::Mat image)
{
	oldImage = image.clone();
	conturs = cv::Mat(image.rows, image.cols, CV_8UC1);
}

void Processing::filterMinMaxMed(int whatFilter)
{
	cv::Mat filtered(binaryImage.rows, binaryImage.cols, CV_8UC1);
	for (int x = 1; x < binaryImage.rows - 1; x++) {
		for (int y = 1; y < binaryImage.cols - 1; y++)
		{
			std::vector < uchar > filterScope;
			for (int boundX = -1; boundX < 2; boundX++)
			{
				for (int boundY = -1; boundY < 2; boundY++)
					filterScope.push_back(binaryImage.at<uchar>(x + boundX, y + boundY));
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
	binaryImage.release();
	binaryImage = filtered;
}

void Processing::findConturs()
{
	int filter[3][3] = { { 0, -1, 0 },{ 1, 5, -1 },{ 0, -1, 0 } };
	for (int x = 1; x < binaryImage.rows - 1; x++) {
		for (int y = 1; y < binaryImage.cols - 1; y++)
		{
			int filterValue = 0;
			for (int boundX = -1; boundX < 2; boundX++)
			{
				for (int boundY = -1; boundY < 2; boundY++)
					filterValue += filterValue + binaryImage.at<uchar>(x + boundX, y + boundY)*filter[boundX + 1][boundY + 1];
			}

			conturs.at<uchar>(x, y) = filterValue;
			if (conturs.at<uchar>(x, y) > 50 && conturs.at<uchar>(x, y) != 255)
				conturs.at<uchar>(x, y) = 255;
			else
				conturs.at<uchar>(x, y) = 0;
		}
	}
}

void Processing::extractShapes()
{
	cv::RNG rng(12345);
	cv::Scalar randomColor;
	newImage = copyImageBinaryToRGB(binaryImage);
	for (int x = 0; x<newImage.rows-1; x++ )
		for (int y = 0; y < newImage.cols-1; y++)
		{
			if (newImage.at<cv::Vec3b>(x, y)[0] == 255 && newImage.at<cv::Vec3b>(x, y)[1] == 255 && newImage.at<cv::Vec3b>(x, y)[2] == 255)
			{
				randomColor= cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				shapeList.push_back(Shape(randomColor));
				floodFill(x, y, randomColor);
			}
		}
}

void Processing::floodFill(unsigned int startX, unsigned int startY, cv::Scalar color)
{
	
	std::list<cv::Point2i> pointQueue;
	int iii = 0;
	pointQueue.push_back(cv::Point2i(startX, startY));
	unsigned int tempX, tempY;
	cv::Point2i left, right, up, down, current;
	while (!pointQueue.empty()) {
		current = pointQueue.front();
		pointQueue.pop_front();
		tempX = current.x;
		tempY = current.y;
		shapeList.back().points.push_back(current);
		newImage.at<cv::Vec3b>(tempX, tempY)[0] = color[0];
		newImage.at<cv::Vec3b>(tempX, tempY)[1] = color[1];
		newImage.at<cv::Vec3b>(tempX, tempY)[2] = color[2];


		left = cv::Point2i(tempX , tempY-1);
		right = cv::Point2i(tempX, tempY+1);
		up = cv::Point2i(tempX-1, tempY);
		down = cv::Point2i(tempX+1, tempY);

		if (left.y > 0 && newImage.at<cv::Vec3b>(left.x, left.y)[0] == 255 && std::find(pointQueue.begin(), pointQueue.end(), left) == pointQueue.end()) {
			pointQueue.push_back(left);
		}
		if (right.y < newImage.cols && newImage.at<cv::Vec3b>(right.x, right.y)[0] == 255 && std::find(pointQueue.begin(), pointQueue.end(), right) == pointQueue.end()) {
			pointQueue.push_back(right);
		}

		if (up.x > 0 && newImage.at<cv::Vec3b>(up.x, up.y)[0] == 255 && std::find(pointQueue.begin(), pointQueue.end(), up) == pointQueue.end()) {
			pointQueue.push_back(up);
		}
		if (down.x < newImage.rows && newImage.at<cv::Vec3b>(down.x, down.y)[0] == 255 && std::find(pointQueue.begin(), pointQueue.end(), down) == pointQueue.end()) {
			pointQueue.push_back(down);
		}
	}
}

void Processing::removeNoice()
{
	int x, y;
	for (std::list<Shape>::iterator ite = shapeList.begin(); ite != shapeList.end(); ite++) {
		if (ite->points.size() <50) {
			ite->color = black;
			makeBlackShape(*ite);
		}
	}
	shapeList.remove(Shape(black));
}

void Processing::makeBlackShape(Shape & shape)
{
	int x, y;
	for (std::list<cv::Point2i>::iterator itPoint = shape.points.begin(); itPoint != shape.points.end(); itPoint++)
	{
		x = itPoint->x;
		y = itPoint->y;
		newImage.at<cv::Vec3b>(x, y)[0] = 0;
		newImage.at<cv::Vec3b>(x, y)[1] = 0;
		newImage.at<cv::Vec3b>(x, y)[2] = 0;
	}
}


