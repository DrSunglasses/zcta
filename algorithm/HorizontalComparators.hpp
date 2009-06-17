/* 
 * File:   HorizontalComparators.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 12, 2009, 11:25 AM
 */

#ifndef _HORIZONTALCOMPARATORS_HPP
#define	_HORIZONTALCOMPARATORS_HPP

#include "../math/Point.hpp"
#include "../math/LineSegment.hpp"
#include "Algorithm.hpp"

void testHorizontalPointComparator();
void testHorizontalLineComparator();

//#define RIGHT_ORDER -1
//#define WRONG_ORDER 1

struct HorizontalPointComparator {
	int operator()(const Point& a, const Point& b) {
		if (a.y() < b.y())
			return -1;
		if (a.y() > b.y())
			return 1;
		if (a.x() < b.x())
			return -1;
		if (a.x() > b.x())
			return 1;
		return 0;
	}
};

struct HorizontalLineComparator {
#define SMALL_X -180
#define LARGE_X 180
	HorizontalLineComparator() : yVal(-500), line(Line(Point(SMALL_X, yVal), Point(LARGE_X, yVal))) {
	}
	int operator()(const Line& a, const Line& b) {
		if (a.horizontal())
			throw std::logic_error("a horizontal");
		if (b.horizontal())
			throw std::logic_error("b horizontal");
		if (a == b)
			return 0;
		Point apt = line.intersectionWith(a);
		Point bpt = line.intersectionWith(b);
		int res = HorizontalPointComparator()(apt, bpt);
		if (res != 0)
			return res;
		Rational aTheta = a.theta(), bTheta = b.theta();
		clamp(&aTheta, 0, pi(), pi());
		clamp(&bTheta, 0, pi(), pi());
		if (aTheta > bTheta)
			return -1;
		if (aTheta < bTheta)
			return 1;
		throw std::logic_error("Unequal but coincident?");
	}
	void update(const Point& pt) {
		yVal = pt.y();
		line = Line(Point(SMALL_X, yVal), Point(LARGE_X, yVal));
	}
	bool pointInFuture(const Point& pt) {
		return pt.y() > yVal;
	}
	bool parallelToSweepLine(const ZCTASegment& seg) {
		return seg.line().horizontal();
	}
private:
	Point::reptype yVal;
	Line line;
	void clamp(Rational* x, const Rational& lowerBound, const Rational& upperBound, const Rational& increment) {
		while (*x < lowerBound)
			*x += increment;
		while (*x >= upperBound)
			*x -= increment;
	}
};

typedef Algorithm<HorizontalPointComparator, HorizontalLineComparator> HorizontalAlgorithm;

#endif	/* _HORIZONTALCOMPARATORS_HPP */

