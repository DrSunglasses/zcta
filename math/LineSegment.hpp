/* 
 * File:   LineSegment.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 3, 2009, 2:33 PM
 */

#ifndef _LINESEGMENT_HPP
#define	_LINESEGMENT_HPP

#include <vector>
#include <algorithm>
#include <stdexcept>
#include "Line.hpp"
#include <boost/functional/hash_fwd.hpp>

template<typename T>
class basic_segment {
public:
	typedef T line_type;
	typedef typename T::point point;
	typedef typename T::reptype reptype;
private:
	point a_, b_;
	line_type line_;
public:
	basic_segment(const point& pt1, const point& pt2) : a_(pt1 < pt2 ? pt1 : pt2), b_(pt1 < pt2 ? pt2 : pt1), line_(a_, b_) {
		if (a_ == b_)
			throw std::logic_error("zero-length segment");
	}
#ifdef MOVE
	basic_segment(point&& pt1, point&& pt2) : a_(std::move(pt1 < pt2 ? pt1 : pt2)), b_(std::move(pt1 < pt2 ? pt2 : pt1)), line_(a_, b_) {
	}
	basic_segment(basic_segment&& seg) : a_(std::move(seg.a_)), b_(std::move(seg.b_)), line_(std::move(seg.line_)) {
	}

	basic_segment& operator=(basic_segment&& seg) {
		a_ = std::move(seg.a_);
		b_ = std::move(seg.b_);
		line_ = std::move(seg.line_);
		return *this;
	}
#endif

	const line_type& line() const {
		return line_;
	}
	const point& a() const {
		return a_;
	}
	const point& b() const {
		return b_;
	}
	double distanceTo(const basic_segment& other) const {
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

	bool operator==(const basic_segment& other) const {
		return a() == other.a() && b() == other.b();
	}
	bool operator!=(const basic_segment& other) const {
		return !(*this == other);
	}
	bool operator<(const basic_segment& other) const {
		return a() < other.a() || (a() == other.a() && b() < other.b());
	}
};

template<typename T>
std::size_t hash_value(const basic_segment<T>& seg) {
	std::size_t seed = 0;
	boost::hash_combine(seed, seg.line());
	boost::hash_combine(seed, seg.a());
	boost::hash_combine(seed, seg.b());
	return seed;
}

typedef basic_segment<Line> LineSegment;

#endif	/* _LINESEGMENT_HPP */


