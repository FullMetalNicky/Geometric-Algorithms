#include "ConvexHull.h"
#include "OrientationTest.h"

std::vector<cv::Point2f> ConvexHull::GrahamsScan(std::vector<cv::Point2f> P, cv::Mat debug)
{
	std::vector<cv::Point2f> CHUpper;
	std::vector<cv::Point2f> CHLower;
	int n = P.size();
	
	if (n < 3) return std::vector<cv::Point2f>();

	//sort by x axis
	std::sort(P.begin(), P.end(), [](cv::Point2f p1, cv::Point2f p2) {

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

	std::vector<cv::Point2f> CH(CHLower);
	CH.insert(CH.end(), CHUpper.begin(), CHUpper.end());

	return CH;
}

std::vector<cv::Point2f> ConvexHull::JarvisMarch(std::vector<cv::Point2f> P, cv::Mat debug)
{
	std::vector<cv::Point2f> CH;
	int n = P.size();

	if (n < 3) return CH;

	 cv::Point2f p0 = *std::min_element(P.begin(), P.end(), [](cv::Point2f p1, cv::Point2f p2) {

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
			//check if there is a Point2f that is more CCW than the current one
			 if (OrientationTest::getSign(P[current], P[i], P[next]) > 0) next = i;
		 }
		
		 current = next;
	 } while (next != first);  // full circle

	return CH;
}

int ConvexHull::JarvisStep(std::vector<cv::Point2f> P, cv::Point2f q, cv::Mat debug)
{
	std::vector<cv::Point2f> CH;
	int n = P.size();

	if (n < 3) return -1;
	int next = 0;
	for (int i = 0; i < n; i++)
	{
		//check if there is a Point2f that is more CCW than the current one
		if (OrientationTest::getSign(q, P[i], P[next]) > 0) next = i;
	}

	return next;
}

std::vector<cv::Point2f> ConvexHull::ChansAlgorithm(std::vector<cv::Point2f> P, cv::Mat debug)
{

	int m = 4;
	int n = P.size();
	
	if (n < 3) return std::vector<cv::Point2f>();

	//repeat until m >= h
	while (true)
	{		
		int r = int(ceil((float(n) / float(m))));
		std::vector<std::vector<cv::Point2f>> miniPs(r, std::vector<cv::Point2f>());
		std::vector<std::vector<cv::Point2f>> miniCHs(r, std::vector<cv::Point2f>());
		int leftmostInd = -1;
		cv::Point2f leftmostPoint2f = cv::Point2f(INT_MAX, INT_MAX);

		for (int i = 0; i < r; ++i)
		{
			//divide into m roups of size r each
			if((i + 1) * m >= n) miniPs[i].insert(miniPs[i].end(), P.begin() + i * m, P.end());
			else miniPs[i].insert(miniPs[i].end(), P.begin() + i * m, P.begin() + (i + 1) * m);

			//compte Graham's scan for each minihull
			if (miniPs[i].size() < 3) miniCHs[i] = miniPs[i];
			else miniCHs[i] = GrahamsScan(miniPs[i], debug);

			//find the leftmost minihull
			cv::Point2f pLeft = *std::min_element(miniCHs[i].begin(), miniCHs[i].end(), [](cv::Point2f p1, cv::Point2f p2) {

				return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
			});

			if ((pLeft.x < leftmostPoint2f.x) || ((pLeft.x == leftmostPoint2f.x) && (pLeft.y == leftmostPoint2f.y)))
			{
				leftmostPoint2f = pLeft;
				leftmostInd = i;
			}
		}

		//start with the miniCH that has the leftmost Point2f, so it has to be on the hull
		std::vector<cv::Point2f> CH;
		CH.push_back(leftmostPoint2f);

		for(int step = 0; step < m; ++m)
		{
			std::vector<cv::Point2f> rightTanVert;
			// q is the point for we search the right tangent in step i
			cv::Point q = CH[CH.size() - 1];

			//find right tangent for all minihulls
			for (int i = 0; i < r; ++i)
			{
				int m = -1;
				//if the query point is part of the minihull, just take the next point on the hull as tangent
				if (miniCHs[i].end() != std::find(miniCHs[i].begin(), miniCHs[i].end(), CH[CH.size() - 1]))
				{
					int ind = std::distance(miniCHs[i].begin(), std::find(miniCHs[i].begin(), miniCHs[i].end(), CH[CH.size() - 1]));
					m = (ind + 1) % miniCHs[i].size();
				}
				//otherwise find the tangent
				else  m = FindRightTangent(miniCHs[i], CH[CH.size() - 1], debug);
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

			//find the correct right tangent for the current Point2f
			int ind = JarvisStep(rightTanVert, CH[CH.size() - 1]);
			if (ind != -1)
			{
				if (CH[0] == rightTanVert[ind]) return CH;
				else
				{
					CH.push_back(rightTanVert[ind]);
				}
			}
			else
			{
				std::cout << "idiot" << std::endl;
			}		
		}

		// squaring scheme
		m = m * m;
	}
}

bool ConvexHull::IsConvex(std::vector<cv::Point2f> P)
{
	int n = P.size();
	if (n < 3) return false;

	//sort by x axis
	std::sort(P.begin(), P.end(), [](cv::Point2f p1, cv::Point2f p2) {

		return ((p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x));
	});

	
	int signChanges = 0;

	for (int i = 0; i < n; ++i)
	{
		if (OrientationTest::getSign(P[i], P[(i + 1) % n], P[(i + 2) % n]) > 0)
		{
			//we check "derivatives" with respect to x for each Point2f, to see we don't have to many sign variations
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

cv::Mat ConvexHull::DrawConvex(std::vector<cv::Point2f> P)
{
	cv::Rect rect = cv::boundingRect(P);
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_8UC3);
	int n = P.size();

	for (int i = 0; i < n; ++i)
	{
		cv::Point2f p1 = P[i] + cv::Point2f(5, 5);
		cv::Point2f p2 = P[(i + 1) % n] + cv::Point2f(5, 5);
		cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
		cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	cv::flip(img, img, 0);

	return img;
}

cv::Mat ConvexHull::DrawConvexAndQueryPoint2f(std::vector<cv::Point2f> P, cv::Point2f q, int tangentIndex)
{
	P.push_back(q);
	cv::Rect rect = cv::boundingRect(P);
	P.pop_back();
	cv::Mat img = cv::Mat::zeros(cv::Size(rect.x + rect.width + 10, rect.y + rect.height + 10), CV_8UC3);
	int n = P.size();

	for (int i = 0; i < n; ++i)
	{
		cv::Point2f p1 = P[i] + cv::Point2f(5, 5);
		cv::Point2f p2 = P[(i + 1) % n] + cv::Point2f(5, 5);
		if(i == tangentIndex) cv::circle(img, p1, 3, cv::Scalar(0, 255, 255));
		else cv::circle(img, p1, 3, cv::Scalar(0, 0, 255));
		cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
	}

	if (-1 != tangentIndex)
	{
		cv::line(img, P[tangentIndex], q, cv::Scalar(255, 255, 255));
	}

	cv::circle(img, q, 3, cv::Scalar(255, 0, 0));

	cv::flip(img, img, 0);

	return img;
}


int ConvexHull::FindRightTangent(std::vector<cv::Point2f> P, cv::Point2f q, cv::Mat debug)
{
	int n = P.size();
	if (n < 3) return -1;

	int l = 0;
	int h = n;

	//move outside the func
	/*if (P.end() != std::find(P.begin(), P.end(), q))
	{
		int ind = std::distance(P.begin(), std::find(P.begin(), P.end(), q));
		return (ind + 1) % n;
	}*/

	if ((OrientationTest::getSign(q, P[0], P[n - 1]) > 0) && (OrientationTest::getSign(q, P[0], P[1]) > 0)) return 0;

	for (; ;)
	{
		int m = (h + l) / 2;
		if (0 == m) return -1;
		int qmm1 = OrientationTest::getSign(q, P[m], P[(m + 1) % n]);
		if (qmm1 > 0.0f)
		{
			if (OrientationTest::getSign(q, P[m], P[(m - 1) % n]) > 0.0f) return m;
		}

		//segment beginning configuration
		if (OrientationTest::getSign(q, P[l], P[(l + 1) % n]) < 0.0f)
		{
			if (qmm1 > 0)
			{
				h = m;
			}
			else
			{
				if (OrientationTest::getSign(q, P[l], P[m]) < 0.0f)
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
				if (OrientationTest::getSign(q, P[l], P[m]) > 0.0f)
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


int ConvexHull::FindMaximalDotProduct(std::vector<cv::Point2f> P, cv::Point2f q, cv::Mat debug)
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