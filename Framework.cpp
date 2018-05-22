
#include "opencv2/highgui/highgui.hpp"
#include<opencv2\core\core.hpp>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <algorithm>
#include "ihls.h"
#include "nhs.h"
#include "postprocessing.h"
#include "utils.h"

#define bright 50
#define cont 100
#define PI         3.141592653589793238462643383279502884L /* pi */
int count = 0;
class Ksztalt{
	cv::Mat image;
	cv::String name;
	long obwod;
	long pole;
	double malinowska;
	double M1;
	double M7;
	double M20;
	double M02;
	double M11;
	int srX;
	int srY;
	long m20, m10, m02, m01, m11;
	float kat;
	int srObrazuX;
	int srObrazuY;
public:
	Ksztalt(cv::Mat &zdjecie, cv::String nazwa, int srXi, int srYi){
		name = nazwa;
		image = zdjecie;		
		srX = srXi;
		srY = srYi;
		getResult2();
	}
	Ksztalt(cv::String nazwaPliku){
		image = cv::imread(nazwaPliku);
		name = nazwaPliku;

	}
	void getResult1(){
		liczPole();
		liczObwod();
		liczMalinowska();
		liczMomentyZwykle();
		liczMomentyCentralne();
		liczNiezmiennik();
		std::cout << "Plik " << name << ": S=" << pole << " L=" << obwod << " W3=" << malinowska << " M1=" << M1 << " M7=" << M7 << std::endl;
	}
	void getResult2() {
		liczPole();
		liczObwod();
		liczMalinowska();
		liczMomentyZwykle();
		liczMomentyCentralne();
		liczNiezmiennik();
		liczKat();
		pokazZnaczki();
		std::cout << "Strzalka " << name <<": kat= "<<kat << "  S=" << pole << " L=" << obwod << " W3=" << malinowska << " M1=" << M1 << " M7=" << M7 << std::endl;
	}
	void liczPole(){
		cv::Mat_<cv::Vec3b> _I = image;
		pole = 0;
		for (int x = 0; x < image.rows; x++){
			for (int y = 0; y < image.cols; y++){
				if (_I(x, y)[0] != 255)
					pole++;
			}
		}
	}
	void pokazZnaczki() {
		cv::Mat_<cv::Vec3b> _I = image;
		_I(srX, srY)[0] = 0;
		_I(srX, srY)[1] = 0;
		_I(srX, srY)[2] = 0;
		_I(srObrazuX, srObrazuY)[0] = 255;
		_I(srObrazuX, srObrazuY)[1] = 255;
		_I(srObrazuX, srObrazuY)[2] = 255;
		std::string fileName = "obrazek" + std::to_string(count)+".png";
		cv::imwrite(fileName, _I);
		cv::waitKey(-1);
		count++;
	}
	void liczObwod(){
		cv::Mat_<cv::Vec3b> _I = image;
		obwod = 0;
		for (int x = 1; x < image.rows-1; x++){
			for (int y = 1; y < image.cols-1; y++){
				if (_I(x, y)[0] != 255 &&
					(_I(x - 1, y - 1)[0] == 255 || _I(x, y - 1)[0] == 255 || _I(x - 1, y)[0] == 255 ||
					_I(x + 1, y + 1)[0] == 255 || _I(x, y + 1)[0] == 255 || _I(x + 1, y)[0] == 255 ||
					_I(x + 1, y - 1)[0] == 255 || _I(x - 1, y + 1)[0] == 255))
					obwod++;
			}
		}
	}
	void liczMalinowska(){
		malinowska = (obwod / (2 * sqrt(pole*PI))) - 1;
	}
	void liczMomentyZwykle(){
		cv::Mat_<cv::Vec3b> _I = image;
		m20 = m10 = m02= m01=m11 = 0;
		for (int x = 0; x < image.rows; x++){
			for (int y = 0; y < image.cols; y++){
				if (_I(x, y)[0] != 255){
					m20 += pow(x, 2);
					m02 += pow(y, 2);
					m10 += x;
					m01 += y;
					m11 += x * y;
				}
			}
		}
	}
	void liczMomentyCentralne(){
		M20 = m20 - pow(m10, 2) / pole;
		M02 = m02 - pow(m01, 2) / pole;
		M11 = (double)m11 - (double)m10 * (double)m01 / (double)pole;
	}

	void liczNiezmiennik(){
		M1 = (M20 + M02) / pow(pole, 2);
		M7 = (M20*M02 - pow(M11, 2)) / pow(pole, 4);
	}
	long getPole(){
		return pole;
	}

	void liczKat() {
		srObrazuX= m10 / pole;
		srObrazuY = m01 / pole;
		kat = atan2(srX - srObrazuX, srY - srObrazuY)*180/PI;
	}

};



cv::Mat& perform(cv::Mat& I, int mode){
  CV_Assert(I.depth() != sizeof(uchar));
  switch(I.channels())  {
  case 1:
    for( int i = 0; i < I.rows; ++i)
        for( int j = 0; j < I.cols; ++j )
            I.at<uchar>(i,j) = (I.at<uchar>(i,j)/32)*32;
    break;
  case 3:
	  cv::Mat_<cv::Vec3b> _I = I;
	  switch (mode){
	  case 1:{		  
		  for (int i = 0; i < I.rows; ++i)
			  for (int j = 0; j < I.cols; ++j){
			  int gray=0;
			  gray += _I(i, j)[0];
			  gray += _I(i, j)[1];
			  gray += _I(i, j)[2];
			  gray = gray / 3;
			  _I(i, j)[0] = std::max(0, -gray + _I(i, j)[2]);
			  _I(i, j)[1] = std::max(0, -gray + _I(i, j)[2]);
			  _I(i, j)[2] = std::max(0, -gray + _I(i, j)[2]);

			  }
		  I = _I;
		  break; 
	  }
	  case 2:{
		  for (int i = 0; i < I.rows; ++i)
			  for (int j = 0; j < I.cols; ++j){
				_I(i, j)[0] = std::min(255, _I(i, j)[0] + bright);
				_I(i, j)[1] = std::min(255, _I(i, j)[1] + bright);
				_I(i, j)[2] = std::min(255, _I(i, j)[2] + bright);
			  }
		  break;
	  }
	  case 3:{
		  for (int i = 0; i < I.rows; ++i)
			  for (int j = 0; j < I.cols; ++j){
			  _I(i, j)[0] = std::min(255, _I(i, j)[0] * (1 + cont / 100));
			  _I(i, j)[1] = std::min(255, _I(i, j)[1] * (1 + cont / 100));
			  _I(i, j)[2] = std::min(255, _I(i, j)[2] * (1 + cont / 100));
			  }
		  break;
	  }
	  case 4:{
		  int tab[8] = { 0 };
			 // std::cout<<tab[0]<<std::endl;
		  for (int i = 0; i < I.rows; ++i)
			  for (int j = 0; j < I.cols; ++j){
			  int gray = 0;
			  gray += _I(i, j)[0];
			  gray += _I(i, j)[1];
			  gray += _I(i, j)[2];
			  gray = gray / 3;
			  if (gray < 31)
				  tab[0]++;
			  else if (gray < 63)
				  tab[1]++;
			  else if (gray < 95)
				  tab[2]++;
			  else if (gray < 127)
				  tab[3]++;
			  else if (gray < 159)
				  tab[4]++;
			  else if (gray < 191)
				  tab[5]++;
			  else if (gray < 224)
				  tab[6]++;
			  else
				  tab[7]++;


			  }
		  for (int i = 0; i < 8; i++)
		  {
			  std::cout << i + 1 << ": " << tab[i] << std::endl;
		  }
		  break;
	  }
	  }
    break;
  }
  return I;
}

cv::Mat selectMax(cv::Mat& I){
    CV_Assert(I.depth() != sizeof(uchar));
    cv::Mat  res(I.rows,I.cols, CV_8UC3);
    switch(I.channels())  {
    case 3:
        cv::Mat_<cv::Vec3b> _I = I;
        cv::Mat_<cv::Vec3b> _R = res;
        for( int i = 0; i < I.rows; ++i)
            for( int j = 0; j < I.cols; ++j ){
                int sel = (_I(i,j)[0] < _I(i,j)[1])?1:0;
                sel = _I(i,j)[sel] < _I(i,j)[2]?2:sel;
                _R(i,j)[0] = sel==0?255:0;
                _R(i,j)[1] = sel==1?255:0;
                _R(i,j)[2] = sel==2?255:0;
            }
        res = _R;
        break;
    }
    return res;
}

std::vector<Ksztalt> znajdzStrzalki(const cv::String &nazwaZdjecia){
	cv::Mat image = cv::imread(nazwaZdjecia);
	cv::Mat_<cv::Vec3b> _I = image;
	std::vector<Ksztalt> zdjecia;
	int minX1, maxX1, minY1, maxY1;
	int minX2, maxX2, minY2, maxY2;
	int minX3, maxX3, minY3, maxY3;
	int minX4, maxX4, minY4, maxY4;
	int minX5, maxX5, minY5, maxY5;
	minX1 = minX2 = minX3 = minX4 = minX5 = minY1 = minY2 = minY2 = minY3 = minY4 = minY5 = image.cols;
	maxX1 = maxX2 = maxX3 = maxX4 = maxX5 = maxY1 = maxY2 = maxY3 = maxY4 = maxY5 = 0;
	for (int x = 0; x < _I.cols; x++)
		for (int y = 0; y < _I.rows; y++){
		if (_I(x, y)[2] == 0){
			minX1 = std::min(minX1, x);
			maxX1 = std::max(maxX1, x);
			minY1 = std::min(minY1, y);			
			maxY1 = std::max(maxY1, y);
		}
		else if (_I(x, y)[2] == 45){
			minX2 = std::min(minX2, x);
			minY2 = std::min(minY2, y);
			maxX2 = std::max(maxX2, x);
			maxY2 = std::max(maxY2, y);
		}
		else if (_I(x, y)[2] == 90){
			minX3 = std::min(minX3, x);
			minY3 = std::min(minY3, y);
			maxX3 = std::max(maxX3, x);
			maxY3 = std::max(maxY3, y);
		}
		else if (_I(x, y)[2] == 135){
			minX4 = std::min(minX4, x);
			minY4 = std::min(minY4, y);
			maxX4 = std::max(maxX4, x);
			maxY4 = std::max(maxY4, y);
		}
		else if (_I(x, y)[2] == 180){
			minX5 = std::min(minX5, x);
			minY5 = std::min(minY5, y);
			maxX5 = std::max(maxX5, x);
			maxY5 = std::max(maxY5, y);
		}

		}
	cv::Mat r1 = image(cv::Rect(minY1, minX1, maxY1 - minY1, maxX1 - minX1));
	cv::Mat r2 = image(cv::Rect(minY2, minX2, maxY2 - minY2, maxX2 - minX2));
	cv::Mat r3 = image(cv::Rect(minY3, minX3, maxY3 - minY3, maxX3 - minX3));
	cv::Mat r4 = image(cv::Rect(minY4, minX4, maxY4 - minY4, maxX4 - minX4));
	cv::Mat r5 = image(cv::Rect(minY5, minX5, maxY5 - minY5, maxX5 - minX5));
	
	cv::waitKey(-1);
	zdjecia.push_back(Ksztalt(r1, "r1", r1.rows / 2, r1.cols / 2));
	zdjecia.push_back(Ksztalt(r2, "r2", r2.rows / 2, r2.cols / 2));
	zdjecia.push_back(Ksztalt(r3, "r3", r3.rows / 2, r3.cols / 2));
	zdjecia.push_back(Ksztalt(r4, "r4", r4.rows / 2, r4.cols / 2));
	zdjecia.push_back(Ksztalt(r5, "r5", r5.rows / 2, r5.cols / 2));
	return zdjecia;
}

int main(int, char *[]) {
    std::cout << "Start ..." << std::endl;
    cv::Mat image = cv::imread("czer.png");
	cv::Mat_<cv::Vec3b> _I = image;

	Postprocessing post = Postprocessing(image);
	post.process();

	cv::Mat image_ihls = convert_rgb_to_ihls(image);
	cv::Mat image_nhs = convert_ihls_to_nhs(image_ihls);
	//cv::Mat filteredMin = filterMinMaxMed(image_nhs, 0);
	//cv::Mat filteredMax = filterMinMaxMed(filteredMin, 1);
	//cv::Mat filteredMed2 = filterMinMaxMed(filteredMin, 2);
	cv::Mat RGB_image = copyImageBinaryToRGB(image_nhs);

	for (int startX = 0; startX < image_nhs.rows; ++startX) {
		for (int startY = 0; startY < image_nhs.cols; ++startY)
		{
			if (image_nhs.at<uchar>(startX, startY) == 255) {
				floodFill(image_nhs, startX, startY, cv::Scalar(100, 100, 100));
				break;
			}
		}
	}
	cv::imshow("conturs", RGB_image);
	cv::waitKey(-1);

	cv::imwrite("flooded.jpg", image);
	cv::Mat conturs = findConturs(image);
	cv::imshow("conturs", conturs);

	znajdzStrzalki("strzalki_1.dib");

	znajdzStrzalki("strzalki_2.dib");



	cv::Mat grey;
	cv::imshow("elipsa", image);
	cv::waitKey(-1);
	int rowsHalf = image.rows/2;
	int colsHalf = image.cols/2;

    cv::imwrite("result.png",image);

    return 0;
}
