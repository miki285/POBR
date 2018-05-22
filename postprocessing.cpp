#include "postprocessing.h"
#include <vector>
#include <algorithm> 
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "utils.h"


void Postprocessing::filterMinMaxMed(int whatFilter)
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
	binaryImage = filtered;
}

void Postprocessing::findConturs()
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

void Postprocessing::floodFill(unsigned int startX, unsigned int startY, cv::Scalar color)
{
	newImage = copyImageBinaryToRGB(binaryImage);
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

		newImage.at<cv::Vec3b>(tempX, tempY)[0] = color[0];
		newImage.at<cv::Vec3b>(tempX, tempY)[1] = color[1];
		newImage.at<cv::Vec3b>(tempX, tempY)[2] = color[2];
		cv::imshow("test", newImage);
		cv::waitKey(1);
		left = cv::Point2i(tempX - 1, tempY);

		right = cv::Point2i(tempX + 1, tempY);
		up = cv::Point2i(tempX, tempY + 1);
		down = cv::Point2i(tempX, tempY - 1);

		if (left.x >= 0 && newImage.at<cv::Vec3b>(left.x, left.y)[0] == 255 && std::find(pointQueue.begin(), pointQueue.end(), left) == pointQueue.end()) {
			pointQueue.push_back(left);
		}
		if (right.x < newImage.cols && newImage.at<cv::Vec3b>(right.x, right.y)[0] == 255 && std::find(pointQueue.begin(), pointQueue.end(), right) == pointQueue.end()) {
			pointQueue.push_back(right);
		}

		if (up.y >= 0 && newImage.at<cv::Vec3b>(up.x, up.y)[0] == 255 && std::find(pointQueue.begin(), pointQueue.end(), up) == pointQueue.end()) {
			pointQueue.push_back(up);
		}
		if (down.y < newImage.rows && newImage.at<cv::Vec3b>(down.x, down.y)[0] == 255 && std::find(pointQueue.begin(), pointQueue.end(), down) == pointQueue.end()) {
			pointQueue.push_back(down);
		}
	}
}

Postprocessing::Postprocessing(cv::Mat image)
{
	oldImage = image.clone();
	conturs= cv::Mat(image.rows, image.cols, CV_8UC1);
}

void Postprocessing::process()
{
	cv::Mat image_ihls = convert_rgb_to_ihls(oldImage);
	binaryImage = convert_ihls_to_nhs(image_ihls);
	cv::imshow("nhs", binaryImage);
	cv::waitKey(-1);
	findConturs();
	cv::imshow("conturs", conturs);
	cv::waitKey(-1);
	floodFill(oldImage.rows / 2, oldImage.cols / 2, cv::Scalar(100, 100, 100));
	cv::imshow("filled", newImage);
	cv::waitKey(-1);

}
