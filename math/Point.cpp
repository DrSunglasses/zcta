#include "Point.hpp"
#include <cmath>
#include <boost/functional/hash.hpp>

double Point::distanceTo(const Point& pt) const {
	Rational dx = x() - pt.x();
	Rational dy = y() - pt.y();
	Rational dSq = dx*dx + dy*dy;
	return std::sqrt(rational_cast(dSq));
}

std::size_t hash_value(const Point& pt) {
	size_t seed = 0;
	boost::hash_combine(seed, pt.x());
	boost::hash_combine(seed, pt.y());
	return seed;
}

std::ostream& operator<<(std::ostream& out, const Point& pt) {
	return out << pt.x() << " " << pt.y();
}

std::istream& operator>>(std::istream& in, Point& pt) {
	return in >> pt.x_ >> pt.y_;
}

bool operator==(const Point& a, const Point& b) {
	return a.x() == b.x() && a.y() == b.y();
}

bool operator<(const Point& a, const Point& b) {
	return a.y() < b.y() || (a.y() == b.y() && a.x() < b.x());
}
