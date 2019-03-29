#include "OrientationTest.h"


int OrientationTest::getSign(cv::Point2f p, cv::Point2f q, cv::Point2f r)
{
	int res = crossProduct(p, q, r);
	return (res > 0) ? 1 : ((res < 0) ? -1 : 0);
}

float OrientationTest::getAngle(cv::Point2f p, cv::Point2f q, cv::Point2f r)
{
	int cross = crossProduct(p, q, r);	
	int dot = dotProduct(p, q, r);

	float alpha = atan2(float(cross), float(dot));

	//return (int)floor(alpha * 180.0 / CV_PI + 0.5);
	return (alpha * 180.0 / CV_PI);
}

int OrientationTest::crossProduct(cv::Point2f p, cv::Point2f q, cv::Point2f r)
{
	int cross = (q.x * r.y - r.x * q.y) - (p.x * r.y - r.x * p.y) + (p.x * q.y - p.y * q.x);
	return cross;
}

int OrientationTest::dotProduct(cv::Point2f p, cv::Point2f q, cv::Point2f r)
{
	cv::Point2f pq = p - q;
	cv::Point2f qr = r - q;
	int dot = pq.x * qr.x + pq.y + qr.y;

	return dot;
}