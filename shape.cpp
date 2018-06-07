#include "shape.h"



Shape::Shape()
{
	m01 = m10 = m02 = m20 = m03 = m30 = m11 = m21 = m12 = 0;

	M01 = M10 = M02 = M20 = M03 = M30 = M11 = M21 = M12 = 0;

	M1 = M2 = M3 = M4 = M6 = M7 = M8 = M9 = 0;

	color = cv::Scalar(255, 255, 255);
}

Shape::Shape(cv::Scalar givenColor)
{
	m01 = m10 = m02 = m20 = m03 = m30 = m11 = m21 = m12 = 0;

	M01 = M10 = M11 = M02 = M20 = M03 = M30  = M21 = M12 = 0;

	M1 = M2 = M3 = M4 = M6 = M7 = M8 = M9 = 0;

	color = givenColor;
}

void Shape::analysisShape()
{
	m01 = countGeometricMoments(0, 1);
	m10 = countGeometricMoments(1, 0);
	m02 = countGeometricMoments(0, 2);
	m20 = countGeometricMoments(2, 0);
	m03 = countGeometricMoments(0, 3);
	m30 = countGeometricMoments(3, 0);
	m11 = countGeometricMoments(1, 1);
	m21 = countGeometricMoments(2, 1);
	m12 = countGeometricMoments(1, 2);
	area = countGeometricMoments(0, 0);

	center_x = m10 / area;
	center_y = m01 / area;

	M01 = m01 - (m01 / area) * area;
	M10 = m10 - (m10 / area) * area;
	M11 = m11 - m10 * m01 / area;
	M02 = m02 - pow(m01, 2) / area;
	M20 = m20 - pow(m10, 2) / area;
	M03 = m03 - 3 * m02 * center_y - 2 * m01 * pow(center_x, 2);
	M30 = m30 - 3 * m20*center_x + 2 * m10 * pow(center_x, 2);
	M21 = m21 - 2 * m11*center_x - m20 * center_y + 2 * m01 * pow(center_x, 2);
	M12 = m12 - 2 * m11*center_y - m02 * center_x + 2 * m10 * pow(center_y, 2);
	
	M1 = (M20 + M02) / pow(area, 2);
	M2 = (pow((M20 - M02), 2) + 4 * pow(M11, 2)) / pow(area, 4);
	M3 = (pow((M30 - 3 * M12), 2) + pow(3 * M21 - M03, 2)) / pow(area, 5);
	M4 = ((pow(M30 + M12, 2) + pow(M21 + M03, 2))) / pow(area, 5);
	M6 = ((M20 - M02)*(pow(M30 + M12, 2) - pow(M21 + M03, 2)) + 4 * M11*((M30 + M12)*(M21 + M03))) / pow(area, 7);
	M7 = (M20*M02 - pow(M11, 2)) / pow(area, 4);
	M8 = (M30*M12 + M21 * M03 - pow(M12, 2) - pow(M21, 2)) / pow(area, 5);
	M9 = (M20*(M21*M03 - pow(M12, 2)) + M02 * (M03*M12 - pow(M21, 2)) - M11 * (M30*M03 - M21 * M12)) / pow(area, 7);

}

bool Shape::operator==(const Shape & q)
{
	return ((color.val[0] == q.color.val[0]) && (color.val[1] == q.color.val[1]) && (color.val[2] == q.color.val[2]));
}

double Shape::countGeometricMoments(int p, int q)
{
	double resultMoment = 0;
	for (std::list<cv::Point2i>::iterator it = points.begin(); it != points.end(); ++it) {
		resultMoment = resultMoment + pow(it->x + 1, p) * pow(it->y + 1, q);
	}
	return resultMoment;
}