#include "utils.h"

cv::Mat copyImageBinaryToRGB(cv::Mat image)
{
	cv::Mat copied_image(image.rows, image.cols, CV_8UC3);
	for (int x = 0; x<image.rows; x++)
		for (int y = 0; y < image.cols; y++)
		{
			copied_image.at<cv::Vec3b>(x, y)[0] = image.at<uchar>(x, y);
			copied_image.at<cv::Vec3b>(x, y)[1] = image.at<uchar>(x, y);
			copied_image.at<cv::Vec3b>(x, y)[2] = image.at<uchar>(x, y);
		}
	return copied_image;
}
