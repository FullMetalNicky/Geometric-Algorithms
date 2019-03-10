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
		@param a - point on the first line
		@param b - point on the first line
		@param c - point on the second line
		@param d - point on the second line
		@return true if intersecting, false otherwise
	**/
	static bool IsIntersecting(cv::Point a, cv::Point b, cv::Point c, cv::Point d);
};

#endif