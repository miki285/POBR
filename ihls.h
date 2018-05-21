#ifndef IHLS_H
#define IHLS_H

#include <opencv/cv.h>

inline double retrieve_normalised_hue(int R, int G, int B);


inline double retrieve_luminance(int R, int G, int B);

inline int retrieve_saturation(int R, int G, int B);

cv::Mat convert_rgb_to_ihls(cv::Mat input_image);

#endif /* IHLS_H_ */