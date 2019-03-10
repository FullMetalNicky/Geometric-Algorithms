#include "ConvexHull.h"
#include "OrientationTest.h"

std::vector<cv::Point> ConvexHull::GrahamsScan(std::vector<cv::Point> P, cv::Mat debug)
{
	std::vector<cv::Point> CHUpper;
	std::vector<cv::Point> CHLower;
	int n = P.size();
	
	if (n < 3) return std::vector<cv::Point>();

	//sort by x axis
	std::sort(P.begin(), P.end(), [](cv::Point p1, cv::Point p2) {

		return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
	});

	CHLower.push_back(P[0]);
	CHLower.push_back(P[1]);

	for (int i = 2; i < n; ++i)
	{
		int ind = CHLower.size() - 1;
		
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

std::vector<cv::Point> ConvexHull::JarvisMarch(std::vector<cv::Point> P, cv::Mat debug)
{
	std::vector<cv::Point> CH;
	int n = P.size();

	if (n < 3) return CH;

	 cv::Point p0 = *std::min_element(P.begin(), P.end(), [](cv::Point p1, cv::Point p2) {

		return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
	});

	 int first = 0;
	 int current = 0;
	 int next = 0;
	 do
	 {
		 CH.push_back(P[current]);
		
		 next = (current + 1) % n;
		 for (int i = 0; i < n; i++)
		 {
			//check if there is a point that is more CCW than the current one
			 if (OrientationTest::getSign(P[current], P[i], P[next]) > 0) next = i;
		 }
		
		 current = next;
	 } while (next != first);  // full circle

	return CH;
}

std::vector<cv::Point> ConvexHull::ChansAlgorithm(std::vector<cv::Point> P, cv::Mat debug)
{

	int m = 4;
	int n = P.size();

	if (n < 3) return std::vector<cv::Point>();

	//repeat until m >= h
	while (true)
	{		
		int r = int(ceil((float(n) / float(m))));
		std::vector<std::vector<cv::Point>> miniPs(r, std::vector<cv::Point>());
		std::vector<std::vector<cv::Point>> miniCHs(r, std::vector<cv::Point>());
		int leftmostInd = -1;
		cv::Point leftmostPoint = cv::Point(INT_MAX, INT_MAX);

		for (int i = 0; i < r; ++i)
		{
			//divide into m roups of size r each
			if((i + 1) * m >= n) miniPs[i].insert(miniPs[i].end(), P.begin() + i * m, P.end());
			else miniPs[i].insert(miniPs[i].end(), P.begin() + i * m, P.begin() + (i + 1) * m);

			//compte Graham's scan for each minihull
			if (miniPs[i].size() < 3) miniCHs[i] = miniPs[i];
			else miniCHs[i] = GrahamsScan(miniPs[i], debug);

			//find the leftmost minihull
			cv::Point pLeft = *std::min_element(miniCHs[i].begin(), miniCHs[i].end(), [](cv::Point p1, cv::Point p2) {

				return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
			});

			if ((pLeft.x < leftmostPoint.x) || ((pLeft.x == leftmostPoint.x) && (pLeft.y == leftmostPoint.y)))
			{
				leftmostPoint = pLeft;
				leftmostInd = i;
			}
		}

		//start with the miniCH that has the leftmost point, so it has to be on the hull
		std::vector<cv::Point> CH;
		CH.push_back(leftmostPoint);

		for(int step = 0; step < m; ++m)
		{
			std::vector<cv::Point> rightTanVert;
			rightTanVert.push_back(CH[CH.size() - 1]);

			//find right tangent for all minihulls
			for (int i = 0; i < r; ++i)
			{
				int m = FindRightTangent(miniCHs[i], CH[CH.size() - 1], debug);
				if (m == -1)
				{
					if (miniCHs[i].size() == 1)
					{
						rightTanVert.push_back(miniCHs[i][0]);
					}
					else //2 element in this minihull
					{
						if(OrientationTest::getSign(CH[CH.size() - 1], miniCHs[i][0], miniCHs[i][1]) > 0) rightTanVert.push_back(miniCHs[i][0]);
						else rightTanVert.push_back(miniCHs[i][1]);
					}
				}
				else rightTanVert.push_back(miniCHs[i][m]);
			}

			//find the correct right tangent for the current point
			std::vector<cv::Point> tmp = JarvisMarch(rightTanVert, debug);
			if (tmp[1] == CH[0])
			{
				return CH;
			}
			CH.push_back(tmp[1]);
		}
		// squaring scheme
		m = m * m;
	}
}

bool ConvexHull::IsConvex(std::vector<cv::Point> P)
{
	int n = P.size();
	if (n < 3) return false;

	//sort by x axis
	std::sort(P.begin(), P.end(), [](cv::Point p1, cv::Point p2) {

		return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
	});

	
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
	int n = P.size();

	for (int i = 0; i < n; ++i)
	{
		cv::Point p1 = P[i] + cv::Point(5, 5);
		cv::Point p2 = P[(i + 1) % n] + cv::Point(5, 5);
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
	int n = P.size();

	for (int i = 0; i < n; ++i)
	{
		cv::Point p1 = P[i] + cv::Point(5, 5);
		cv::Point p2 = P[(i + 1) % n] + cv::Point(5, 5);
		if(i == tangentIndex) cv::circle(img, p1, 3, cv::Scalar(0, 255, 255));
		else cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
		cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	cv::circle(img, q, 3, cv::Scalar(255, 0, 0));

	cv::flip(img, img, 0);

	return img;
}


int ConvexHull::FindRightTangent(std::vector<cv::Point> P, cv::Point q, cv::Mat debug)
{
	int n = P.size();
	if (n < 3) return -1;

	int l = 0;
	int h = n;

	if (P.end() != std::find(P.begin(), P.end(), q))
	{
		int ind = std::distance(P.begin(), std::find(P.begin(), P.end(), q));
		return (ind + 1) % n;
	}

	if ((OrientationTest::getSign(q, P[0], P[n - 1]) > 0) && (OrientationTest::getSign(q, P[0], P[1]) > 0)) return 0;

	for (; ;)
	{
		int m = (h + l) / 2;
		int qmm1 = OrientationTest::getSign(q, P[m], P[(m + 1) % n]);
		if (qmm1 > 0)
		{
			if (OrientationTest::getSign(q, P[m], P[(m - 1) % n]) > 0) return m;
		}

		//segment beginning configuration
		if (OrientationTest::getSign(q, P[l], P[(l + 1) % n]) < 0)
		{
			if (qmm1 > 0)
			{
				h = m;
			}
			else
			{
				if (OrientationTest::getSign(q, P[l], P[m]) < 0)
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
				if (OrientationTest::getSign(q, P[l], P[m]) > 0)
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


int ConvexHull::FindMaximalDotProduct(std::vector<cv::Point> P, cv::Point q, cv::Mat debug)
{
	int n = P.size();
	if (n < 3) return -1;

	int l = 0;
	int h = n;

	if ((P[0].dot(q) > P[n - 1].dot(q)) && (P[0].dot(q) > P[1].dot(q))) return 0;

	for (; ;)
	{
		int m = (h + l) / 2;
		int qmm1 = P[m].dot(q) > P[(m + 1) % n].dot(q);
		if (qmm1)
		{
			if (P[m].dot(q) > P[(m - 1) % n].dot(q)) return m;
		}

		//segment beginning configuration
		if (!(P[l].dot(q) > P[(l + 1) % n].dot(q)))
		{
			if (qmm1)
			{
				h = m;
			}
			else
			{
				if (!(P[l].dot(q) > P[m].dot(q)))
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
				if (P[l].dot(q) > P[m].dot(q))
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