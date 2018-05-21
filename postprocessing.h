#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H
#include <opencv/cv.h>
cv::Mat filterMinMaxMed(cv::Mat image, int whatFilter);
cv::Mat findConturs(cv::Mat image);
cv::Mat findConturs(cv::Mat image, unsigned int x, unsigned int y);
#endif // !POST_PROCESSING_H
#pragma once
