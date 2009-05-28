/* 
 * File:   ZCTALine.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 11, 2009, 9:41 PM
 */

#ifndef _ZCTALINE_HPP
#define	_ZCTALINE_HPP

#include <stdexcept>
#include <boost/unordered_set.hpp>
#include "ZCTASegment.hpp"

class ZCTALine : public Line {
	boost::unordered_set<ZCTASegment> segments;
public:
	typedef boost::unordered_set<ZCTASegment>::const_iterator iterator;
	ZCTALine(const Point& a, const Point& b) : Line(a, b) {
	}
	ZCTALine(const Line& line) : Line(line) {
	}
	void add(const ZCTASegment& seg) {
		if (!segments.insert(seg).second)
			throw std::logic_error("Duplicate segment added to line");
	}
	void remove(const ZCTASegment& seg) {
		if (!segments.erase(seg))
			throw std::logic_error("Attempted erase of segment not on line");
	}
	bool empty() const {
		return segments.empty();
	}
	iterator begin() const {
		return segments.cbegin();
	}
	iterator end() const {
		return segments.cend();
	}
};

inline std::size_t hash_value(const ZCTALine& zl) {
	boost::hash<Line> hasher;
	return hasher(zl);
}

#endif	/* _ZCTALINE_HPP */

