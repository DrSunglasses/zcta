#include "Line.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>

Line::Line(const Point& a, const Point& b) { //TODO: convert to initializer-list?
	Rational dx = a.x() - b.x(), dy = a.y() - b.y();
	Rational C = a.y() * dx - a.x() * dy;
	A = dy / C;
	B = dx / C;
	theta_ = atan2(dy, dx);
	bottomSquared = sqrt(rational_cast(A * A + B * B));
}

bool Line::vertical() const {
	return B == 0;
}

bool Line::horizontal() const {
	return A == 0;
}

Point Line::intersectionWith(const Line& other) const {
	if (parallelTo(other))
		throw std::logic_error("parallel lines: " + boost::lexical_cast<std::string>(*this) + ", " + boost::lexical_cast<std::string>(other));
	if (*this == other)
		throw std::logic_error("coincident lines: " + boost::lexical_cast<std::string>(*this) + ", " + boost::lexical_cast<std::string>(other));
	Rational bottom = A * other.B - other.A*B;
	Rational topX = B - other.B;
	Rational topY = A - other.A;
	if (bottom == 0 && (topX == 0 || topY == 0))
		throw std::logic_error("UNEXPECTED coincident lines: " + boost::lexical_cast<std::string>(*this) + ", " + boost::lexical_cast<std::string>(other));
	if (bottom == 0)
		throw std::logic_error("UNEXPECTED parallel lines: " + boost::lexical_cast<std::string>(*this) + ", " + boost::lexical_cast<std::string>(other));
	return Point(topX / bottom, topY / bottom);
}

bool Line::parallelTo(const Line& other) const {
	Rational us = theta(), them = other.theta();
	while (us < 0)
		us += pi();
	while (us >= pi())
		us -= pi();
	while (them < 0)
		them += pi();
	while (them >= pi())
		them -= pi();
	return us == them;
}

double Line::distanceTo(const Point& pt) const {
	Rational top = abs(A * pt.x() + B * pt.y() + 1);
	return rational_cast(top) / (bottomSquared);
}

Rational Line::theta() const {
	return theta_;
}

std::size_t hash_value(const Line& pt) {
	std::size_t seed = 0;
	boost::hash_combine(seed, pt.A);
	boost::hash_combine(seed, pt.B);
	return seed;
}

bool operator==(const Line& l, const Line& r) {
	return l.A == r.A && l.B == r.B;
}

bool operator!=(const Line& l, const Line& r) {
	return !(l == r);
}

std::ostream& operator<<(std::ostream& out, const Line& pt) {
	return out << "[" << pt.A << ", " << pt.B << ", " << pt.theta() << "]";
}
