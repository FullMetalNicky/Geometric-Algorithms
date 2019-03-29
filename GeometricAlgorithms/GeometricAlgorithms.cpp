// GeometricAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <time.h> 
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "ConvexHull.h"
#include "Polygon.h"
#include "OrientationTest.h"
#include <fstream>
#include <chrono>


std::vector<cv::Point> GenerateRandonPoints(int n, int xRange, int yRange);
cv::Mat DrawRandom(std::vector<cv::Point> points);
std::vector<cv::Point> ParseKoljasDoc();
cv::Mat DrawConvexAndQueryPointFloat(std::vector<cv::Point2f> P, cv::Point2f q, int tangentIndex);

void Stuff()
{
	Polygon P;
	std::vector<cv::Point> poly = P.GenerateRandom(36, 500, 500);
	cv::Mat imgPolygon = P.DrawPolygon(poly);


	ConvexHull ch;
	std::vector<cv::Point> CH = ch.GrahamsScan(poly, imgPolygon);
	cv::Mat imgCH = ch.DrawConvex(CH);
	cv::Mat res;
	cv::hconcat(imgPolygon, imgCH, res);

	/*std::vector<cv::Point> CH2 = ch.JarvisMarch(poly, imgPolygon);
	cv::Mat imgCH2 = ch.DrawConvex(CH2);
	cv::Mat res2;
	cv::hconcat(imgPolygon, imgCH2, res2);*/

	std::vector<cv::Point> CH2 = ch.ChansAlgorithm(poly, imgPolygon);
	cv::Mat imgCH2 = ch.DrawConvex(CH2);
	cv::Mat res2;
	cv::hconcat(imgPolygon, imgCH2, res2);
	cv::imshow("Random Polygon -> Chans Convex Hull", res2);

	cv::Point q(0, 1);
	//int m = ch.FindRightTangent(CH, q);
	int m = ch.FindMaximalDotProduct(CH, q, imgCH);
	cv::Mat imgTangent = ch.DrawConvexAndQueryPoint(CH, q, m);

	//cv::imshow("polygon vs. convex hull", res);
	cv::imshow("Right tangent", imgTangent);
	cv::waitKey();
}

void TestPolygonConstructionSpeed()
{
	int size = 200000;
	std::cout << "generating random points..." << std::endl;
	auto start1 = std::chrono::steady_clock::now();

	std::vector<cv::Point> randomPoints = GenerateRandonPoints(size, 1000000, 1000000);
	auto end1 = std::chrono::steady_clock::now();
	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
	std::cout << duration1.count() << std::endl;

	std::cout << "connecting random points..." << std::endl;
	Polygon p;
	auto start = std::chrono::steady_clock::now();
	std::vector<cv::Point> CCWpoints = p.ConnectCCW(randomPoints);
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << duration.count() << std::endl;
}

void TestRightTangentSpeed()
{
	std::cout << "reading points from file..." << std::endl;
	std::vector<cv::Point> convexPoints = ParseKoljasDoc();
	ConvexHull ch;
	cv::Point q(0, 0);

	std::cout << "computing right tangent..." << std::endl;

	auto start = std::chrono::steady_clock::now();
	int tanInd = ch.FindRightTangent(convexPoints, q);
	auto end = std::chrono::steady_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << duration.count() << std::endl;
}

void TestRightTangentCorrectness()
{
	std::vector<cv::Point> randomPoints = GenerateRandonPoints(100, 1000, 1000);
	Polygon p;
	std::vector<cv::Point> CCWpoints = p.ConnectCCW(randomPoints);
	cv::Mat polyImg = p.DrawPolygon(CCWpoints);

	ConvexHull ch;
	std::vector<cv::Point> convexPoints = ch.GrahamsScan(CCWpoints);
	cv::Mat convImg = ch.DrawConvex(convexPoints);

	cv::Point q(0, 0);
	int ind = ch.FindRightTangent(convexPoints, q);
	cv::Mat tanImg = ch.DrawConvexAndQueryPoint(convexPoints, q, ind);

	std::cout << "done." << std::endl;
}

void TestPolygonConstructionCorrectness()
{
	std::vector<cv::Point> randomPoints = GenerateRandonPoints(100, 1000, 1000);
	cv::Mat randomImg = DrawRandom(randomPoints);

	Polygon p;
	std::vector<cv::Point> CCWpoints = p.ConnectCCW(randomPoints);
	cv::Mat polyImg = p.DrawPolygon(CCWpoints);

	std::cout << "done." << std::endl;
}

void TestChan()
{
	std::vector<cv::Point> randomPoints = GenerateRandonPoints(100, 1000, 1000);
	Polygon p;
	std::vector<cv::Point> CCWpoints = p.ConnectCCW(randomPoints);
	cv::Mat polyImg = p.DrawPolygon(CCWpoints);

	ConvexHull ch;
	std::vector<cv::Point> convexPoints = ch.ChansAlgorithm(CCWpoints, polyImg);
	cv::Mat convImg = ch.DrawConvex(convexPoints);

	std::cout << "done." << std::endl;
}

int main()
{
	TestPolygonConstructionSpeed();
	//TestRightTangentSpeed();
	//TestRightTangentCorrectness();
	// TestPolygonConstructionCorrectness();
	//TestChan();

	return 0;
}



std::vector<cv::Point> ParseKoljasDoc()
{
	std::ifstream inFile;
	inFile.open("convex_1000.txt", std::ios::in);
	std::vector<cv::Point> convexPoints;

	std::string::size_type sz;
	if (inFile.is_open()) 
	{
		std::string line;
		while (getline(inFile, line)) 
		{
			//printf("%s", line.c_str());
			std::size_t pos = line.find(' ');
			std::string yStr = line.substr(pos + 1);
			std::string xStr = line.substr(0, pos);
			//std::cout << "x: " << xStr << " y: " << yStr << std::endl;
			float x = std::stof(xStr, &sz) * 100000.0f;
			float y = std::stof(yStr, &sz) * 100000.0f;
			convexPoints.push_back(cv::Point(x, y));
		}
		inFile.close();
	}

	return convexPoints;
}


cv::Mat DrawRandom(std::vector<cv::Point> points)
{
	cv::Rect rect = cv::boundingRect(points);
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_8UC3);
	for (int i = 0; i < points.size(); ++i)
	{
		cv::Point p1 = points[i] + cv::Point(5, 5);
		cv::Point p2 = points[(i + 1) % points.size()] + cv::Point(5, 5);
		cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
		//cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	cv::flip(img, img, 0);

	return img;
}


std::vector<cv::Point> GenerateRandonPoints(int n, int xRange, int yRange)
{
	std::vector<cv::Point> P;
	srand(time(NULL));

	for (int i = 0; i < n; ++i)
	{
		int x = rand() % xRange;
		int y = rand() % yRange;
		cv::Point p(x, y);
		if (P.end() == std::find(P.begin(), P.end(), p))
		{
			P.push_back(p);
		}
	}

	return P;
}

