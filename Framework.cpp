
#include "opencv2/highgui/highgui.hpp"
#include<opencv2\core\core.hpp>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <algorithm>
#include "ihls.h"
#include "nhs.h"
#include "signProcessing.h"
#include "utils.h"


int main(int, char *[]) {
    std::cout << "Start ..." << std::endl;
	for (int i = 2; i <= 31; i++) {
		std::string name = "d (" + std::to_string(i) + ").jpg";
		cv::Mat image = cv::imread("a (19).jpg");
		cv::Mat_<cv::Vec3b> _I = image;

		SignProcessing post = SignProcessing(image);
		post.process();
		cv::destroyAllWindows();
	}
 

    return 0;
}
