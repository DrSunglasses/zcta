//June 16, 2009

#ifndef VERTICALCOMPARATORS_HPP
#define VERTICALCOMPARATORS_HPP

#include "../math/Point.hpp"
#include "../math/LineSegment.hpp"
#include "Algorithm.hpp"

void testVerticalPointComparator();
void testVerticalLineComparator();

struct VerticalPointComparator {
	int operator()(const Point& a, const Point& b) {
		const Rational &ax = a.x(), &bx = b.x();
		//points further left/west come first
		if (ax < bx)
			return -1;
		if (ax > bx)
			return 1;
		const Rational &ay = a.y(), &by = b.y();
		//points higher/north come first so that higher-angled
		//lines are first on the sweep line
		if (ay > by)
			return -1;
		if (ay < by)
			return 1;
		return 0;
	}
};

struct VerticalLineComparator {
#define LARGE_Y 180
#define SMALL_Y -180
	VerticalLineComparator() : xVal(-500), line(Point(xVal, LARGE_Y), Point(xVal, SMALL_Y)) {
	}
	int operator()(const Line& a, const Line& b) {
		if (a.vertical())
			throw std::logic_error("a vertical");
		if (b.vertical())
			throw std::logic_error("b vertical");
		if (a == b)
			return 0;
		Point apt = line.intersectionWith(a);
		Point bpt = line.intersectionWith(b);
		int res = VerticalPointComparator()(apt, bpt);
		if (res != 0)
			return res;
		Rational aTheta = a.theta(), bTheta = b.theta();
		clamp(&aTheta, -pi()/2, pi()/2, pi());
		clamp(&bTheta, -pi()/2, pi()/2, pi());
		//higher angles come first
		if (aTheta > bTheta)
			return -1;
		if (aTheta < bTheta)
			return 1;
		throw std::logic_error("Unequal but conincident?");
	}
	void update(const Point& pt) {
		xVal = pt.x();
		line = Line(Point(xVal, LARGE_Y), Point(xVal, SMALL_Y));
	}
	bool pointInFuture(const Point& pt) {
		return pt.x() > xVal;
	}
	bool parallelToSweepLine(const ZCTASegment& seg) {
		return seg.line().vertical();
	}
private:
	Rational xVal;
	Line line;
	void clamp(Rational* x, const Rational& lowerBound, const Rational& upperBound, const Rational& increment) {
		//alters x to be in [lowerBound, upperBound)
		while (*x < lowerBound)
			*x += increment;
		while (*x >= upperBound)
			*x -= increment;
	}
};

typedef Algorithm<VerticalPointComparator, VerticalLineComparator> VerticalAlgorithm;

#endif //VERTICAL_COMPARATORS_HPP
