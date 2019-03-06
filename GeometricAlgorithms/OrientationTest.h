#ifndef ORIENTATIONTEST_H_
#define ORIENTATIONTEST_H_

#include <opencv2/opencv.hpp>


/** \class OrientationTest
*  \brief A class for checking relations between 3 points on a plane
*/

class OrientationTest
{
public:

	/** @brief get the sign of the orientation between 3 points (cross product)
		@param p - first point
		@param q - second point
		@param r - third point
		@return sign of orientation test
	**/
	static int getSign(cv::Point p, cv::Point q, cv::Point r);

	/** @brief get the angle between 3 points (cross product)
		@param p - first point
		@param q - second point
		@param r - third point
		@return angle between the points
	**/
	static int getAngle(cv::Point p, cv::Point q, cv::Point r);
};

#endif