#include "OrientationTest.h"


int OrientationTest::getSign(cv::Point p, cv::Point q, cv::Point r)
{
	int res = getAngle(p, q, r);
	return (res > 0) ? 1 : ((res < 0) ? -1 : 0);
}

int OrientationTest::getAngle(cv::Point p, cv::Point q, cv::Point r)
{
	int res = (q.x * r.y - r.x * q.y) - (p.x * r.y - r.x * p.y) + (p.x * q.y - p.y * q.x);
	return res;
}