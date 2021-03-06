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

#include "Data.h"

cv::Mat DrawRandom(std::vector<cv::Point2f> Point2fs);
std::vector<cv::Point2f> ParseKoljasDoc();

void Stuff()
{
	Polygon P;
	std::vector<cv::Point2f> poly = P.GenerateRandom(36, 500, 500);
	cv::Mat imgPolygon = P.DrawPolygon(poly);


	ConvexHull ch;
	std::vector<cv::Point2f> CH = ch.GrahamsScan(poly, imgPolygon);
	cv::Mat imgCH = ch.DrawConvex(CH);
	cv::Mat res;
	cv::hconcat(imgPolygon, imgCH, res);

	/*std::vector<cv::Point2f> CH2 = ch.JarvisMarch(poly, imgPolygon);
	cv::Mat imgCH2 = ch.DrawConvex(CH2);
	cv::Mat res2;
	cv::hconcat(imgPolygon, imgCH2, res2);*/

	std::vector<cv::Point2f> CH2 = ch.ChansAlgorithm(poly, imgPolygon);
	cv::Mat imgCH2 = ch.DrawConvex(CH2);
	cv::Mat res2;
	cv::hconcat(imgPolygon, imgCH2, res2);
	cv::imshow("Random Polygon -> Chans Convex Hull", res2);

	cv::Point2f q(0, 1);
	//int m = ch.FindRightTangent(CH, q);
	int m = ch.FindMaximalDotProduct(CH, q, imgCH);
	cv::Mat imgTangent = ch.DrawConvexAndQueryPoint2f(CH, q, m);

	//cv::imshow("polygon vs. convex hull", res);
	cv::imshow("Right tangent", imgTangent);
	cv::waitKey();
}

void TestGrahamSpeed()
{
	int size = 200000;
	std::cout << "generating random Point2fs..." << std::endl;
	auto start1 = std::chrono::steady_clock::now();

	Polygon p;
	std::vector<cv::Point2f> randomPoints = p.GenerateRandom(size, 1000000, 1000000);
	auto end1 = std::chrono::steady_clock::now();
	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
	std::cout << duration1.count() << std::endl;
	std::cout << "connecting random Point2fs..." << std::endl;
	std::vector<cv::Point2f> CCWPoint2fs = p.ConnectCCW(randomPoints);

	ConvexHull ch;
	std::cout << "Graham..." << std::endl;
	auto start = std::chrono::steady_clock::now();
	std::vector<cv::Point2f> convPoints = ch.GrahamsScan(CCWPoint2fs);
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << duration.count() << std::endl;
}



void TestRightTangentSpeed()
{
	std::cout << "reading Point2fs from file..." << std::endl;
	std::vector<cv::Point2f> convexPoints = ParseKoljasDoc();
	//cv::Mat rand = DrawRandom(convexPoints);
	//cv::imwrite("rand.jpg", rand);

	ConvexHull ch;
	std::vector<cv::Point2f> convexPoints2 = ch.GrahamsScan(convexPoints);
	//cv::Mat conv = ch.DrawConvex(convexPoints2);
	//cv::imwrite("conv.jpg", conv);

	cv::Point2f q(0, 0);

	std::cout << "computing right tangent..." << std::endl;

	auto start = std::chrono::steady_clock::now();
	int tanInd = ch.FindRightTangent(convexPoints2, q);
	auto end = std::chrono::steady_clock::now();
	//cv::Mat tan = ch.DrawConvexAndQueryPoint2f(convexPoints2, q, tanInd);
	////cv::imwrite("tan.jpg", tan);

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << duration.count() << std::endl;
}

void TestRightTangentCorrectness()
{
	Polygon p;
	std::vector<cv::Point2f> randomPoints = p.GenerateRandom(100, 1000, 1000);
	std::vector<cv::Point2f> CCWPoint2fs = p.ConnectCCW(randomPoints);
	cv::Mat polyImg = p.DrawPolygon(CCWPoint2fs);
	cv::imwrite("polyImg.jpg", polyImg);

	ConvexHull ch;
	std::vector<cv::Point2f> convexPoint2fs = ch.GrahamsScan(CCWPoint2fs);
	cv::Mat convImg = ch.DrawConvex(convexPoint2fs);
	cv::imwrite("convImg.jpg", convImg);

	cv::Point2f q(0, 0);
	int ind = ch.FindRightTangent(convexPoint2fs, q, convImg);
	cv::Mat tanImg = ch.DrawConvexAndQueryPoint2f(convexPoint2fs, q, ind);
	cv::imwrite("tanImg.jpg", tanImg);

	std::cout << "done." << std::endl;
}

void TestPolygonConstructionCorrectness()
{
	Polygon p;
	std::vector<cv::Point2f> randomPoints = p.GenerateRandom(100, 1000, 1000);
	cv::Mat randomImg = DrawRandom(randomPoints);

	std::vector<cv::Point2f> CCWPoint2fs = p.ConnectCCW(randomPoints);
	cv::Mat polyImg = p.DrawPolygon(CCWPoint2fs);

	std::cout << "done." << std::endl;
}

void TestChan()
{
	Polygon p;
	std::vector<cv::Point2f> randomPoints = p.GenerateRandom(100, 1000, 1000);
	std::vector<cv::Point2f> CCWPoints = p.ConnectCCW(randomPoints);
	cv::Mat polyImg = p.DrawPolygon(CCWPoints);

	ConvexHull ch;
	std::vector<cv::Point2f> convexPoints = ch.ChansAlgorithm(CCWPoints, polyImg);
	cv::Mat convImg = ch.DrawConvex(convexPoints);

	std::vector<cv::Point2f> convexPoints2 = ch.GrahamsScan(CCWPoints, polyImg);
	cv::Mat convImg2 = ch.DrawConvex(convexPoints2);


	std::cout << "done." << std::endl;
}

int main()
{
	//TestPolygonConstructionSpeed();
	//TestRightTangentSpeed();
	//TestRightTangentCorrectness();
	// TestPolygonConstructionCorrectness();
	TestChan();
	//TestGrahamSpeed();

	return 0;
}



std::vector<cv::Point2f> ParseKoljasDoc()
{
	std::ifstream inFile;
	inFile.open("convex_1000.txt", std::ios::in);
	std::vector<cv::Point2f> convexPoint2fs;

	std::string::size_type sz;
	if (inFile.is_open()) 
	{
		std::string line;
		while (getline(inFile, line)) 
		{
			std::size_t pos = line.find(' ');
			std::string yStr = line.substr(pos + 1);
			std::string xStr = line.substr(0, pos);
			float x = (std::stof(xStr, &sz) + 10.0f) * 100.0f;
			float y = (std::stof(yStr, &sz) + 10.0f) * 100.0f;
			//float x = std::stof(xStr, &sz);
			//float y = std::stof(yStr, &sz);
			convexPoint2fs.push_back(cv::Point2f(x, y));
		}
		inFile.close();
	}

	return convexPoint2fs;
}


cv::Mat DrawRandom(std::vector<cv::Point2f> Point2fs)
{
	cv::Rect rect = cv::boundingRect(Point2fs);
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_32FC3);
	for (int i = 0; i < Point2fs.size(); ++i)
	{
		cv::Point2f p1 = Point2fs[i] + cv::Point2f(5, 5);
		//cv::Point2f p2 = Point2fs[(i + 1) % Point2fs.size()] + cv::Point2f(5, 5);
		cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
	}

	cv::flip(img, img, 0);

	return img;
}



