#ifndef ORIENTATIONTEST_H_
#define ORIENTATIONTEST_H_

#include <opencv2/opencv.hpp>


/** \class OrientationTest
*  \brief A class for checking relations between 3 Point2fs on a plane
*/

class OrientationTest
{
public:

	/** @brief get the sign of the orientation between 3 Point2fs (cross product)
		@param p - first Point2f
		@param q - second Point2f
		@param r - third Point2f
		@return sign of orientation test
	**/
	static int getSign(cv::Point2f p, cv::Point2f q, cv::Point2f r);

	/** @brief get the angle between 3 Point2fs (cross product)
		@param p - first Point2f
		@param q - second Point2f
		@param r - third Point2f
		@return angle between the Point2fs
	**/
	static float getAngle(cv::Point2f p, cv::Point2f q, cv::Point2f r);

	/** @brief computes the cross product between 3 Point2fs (cross product)
		@param p - first Point2f
		@param q - second Point2f
		@param r - third Point2f
		@return cross product between the Point2fs
	**/
	static int crossProduct(cv::Point2f p, cv::Point2f q, cv::Point2f r);

	/** @brief computes the dot product between 3 Point2fs (cross product)
		@param p - first Point2f
		@param q - second Point2f
		@param r - third Point2f
		@return d otproduct between the Point2fs
	**/
	static int dotProduct(cv::Point2f p, cv::Point2f q, cv::Point2f r);
};

#endif