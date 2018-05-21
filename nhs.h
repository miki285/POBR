#ifndef NHS_H_
#define NHS_H_

#include <opencv/cv.h>


#define R_HUE_MAX 15
#define R_HUE_MIN 240
//#define R_SAT_MIN 30
#define R_SAT_MIN 30

cv::Mat convert_ihls_to_nhs(cv::Mat ihls_image);

#endif /* NHS_H_ */