#include "signProcessing.h"


bool SignProcessing::isCircle(const Shape & shape)
{
	bool result = false;
	if (shape.M1 > 0.35 && shape.M1 < 0.82 && shape.M2>0 && shape.M2 < 0.01 && shape.M7>0.04 && shape.M7 < 0.2)
		result = true;
	return result;
}

void SignProcessing::checkIfShapeCircle()
{
	cv::Scalar black = cv::Scalar(0, 0, 0);

	for (std::list<Shape>::iterator ite = shapeList.begin(); ite != shapeList.end(); ite++) {
		ite->analysisShape();
		if (!isCircle(*ite)) {
			makeBlackShape(*ite);
			ite->color = black;
		}
			
	}
	shapeList.remove(Shape(black));

}

void SignProcessing::process()
{
	//filterMinMaxMed(1);
	//filterMinMaxMed(0);
	//filterMinMaxMed(2);
	std::cout << "Obraz wyfiltrowany w przestrzeni IHLS dla koloru czerwonego\n";
	cv::imshow("filteredEntryImage", binaryImage);
	cv::waitKey(-1);
	std::cout << "Obraz z ksztaltami wypelnionymi algorytmem floodfill\n";
	extractShapes();
	cv::imshow("extractedShapes", newImage);
	cv::waitKey(-1);

	
	removeNoice();
	cv::imshow("onlyShapesBiggerThan", newImage);
	std::cout << "Obraz z ksztaltami wiekszymi niz 50 pikseli\n";
	cv::waitKey(-1);


	checkIfShapeCircle();
	cv::imshow("onlySigns", newImage);
	std::cout << "Obraz z ksztaltami ktorych momenty odpowiadaja znakom ograniczenia predkosci\n";
	cv::waitKey(-1);
	std::cout << "Teraz program przechodzi w analize poszczegolnych znakow drogowych i ich zawartosci\n";
	CheckSpeed();
	markSigns();
	cv::imshow("Result", oldImage);
	cv::waitKey(-1);

}

SignProcessing::SignProcessing(cv::Mat image)
{
	oldImage = image.clone();
	conturs = cv::Mat(image.rows, image.cols, CV_8UC1);
	newImage = convert_rgb_to_ihls(oldImage);
	binaryImage = convert_ihls_to_nhs(newImage, 0);
}

void SignProcessing::CheckSpeed() {
	int minX1, maxX1, minY1, maxY1;
	cv::Scalar color;
	for (std::list<Shape>::iterator ite = shapeList.begin(); ite != shapeList.end(); ite++) {
		 color = ite->color;
		 minX1 = minY1 = newImage.cols;
		 maxX1 = maxY1 = 0;
		 for (std::list<cv::Point2i>::iterator pointIte = ite->points.begin(); pointIte != ite->points.end(); pointIte++) {
			 minX1 = std::min(minX1, pointIte->x);
			 maxX1 = std::max(maxX1, pointIte->x);
			 minY1 = std::min(minY1, pointIte->y);
			 maxY1 = std::max(maxY1, pointIte->y);
		 }
		 cv::Mat cropped = (oldImage((cv::Rect(minY1, minX1, maxY1 - minY1, maxX1 - minX1))));
		 SpeedProcessing speed = SpeedProcessing(cropped);
		 speed.process();

		 if (!speed.checkIfSpeed()) {
			 makeBlackShape(*ite);
			 ite->color = black;
		 }
		 else if(speed.checkIfFive()) {
			 ite->color = cv::Scalar(0, 0, 255);
		 }
		 else if (speed.checkIfThree()) {
			 ite->color = cv::Scalar(120,0, 250);
		 }
		 else if(speed.checkIfSeven()){
			 ite->color = cv::Scalar(200, 200,200);
		 }
		 else {
			 ite->color = cv::Scalar(0, 255, 0);
		 }
		 cropped.release();
	}
	shapeList.remove(Shape(black));
}

void SignProcessing::markSigns() {
	
	for (std::list<Shape>::iterator ite = shapeList.begin(); ite != shapeList.end(); ite++) {
		for (std::list<cv::Point2i>::iterator pointIte = ite->points.begin(); pointIte != ite->points.end(); pointIte++) {
			oldImage.at<cv::Vec3b>(pointIte->x, pointIte->y)[0] = ite->color[0];
			oldImage.at<cv::Vec3b>(pointIte->x, pointIte->y)[1] = ite->color[1];
			oldImage.at<cv::Vec3b>(pointIte->x, pointIte->y)[2] = ite->color[2];
		}
	}
	cv::imwrite("result.jpg", oldImage);
	

}
