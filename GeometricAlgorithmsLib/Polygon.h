#ifndef POLYGON_H_
#define POLYGON_H_

#include <opencv2/opencv.hpp>

/** \class Polygon
*  \brief A class that handles polygons and operations on polygons
*/


class Polygon
{
public:

	/** @brief constructor that stores the given Point2fs as a polygon connected in a CCW order 
		@param Point2fs - list of 2D Point2fs defining the polygon
		@return
	**/
	Polygon(std::vector<cv::Point2f> Point2fs = std::vector<cv::Point2f>());

	/** @brief generates a polygon constructed of n random Point2fs in range 
		@param numPoint2fs - number of Point2fs in polygon
		@param xRange - maximal values on x axis for Point2fs
		@param yRange - maximal values on y axis for Point2fs
		@return the polygon as a CCW list of vertices
	**/
	std::vector<cv::Point2f> GenerateRandom(int numPoint2fs, int xRange, int yRange);

	/** @brief connects n Point2fs into a polygon in CCW order
		@param Point2fs - list of 2D Point2fs defining the polygon
		@return the polygon as a CCW list of vertices
	**/
	std::vector<cv::Point2f> ConnectCCW(std::vector<cv::Point2f> Point2fs);

	/** @brief draws the polygon as Point2fs connects by lines
		@param Point2fs - list of 2D Point2fs defining the polygon
		@return an RGB image
	**/
	cv::Mat DrawPolygon(std::vector<cv::Point2f> Point2fs);

private:

	std::vector<cv::Point2f> m_Point2fs;
};

#endif