#include "Polygon.h"
#include "OrientationTest.h"

Polygon::Polygon(std::vector<cv::Point> points)
{
	if(points.size()) m_points = ConnectCCW(points);
}

std::vector<cv::Point> Polygon::GenerateRandom(int n, int xRange, int yRange)
{
	std::vector<cv::Point> P;

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

	P = ConnectCCW(P);

	return P;
}

std::vector<cv::Point> Polygon::ConnectCCW(std::vector<cv::Point> points)
{
	std::vector<cv::Point> pUpper;
	std::vector<cv::Point> pLower;
	int n = points.size();
	//sort by x axis
	std::sort(points.begin(), points.end(), [](cv::Point p1, cv::Point p2) {

		return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
	});

	cv::Point right = points[n - 1];
	cv::Point left = points[0];
	pLower.push_back(left);

	for (int i = 1; i < n - 1; ++i)
	{
		if (OrientationTest::getSign(right, left, points[i]) > 0)
		{
			pLower.push_back(points[i]);
		}
		else
		{
			pUpper.push_back(points[i]);
		}
	}
	pUpper.push_back(right);
	std::reverse(pUpper.begin(), pUpper.end());

	std::vector<cv::Point> P(pLower);
	P.insert(P.end(), pUpper.begin(), pUpper.end());

	return P;
}


cv::Mat Polygon::DrawPolygon(std::vector<cv::Point> points)
{
	cv::Rect rect = cv::boundingRect(points);
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_8UC3);
	for (int i = 0; i < points.size(); ++i)
	{
		cv::Point p1 = points[i] + cv::Point(5, 5);
		cv::Point p2 = points[(i + 1) % points.size()] + cv::Point(5, 5);
		cv::circle(img, p1, 1, cv::Scalar(0, 0, 255));
		cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	cv::flip(img, img, 0);

	return img;
}