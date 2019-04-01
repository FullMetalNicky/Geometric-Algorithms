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
		@param P - list of 2D Point2fs defining the polygon
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return a convex polygon given by a list of vertices ordered CCW
	**/
	std::vector<cv::Point2f> GrahamsScan(std::vector<cv::Point2f> P, cv::Mat debug = cv::Mat());

	/** @brief computes convex hull using Jarvis march
		@param P - list of 2D Points defining the polygon
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return a convex polygon given by a list of vertices ordered CCW
	**/
	std::vector<cv::Point2f> JarvisMarch(std::vector<cv::Point2f> P, cv::Mat debug = cv::Mat());

	/** @brief computes one step of Jarvis march - computing the right tangent for point q, where P is some general set of points
		@param P - list of 2D Points
		@param q - query point
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return an index of the point in P that is a right tangent to q
	**/
	int JarvisStep(std::vector<cv::Point2f> P, cv::Point2f q, cv::Mat debug = cv::Mat());

	/** @brief computes convex hull using Chan's algoritm
		@param P - list of 2D Point2fs defining the polygon
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return a convex polygon given by a list of vertices ordered CCW
	**/
	std::vector<cv::Point2f> ChansAlgorithm(std::vector<cv::Point2f> P, cv::Mat debug = cv::Mat());

	/** @brief checks if a list of vertices are defining a CCW convex hull
		@param P - list of 2D Point2fs defining the polygon
		@return true if it's a CCW convex hull, false otherwise
	**/
	bool IsConvex(std::vector<cv::Point2f> P);

	/** @brief draws the polygon as Point2fs connects by lines
		@param P - list of 2D Point2fs defining the polygon
		@return an RGB image 
	**/
	cv::Mat DrawConvex(std::vector<cv::Point2f> P);

	/** @brief draws the polygon as Point2fs connects by lines and the query Point2f
		@param P - list of 2D Point2fs defining the polygon
		@param q - query Point2f
		@param tangentIndex - the result of FindRightTangent function
		@return an RGB image
	**/
	cv::Mat DrawConvexAndQueryPoint2f(std::vector<cv::Point2f> P, cv::Point2f q, int tangentIndex = -1);

	/** @brief finds the right tangent for a query Point2f and a CCW convex hull
		@param P - list of 2D Point2fs defining a CCW convex hull
		@param q - query Point2f
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return the index of the vertex in P which is the right tangent
	**/
	int FindRightTangent(std::vector<cv::Point2f> P, cv::Point2f q, cv::Mat debug = cv::Mat());

	/** @brief finds the maximal dot product between a query Point2f and a CCW convex hull
		@param P - list of 2D Point2fs defining a CCW convex hull
		@param q - query Point2f
		@param debug - Optionary. An image of the polygon P, for debugging purposes
		@return the index of the vertex in P which is gives maximal dot product with q
	**/
	int FindMaximalDotProduct(std::vector<cv::Point2f> P, cv::Point2f q, cv::Mat debug = cv::Mat());

};

#endif