#include "ConvexHull.h"
#include "OrientationTest.h"

std::vector<cv::Point> ConvexHull::GrahamsScan(std::vector<cv::Point> P)
{
	std::vector<cv::Point> CHUpper;
	std::vector<cv::Point> CHLower;
	int n = P.size();
	
	//sort by x axis
	std::sort(P.begin(), P.end(), [](cv::Point p1, cv::Point p2) {

		return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
	});

	CHLower.push_back(P[0]);
	CHLower.push_back(P[1]);

	for (int i = 2; i < n; ++i)
	{
		int ind = CHLower.size() - 1;
		int orient = OrientationTest::getSign(CHLower[ind - 1], CHLower[ind], P[i]);
		
		while (OrientationTest::getSign(CHLower[ind - 1], CHLower[ind], P[i]) < 0)
		{
			CHLower.erase(CHLower.begin() + ind);
			ind = CHLower.size() - 1;
			if (ind == 0) break;
		}
		if (i != (n - 1))
		{
			CHLower.push_back(P[i]);
		}
	}

	CHUpper.push_back(P[n - 1]);
	CHUpper.push_back(P[n - 2]);
	for (int i = n - 3; i >= 0; --i)
	{
		int ind = CHUpper.size() - 1;
		int orient = OrientationTest::getSign(CHUpper[ind - 1], CHUpper[ind], P[i]);
		
		while (OrientationTest::getSign(CHUpper[ind - 1], CHUpper[ind], P[i]) < 0)
		{
			CHUpper.erase(CHUpper.begin() + ind);
			ind = CHUpper.size() - 1;
			if (ind == 0) break;
		}
		if (i != 0)
		{
			CHUpper.push_back(P[i]);
		}
	}

	std::vector<cv::Point> CH(CHLower);
	CH.insert(CH.end(), CHUpper.begin(), CHUpper.end());

	return CH;
}

bool ConvexHull::IsConvex(std::vector<cv::Point> P)
{
	//sort by x axis
	std::sort(P.begin(), P.end(), [](cv::Point p1, cv::Point p2) {

		return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
	});

	int n = P.size();
	int signChanges = 0;

	for (int i = 0; i < n; ++i)
	{
		if (OrientationTest::getSign(P[i], P[(i + 1) % n], P[(i + 2) % n]) > 0)
		{
			//we check "derivatives" with respect to x for each point, to see we don't have to many sign variations
			if ((P[i].x - P[(i + 1) % n].x) * (P[(i + 1) % n].x - P[(i + 2) % n].x) < 0)
			{
				++signChanges;
				//more than two sign variations means we will have an intersection
				if (signChanges > 2) return false;
			}
		}
		else return false;
	}

	return true;
}

cv::Mat ConvexHull::DrawConvex(std::vector<cv::Point> P)
{
	cv::Rect rect = cv::boundingRect(P);
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_8UC3);
	for (int i = 0; i < P.size(); ++i)
	{
		cv::Point p1 = P[i] + cv::Point(5, 5);
		cv::Point p2 = P[(i + 1) % P.size()] + cv::Point(5, 5);
		cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
		cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	cv::flip(img, img, 0);

	return img;
}

cv::Mat ConvexHull::DrawConvexAndQueryPoint(std::vector<cv::Point> P, cv::Point q, int tangentIndex)
{
	P.push_back(q);
	cv::Rect rect = cv::boundingRect(P);
	P.pop_back();
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_8UC3);
	for (int i = 0; i < P.size(); ++i)
	{
		cv::Point p1 = P[i] + cv::Point(5, 5);
		cv::Point p2 = P[(i + 1) % P.size()] + cv::Point(5, 5);
		if(i == tangentIndex) cv::circle(img, p1, 3, cv::Scalar(0, 255, 255));
		else cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
		cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	cv::circle(img, q, 3, cv::Scalar(255, 0, 0));

	cv::flip(img, img, 0);

	return img;
}


int ConvexHull::FindRightTangent(std::vector<cv::Point> P, cv::Point q)
{
	int l = 0;
	int h = P.size();

	for (; ;)
	{
		int m = (h + l) / 2;
		int qmm1 = OrientationTest::getSign(q, P[m], P[(m + 1) % P.size()]);
		if (qmm1 > 0)
		{
			int qm1m = OrientationTest::getSign(q, P[m], P[(m - 1) % P.size()]);
			if (qm1m > 0) return m;
		}

		//segment beginning configuration
		int qll1 = OrientationTest::getSign(q, P[l], P[(l + 1) % P.size()]);
		if (qll1 < 0)
		{
			if (qmm1 > 0)
			{
				h = m;
			}
			else
			{
				int qlm = OrientationTest::getSign(q, P[l], P[m]);
				if (qlm < 0)
				{
					l = m;
				}
				else
				{
					h = m;
				}
			}
		}
		else
		{
			if (qmm1 < 0)
			{
				l = m;
			}
			else
			{
				int qlm = OrientationTest::getSign(q, P[l], P[m]);
				if (qlm > 0)
				{
					h = m;
				}
				else
				{
					l = m;
				}
			}
		}
	}
}


int ConvexHull::FindMaximalDotProduct(std::vector<cv::Point> P, cv::Point q)
{
	int l = 0;
	int h = P.size();

	for (; ;)
	{
		int m = (h + l) / 2;
		int mq = P[m].dot(q);
		int m1q = P[(m + 1) % P.size()].dot(q);
		int qmm1 = P[m].dot(q) > P[(m + 1) % P.size()].dot(q);
		if (qmm1)
		{
			int mq = P[m].dot(q);
			int q1m = P[(m - 1) % P.size()].dot(q);
			int qm1m = P[m].dot(q) > P[(m - 1) % P.size()].dot(q);
			if (qm1m) return m;
		}

		//segment beginning configuration
		int qll1 = P[l].dot(q) > P[(l + 1) % P.size()].dot(q);
		if (!qll1)
		{
			if (qmm1)
			{
				h = m;
			}
			else
			{
				int qlm = P[l].dot(q) > P[m].dot(q);
				if (!qlm)
				{
					l = m;
				}
				else
				{
					h = m;
				}
			}
		}
		else
		{
			if (!qmm1)
			{
				l = m;
			}
			else
			{
				int qlm = P[l].dot(q) > P[m].dot(q);
				if (qlm)
				{
					h = m;
				}
				else
				{
					l = m;
				}
			}
		}
	}
}