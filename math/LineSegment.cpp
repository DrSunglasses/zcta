#include "LineSegment.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <boost/functional/hash.hpp>

LineSegment::LineSegment(const Point& pt1, const Point& pt2) : a_(pt1 < pt2 ? pt1 : pt2), b_(pt1 < pt2 ? pt2 : pt1), line_(a_, b_) {
	if (a_ == b_)
		throw std::logic_error("zero-length segment");
}

double LineSegment::distanceTo(const LineSegment& other) const {
	std::vector<double> vec(8);
	//line-endpt distances
	vec.push_back(line().distanceTo(other.a()));
	vec.push_back(line().distanceTo(other.b()));
	vec.push_back(other.line().distanceTo(a()));
	vec.push_back(other.line().distanceTo(b()));
	//endpt-endpt distances
	vec.push_back(a().distanceTo(other.a()));
	vec.push_back(a().distanceTo(other.b()));
	vec.push_back(b().distanceTo(other.a()));
	vec.push_back(b().distanceTo(other.b()));
	return *std::min_element(vec.begin(), vec.end());
}

bool operator==(const LineSegment& l, const LineSegment& r) {
	return l.a() == r.a() && l.b() == r.b();
}

bool operator!=(const LineSegment& l, const LineSegment& r) {
	return !(l == r);
}

bool operator<(const LineSegment& l, const LineSegment& r) {
	return l.a() < r.a() || (l.a() == r.a() && l.b() < r.b());
}

std::size_t hash_value(const LineSegment& seg) {
	std::size_t seed = 0;
	boost::hash_combine(seed, seg.line());
	boost::hash_combine(seed, seg.a());
	boost::hash_combine(seed, seg.b());
	return seed;
}
