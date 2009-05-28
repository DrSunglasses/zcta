/* 
 * File:   Point.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 1, 2009, 3:27 PM
 */

#ifndef _POINT_HPP
#define	_POINT_HPP

#include "Rational.hpp"
#include "boost/operators.hpp"
#include <utility>

template<typename T>
class basic_point : boost::totally_ordered< basic_point<T> > {
private:
	T x_, y_;
public:
	typedef T reptype;

	basic_point(const T& x = T(), const T& y = T()) : x_(x), y_(y) {}
	//allow natural conversions for e.g. basic_point(1, 2)
	template<typename A, typename B>
	basic_point(const A& x, const B& y) : x_(reptype(x)), y_(reptype(y)) {}
	//default copy ctor is fine too
#ifdef MOVE
	basic_point(T&& x, T&& y) : x_(std::move(x)), y_(std::move(y)) {}

	//default copy assignment is fine
	basic_point& operator=(basic_point&& pt) {
		x_ = std::move(pt.x_);
		y_ = std::move(pt.y_);
		return *this;
	}
#endif

	const reptype& x() const {
		return x_;
	}
	const reptype& y() const {
		return y_;
	}
	double distanceTo(const basic_point& pt) const {
		reptype dx = x() - pt.x();
		reptype dy = y() - pt.y();
		reptype dSq = dx*dx + dy*dy;
		return sqrt(boost::rational_cast<double>(dSq));
	}

	friend std::ostream& operator<<(std::ostream& out, const basic_point<T>& pt) {
		return out << "[" << pt.x_ << ", " << pt.y_ << "]";
	}
};

template<typename T>
bool operator==(const basic_point<T>& a, const basic_point<T>& b) {
	return a.x() == b.x() && a.y() == b.y();
}
template<typename T>
bool operator<(const basic_point<T>& a, const basic_point<T>& b) {
	return a.x() < b.x() && a.y() < b.y();
}

template<typename T>
std::size_t hash_value(const basic_point<T>& pt) {
	size_t seed = 0;
	boost::hash_combine(seed, pt.x());
	boost::hash_combine(seed, pt.y());
	return seed;
}

typedef basic_point<Rational> Point;

#endif	/* _POINT_HPP */

