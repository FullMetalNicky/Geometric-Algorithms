#include "Line.h"
#include "OrientationTest.h"

bool Line::IsIntersecting(cv::Point a, cv::Point b, cv::Point c, cv::Point d)
{
	//we check the condition for non - intersection
	bool firstCombo = ((OrientationTest::getSign(a, b, c) == OrientationTest::getSign(a, b, d)) && (OrientationTest::getSign(c, d, a) != OrientationTest::getSign(c, d, b)));
	bool secondCombo = ((OrientationTest::getSign(a, b, c) != OrientationTest::getSign(a, b, d)) && (OrientationTest::getSign(c, d, a) == OrientationTest::getSign(c, d, b)));
	// we return the not of the answe because we look for intersection
	return !(firstCombo || secondCombo);
}