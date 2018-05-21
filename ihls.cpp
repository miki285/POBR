#include "ihls.h"
#include "math_mimax.h"

#include <cmath>

#include <iostream>

#define M_PI 3.14

double retrieve_theta(int R, int G, int B)
{
	double cosTheta = (R - (G * 0.5) - (B * 0.5)) / sqrtf((R * R) + (G * G) + (B * B) - (R * G) - (R * B) - (G * B));
	double theta = acos(cosTheta);

	return theta;
}

double retrieve_normalised_hue(int R, int G, int B)
{
	double hue;
	if (B <= G)
	{
		hue = retrieve_theta(R, G, B);
	}
	else
	{
		hue = (2 * M_PI) - retrieve_theta(R, G, B);
	}
	hue = hue * 255 / (2 * M_PI);
	return hue;
}

double retrieve_luminance(int R, int G, int B)
{
	return (0.210 * R) + (0.715 * G) + (0.072 * B);
}

int retrieve_saturation(int R, int G, int B)
{
	int max = maximum(R, G, B);
	int min = minimum(R, G, B);

	return (max - min);
}

cv::Mat convert_rgb_to_ihls(cv::Mat image)
{
	assert(image.channels() == 3);

	cv::Mat_<cv::Vec3b> ihls_image(image.rows, image.cols, CV_8UC3);
	cv::Mat_<cv::Vec3b> rgb_image = image;


	for (int x = 0; x < image.rows; ++x)
	{
		for (int y = 0; y < image.cols; ++y)
		{
			int b = rgb_image(x, y)[0];
			int g = rgb_image(x, y)[1];
			int r = rgb_image(x, y)[2];
			ihls_image(x,y)[0] = retrieve_saturation(r, g, b);
			ihls_image(x, y)[1] = retrieve_luminance(r, g, b);
			ihls_image(x, y)[2] = retrieve_normalised_hue(r, g, b);
		}
	}

	return ihls_image;
}