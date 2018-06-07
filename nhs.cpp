/*
* nhs.cpp
*/
#include "nhs.h"
#include <cmath>


cv::Mat convert_ihls_to_nhs(cv::Mat image, int typeFiltering)
{
	assert(image.channels() == 3);
	cv::Mat_<cv::Vec3b> ihls_image = image;
	cv::Mat nhs_image(ihls_image.rows, ihls_image.cols, CV_8UC1);
	int hue, sat, lum;
	switch (typeFiltering)
	{
	case 0:
		
		for (int x = 0; x < ihls_image.rows; ++x) {
			for (int y = 0; y < ihls_image.cols; ++y)
			{
				sat = ihls_image(x, y)[0];
				lum = ihls_image(x, y)[1];
				hue = ihls_image(x, y)[2];
				if ((hue<R_HUE_MAX || hue>R_HUE_MIN) && sat > R_SAT_MIN)
					nhs_image.at<uchar>(x, y) = 255;
				else
					nhs_image.at<uchar>(x, y) = 0;
			}
		}
		break;
	case 1:
		for (int x = 0; x < ihls_image.rows; ++x) {
			for (int y = 0; y < ihls_image.cols; ++y)
			{
				sat = ihls_image(x, y)[0];
				lum = ihls_image(x, y)[1];
				hue = ihls_image(x, y)[2];
				if (lum < BL_LUM_MAX && sat < BL_SAT_MAX)
					nhs_image.at<uchar>(x, y) = 255;
				else
					nhs_image.at<uchar>(x, y) = 0;
			}
		}
			break;
	default:
		break;
	}


	return nhs_image;
}
