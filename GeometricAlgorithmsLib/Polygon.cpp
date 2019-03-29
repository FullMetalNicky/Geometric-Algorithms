#include "Polygon.h"
#include "OrientationTest.h"
#include <time.h> 
#include <random>

Polygon::Polygon(std::vector<cv::Point2f> Point2fs)
{
	if(Point2fs.size()) m_Point2fs = ConnectCCW(Point2fs);
}

std::vector<cv::Point2f> Polygon::GenerateRandom(int n, int xRange, int yRange)
{
	std::vector<cv::Point2f> P;
	std::random_device rd;  
	std::mt19937 randGen(rd()); 
	std::uniform_real_distribution<> randDistX(0.0, float(xRange));
	std::uniform_real_distribution<> randDistY(0.0, float(yRange));


	for (int i = 0; i < n; ++i)
	{
		float x = randDistX(randGen);
		float y = randDistY(randGen);
		cv::Point2f p(x, y);
		//if (P.end() == std::find(P.begin(), P.end(), p))
		{
			P.push_back(p);
		}
	}

	P = ConnectCCW(P);

	return P;
}

std::vector<cv::Point2f> Polygon::ConnectCCW(std::vector<cv::Point2f> Point2fs)
{
	std::vector<cv::Point2f> pUpper;
	std::vector<cv::Point2f> pLower;
	int n = Point2fs.size();
	//sort by x axis
	std::sort(Point2fs.begin(), Point2fs.end(), [](cv::Point2f p1, cv::Point2f p2) {

		return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
	});

	cv::Point2f right = Point2fs[n - 1];
	cv::Point2f left = Point2fs[0];
	pLower.push_back(left);

	for (int i = 1; i < n - 1; ++i)
	{
		if (OrientationTest::getSign(right, left, Point2fs[i]) > 0)
		{
			pLower.push_back(Point2fs[i]);
		}
		else
		{
			pUpper.push_back(Point2fs[i]);
		}
	}
	pUpper.push_back(right);
	std::reverse(pUpper.begin(), pUpper.end());

	std::vector<cv::Point2f> P(pLower);
	P.insert(P.end(), pUpper.begin(), pUpper.end());

	return P;
}


cv::Mat Polygon::DrawPolygon(std::vector<cv::Point2f> Point2fs)
{
	cv::Rect rect = cv::boundingRect(Point2fs);
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_8UC3);
	for (int i = 0; i < Point2fs.size(); ++i)
	{
		cv::Point2f p1 = Point2fs[i] + cv::Point2f(5, 5);
		cv::Point2f p2 = Point2fs[(i + 1) % Point2fs.size()] + cv::Point2f(5, 5);
		cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
		cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	cv::flip(img, img, 0);

	return img;
}