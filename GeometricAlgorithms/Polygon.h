#ifndef POLYGON_H_
#define POLYGON_H_

#include <opencv2/opencv.hpp>

/** \class Polygon
*  \brief A class that handles polygons and operations on polygons
*/


class Polygon
{
public:

	/** @brief constructor that stores the given points as a polygon connected in a CCW order 
		@param points - list of 2D points defining the polygon
		@return
	**/
	Polygon(std::vector<cv::Point> points = std::vector<cv::Point>());

	/** @brief generates a polygon constructed of n random points in range 
		@param numPoints - number of points in polygon
		@param xRange - maximal values on x axis for points
		@param yRange - maximal values on y axis for points
		@return the polygon as a CCW list of vertices
	**/
	std::vector<cv::Point> GenerateRandom(int numPoints, int xRange, int yRange);

	/** @brief connects n points into a polygon in CCW order
		@param points - list of 2D points defining the polygon
		@return the polygon as a CCW list of vertices
	**/
	std::vector<cv::Point> ConnectCCW(std::vector<cv::Point> points);

	/** @brief draws the polygon as points connects by lines
		@param points - list of 2D points defining the polygon
		@return an RGB image
	**/
	cv::Mat DrawPolygon(std::vector<cv::Point> points);

private:

	std::vector<cv::Point> m_points;
};

#endif