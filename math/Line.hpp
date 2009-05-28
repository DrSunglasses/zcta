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
#include <boost/optional.hpp>

template<typename T>
class basic_line {
public:
	typedef typename T::reptype reptype;
	typedef T point;
private:
	reptype A, B; //Ax + By = 1
	mutable boost::optional<double> theta_, bottomSquared;
public:
	basic_line(const point& a, const point& b) { //TODO: convert to initializer-list?
		reptype dx = a.x() - b.x(), dy = a.y() - b.y();
		reptype C = -a.x()*dy + a.y()*dx;
		A = dy/C;
		B = dx/C;
	}
	bool vertical() const {
		return !B;
	}
	bool horizontal() const {
		return !A;
	}
//	const reptype slope() const {
//		//TODO: possible candidate for caching or eagerly initializing
//		return A/B;
//	}
	point intersectionWith(const basic_line& other) const {
		reptype bottom = A*other.B - other.A*B;
		reptype topX = B - other.B;
		reptype topY = A - other.A;
		if ((!topX||!topY) && !bottom)
			throw std::logic_error("coincident lines");
		if (!bottom)
			throw std::logic_error("parallel lines");
		return T(topX/bottom, topY/bottom);
	}
	bool parallelTo(const basic_line& other) const {
		return theta() == other.theta();
	}
	double distanceTo(const point& pt) const {
		if (!bottomSquared)
			bottomSquared = sqrt(boost::rational_cast<double>(A*A + B*B));
		reptype top = abs(A*pt.x() + B*pt.y() + 1);
		return boost::rational_cast<double>(top)/(*bottomSquared);
	}
	double theta() const {
		if (theta_)
			return *theta_;
		if (vertical())
			return 3.14159265358979323846/2;
		double rhs = boost::rational_cast<double>(A/B);
		double theta = std::atan(rhs);
		if (theta < 0)
			theta += 3.14159265358979323846; //normalize to [0, pi]
		theta_ = theta;
		return theta;
	}
	bool operator==(const basic_line& other) const {
		return A == other.A && B == other.B;
	}
	bool operator!=(const basic_line& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator<<(std::ostream& out, const basic_line<T>& pt) {
		return out << "[" << pt.A << ", " << pt.B << "]";
	}
	friend std::size_t hash_value(const basic_line<T>& pt) {
		std::size_t seed = 0;
		boost::hash_combine(seed, pt.A);
		boost::hash_combine(seed, pt.B);
		return seed;
	}
};

typedef basic_line<Point> Line;

#endif	/* _LINE_HPP */

