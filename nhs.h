#ifndef NHS_H_
#define NHS_H_

#include <opencv/cv.h>


#define R_HUE_MAX 15
#define R_HUE_MIN 240
//#define R_SAT_MIN 30
#define R_SAT_MIN 30


#define BL_LUM_MAX 100
#define BL_SAT_MAX 50



cv::Mat convert_ihls_to_nhs(cv::Mat ihls_image,  int typeFiltering);

#endif /* NHS_H_ */