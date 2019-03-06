// GeometricAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "ConvexHull.h"
#include "Polygon.h"



int main()
{
	Polygon P;
	std::vector<cv::Point> poly = P.GenerateRandom(15, 500, 500);
	cv::Mat imgPolygon = P.DrawPolygon(poly);
	
	ConvexHull ch;
	std::vector<cv::Point> CH = ch.GrahamsScan(poly);
	cv::Mat imgCH = ch.DrawConvex(CH);

	cv::Mat res;
	cv::hconcat(imgPolygon, imgCH, res);

	cv::Point q(40, 50);
	int m = ch.FindRightTangent(CH, q);
	cv::Mat imgTangent = ch.DrawConvexAndQueryPoint(CH, q, m);

	//cv::imshow("polygon vs. convex hull", res);
	cv::imshow("Right tangent", imgTangent);
	cv::waitKey();

	return 0;
}

