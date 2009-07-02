/* 
 * File:   Line.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 1, 2009, 3:21 PM
 */

#ifndef _LINE_HPP
#define	_LINE_HPP

#include "Rational.hpp"
#include "Point.hpp"
#include <cmath>
#include <iosfwd>

class Line {
private:
	Rational A, B; //Ax + By = 1
	Rational theta_;
	double bottomSquared;
public:
	//TODO: remove these once LineSegment doesn't need them
	typedef Point point;
	typedef Rational reptype;

	Line(const Point& a, const Point& b);
	bool vertical() const;
	bool horizontal() const;
	Point intersectionWith(const Line& other) const;
	bool parallelTo(const Line& other) const;
	double distanceTo(const Point& pt) const;
	Rational theta() const;

	friend bool operator==(const Line& l, const Line& r);
	friend std::ostream& operator<<(std::ostream& out, const Line& pt);
	friend std::size_t hash_value(const Line& pt);
};

bool operator!=(const Line& l, const Line& r);

#endif	/* _LINE_HPP */

