/* 
 * File:   LineSegment.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 3, 2009, 2:33 PM
 */

#ifndef _LINESEGMENT_HPP
#define	_LINESEGMENT_HPP

#include "Line.hpp"
#include <boost/functional/hash_fwd.hpp>

class LineSegment {
private:
	Point a_, b_;
	Line line_;
public:
	LineSegment(const Point& pt1, const Point& pt2);

	const Line& line() const {
		return line_;
	}
	const Point& a() const {
		return a_;
	}
	const Point& b() const {
		return b_;
	}
	double distanceTo(const LineSegment& other) const;
};

bool operator==(const LineSegment& l, const LineSegment& r);
bool operator!=(const LineSegment& l, const LineSegment& r);
bool operator<(const LineSegment& l, const LineSegment& r);

std::size_t hash_value(const LineSegment& seg);

#endif	/* _LINESEGMENT_HPP */


