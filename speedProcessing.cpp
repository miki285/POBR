#include "speedProcessing.h"
#include "signProcessing.h"

void SpeedProcessing::process()
{
	//filterMinMaxMed(2);
	//cv::imshow("binaryCut", binaryImage);
	//cv::waitKey(-1);
	extractShapes();
	removeNoice();
	cv::imshow("floodFilledCut", newImage);
	cv::waitKey(-1);
}

SpeedProcessing::SpeedProcessing(cv::Mat image)
{
	oldImage = image.clone();
	conturs = cv::Mat(image.rows, image.cols, CV_8UC1);
	newImage = convert_rgb_to_ihls(oldImage);
	binaryImage = convert_ihls_to_nhs(newImage,1);
	containZero = false;
	containFive = false;
	containThree = false;
	containSeven = false;
}


bool SpeedProcessing::checkIfSpeed()
{
	cv::Scalar black = cv::Scalar(0, 0, 0);

	for (std::list<Shape>::iterator ite = shapeList.begin(); ite != shapeList.end(); ite++) {
		ite->analysisShape();
		if (ite->M3 > 2070 && ite->M2 < 0.03 && ite->M6<0 && ite->M7>0.021 && ite->M9< 0.03)
		{

			containZero = true;
			ite->color=(black);
			makeBlackShape(*ite);
		}
	}
	shapeList.remove(Shape(black));
	return containZero;
}

bool SpeedProcessing::checkIfFive()
{

	for (std::list<Shape>::iterator ite = shapeList.begin(); ite != shapeList.end(); ite++) {
		ite->analysisShape();
		if (ite->M1 > 0.37 && ite->M1 < 0.43 && ite->M2 > 0.04 && ite->M2 < 0.0594  && ite->M3 < 600 && ite->M8 < 0.3)
		{
			containFive = true;
			ite->color = (black);
			makeBlackShape(*ite);
		}

	}
	shapeList.remove(Shape(black));
	return containFive;
}

bool SpeedProcessing::checkIfThree()
{

	for (std::list<Shape>::iterator ite = shapeList.begin(); ite != shapeList.end(); ite++) {
		ite->analysisShape();

		if (ite->M1 > 0.2 && ite->M1<0.43 && ite->M2>0.005 && ite->M2 < 0.06  && ite->M3>1 && ite->M6 <150 && ite->M7>0.001 && ite->M7 < 0.0302 && ite->M8 < 0.412 && ite->M9 < 0.132 && ite->M8>-0.018)
		{
			containThree = true;
			ite->color = (black);
			makeBlackShape(*ite);
		}
	}
	shapeList.remove(Shape(black));
	return containThree;
}

bool SpeedProcessing::checkIfSeven()
{

	for (std::list<Shape>::iterator ite = shapeList.begin(); ite != shapeList.end(); ite++) {
		ite->analysisShape();
		if (ite->M1 < 0.51 && ite->M3 < 4600 && ite->M7 < 0.03 && ite->M6>-1434 && ite->M8 < 1.099 && ite->M8>-0.0052)
		{
			containSeven = true;
			ite->color = (black);
			makeBlackShape(*ite);
		}
	}
	shapeList.remove(Shape(black));
	return containSeven;
}