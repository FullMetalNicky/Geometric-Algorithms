#ifndef CONVEXHULL_H_
#define CONVEXHULL_H_
#include <opencv2/opencv.hpp>

/** \class ConvexHull
*  \brief A class for convec hull algorithms
*/


class ConvexHull
{
public:

	/** @brief computes convex hull using Graham's scan
		@param P - list of 2D points defining the polygon
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return a convex polygon given by a list of vertices ordered CCW
	**/
	std::vector<cv::Point> GrahamsScan(std::vector<cv::Point> P, cv::Mat debug = cv::Mat());

	/** @brief computes convex hull using Jarvis march
		@param P - list of 2D points defining the polygon
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return a convex polygon given by a list of vertices ordered CCW
	**/
	std::vector<cv::Point> JarvisMarch(std::vector<cv::Point> P, cv::Mat debug = cv::Mat());

	/** @brief computes convex hull using Chan's algoritm
		@param P - list of 2D points defining the polygon
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return a convex polygon given by a list of vertices ordered CCW
	**/
	std::vector<cv::Point> ChansAlgorithm(std::vector<cv::Point> P, cv::Mat debug = cv::Mat());

	/** @brief checks if a list of vertices are defining a CCW convex hull
		@param P - list of 2D points defining the polygon
		@return true if it's a CCW convex hull, false otherwise
	**/
	bool IsConvex(std::vector<cv::Point> P);

	/** @brief draws the polygon as points connects by lines
		@param P - list of 2D points defining the polygon
		@return an RGB image 
	**/
	cv::Mat DrawConvex(std::vector<cv::Point> P);

	/** @brief draws the polygon as points connects by lines and the query point
		@param P - list of 2D points defining the polygon
		@param q - query point
		@param tangentIndex - the result of FindRightTangent function
		@return an RGB image
	**/
	cv::Mat DrawConvexAndQueryPoint(std::vector<cv::Point> P, cv::Point q, int tangentIndex = -1);

	/** @brief finds the right tangent for a query point and a CCW convex hull
		@param P - list of 2D points defining a CCW convex hull
		@param q - query point
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return the index of the vertex in P which is the right tangent
	**/
	int FindRightTangent(std::vector<cv::Point> P, cv::Point q, cv::Mat debug = cv::Mat());

	/** @brief finds the maximal dot product between a query point and a CCW convex hull
		@param P - list of 2D points defining a CCW convex hull
		@param q - query point
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return the index of the vertex in P which is gives maximal dot product with q
	**/
	int FindMaximalDotProduct(std::vector<cv::Point> P, cv::Point q, cv::Mat debug = cv::Mat());

};

#endif