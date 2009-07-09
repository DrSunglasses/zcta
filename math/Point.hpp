/* 
 * File:   Point.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 1, 2009, 3:27 PM
 */

#ifndef _POINT_HPP
#define	_POINT_HPP

#include "Rational.hpp"
#include <utility>
#include <boost/operators.hpp>
#include <boost/functional/hash_fwd.hpp>

class Point : boost::totally_ordered<Point> {
private:
	Rational x_, y_;
public:
	typedef Rational reptype;

	Point(const Rational& x = Rational(), const Rational& y = Rational()) : x_(x), y_(y) {
	}
	//allow natural conversions for e.g. basic_point(1, 2)
	template<typename A, typename B>
	Point(const A& x, const B& y) : x_(x), y_(y) {
	}

	const Rational& x() const {
		return x_;
	}
	const Rational& y() const {
		return y_;
	}
	double distanceTo(const Point& pt) const;
	
	friend std::istream& operator>>(std::istream& in, Point& pt);
};

std::ostream& operator<<(std::ostream& out, const Point& pt);
//operator>> provided as friend

bool operator==(const Point& a, const Point& b);

bool operator<(const Point& a, const Point& b);

std::size_t hash_value(const Point& pt);

#endif	/* _POINT_HPP */

