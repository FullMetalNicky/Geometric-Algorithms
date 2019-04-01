#include "Line.h"
#include "OrientationTest.h"

bool Line::IsIntersecting(cv::Point2f a, cv::Point2f b, cv::Point2f c, cv::Point2f d)
{
	//we check the condition for non - intersection
	bool firstCombo = ((OrientationTest::getSign(a, b, c) == OrientationTest::getSign(a, b, d)) && (OrientationTest::getSign(c, d, a) != OrientationTest::getSign(c, d, b)));
	bool secondCombo = ((OrientationTest::getSign(a, b, c) != OrientationTest::getSign(a, b, d)) && (OrientationTest::getSign(c, d, a) == OrientationTest::getSign(c, d, b)));
	// we return the not of the answe because we look for intersection
	return !(firstCombo || secondCombo);
}