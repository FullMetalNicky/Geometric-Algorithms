#ifndef LINE_H_
#define LINE_H_

#include <opencv2/opencv.hpp>

/** \class Line
*  \brief A class that 
*/


class Line
{
public:

	/** @brief checks if two lines ab and cd intersect
		@param a - Point2f on the first line
		@param b - Point2f on the first line
		@param c - Point2f on the second line
		@param d - Point2f on the second line
		@return true if intersecting, false otherwise
	**/
	static bool IsIntersecting(cv::Point2f a, cv::Point2f b, cv::Point2f c, cv::Point2f d);
};

#endif