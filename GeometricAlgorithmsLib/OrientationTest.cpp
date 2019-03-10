#include "OrientationTest.h"


int OrientationTest::getSign(cv::Point p, cv::Point q, cv::Point r)
{
	int res = crossProduct(p, q, r);
	return (res > 0) ? 1 : ((res < 0) ? -1 : 0);
}

float OrientationTest::getAngle(cv::Point p, cv::Point q, cv::Point r)
{
	int cross = crossProduct(p, q, r);	
	int dot = dotProduct(p, q, r);

	float alpha = atan2(float(cross), float(dot));

	//return (int)floor(alpha * 180.0 / CV_PI + 0.5);
	return (alpha * 180.0 / CV_PI);
}

int OrientationTest::crossProduct(cv::Point p, cv::Point q, cv::Point r)
{
	int cross = (q.x * r.y - r.x * q.y) - (p.x * r.y - r.x * p.y) + (p.x * q.y - p.y * q.x);
	return cross;
}

int OrientationTest::dotProduct(cv::Point p, cv::Point q, cv::Point r)
{
	cv::Point pq = p - q;
	cv::Point qr = r - q;
	int dot = pq.x * qr.x + pq.y + qr.y;

	return dot;
}