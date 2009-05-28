/* 
 * File:   ZCTASegment.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 8, 2009, 9:49 PM
 */

#ifndef _ZCTASEGMENT_HPP
#define	_ZCTASEGMENT_HPP

#include "../math/LineSegment.hpp"
#include "ZCTA.hpp"

class ZCTASegment : public LineSegment {
	ZCTA zcta_;
public:
	ZCTASegment(const LineSegment& seg, const ZCTA& z) : LineSegment(seg.a(), seg.b()), zcta_(z) {
	}
	const ZCTA& zcta() const {
		return zcta_;
	}
	bool operator==(const ZCTASegment& a) const {
		return LineSegment::operator==(a) && zcta() == a.zcta();
	}
	bool operator!=(const ZCTASegment& a) const {
		return !(*this == a);
	}
	bool operator<(const ZCTASegment& a) const {
		return LineSegment::operator<(a);
	}
};

inline std::size_t hash_value(const ZCTASegment& z) {
	std::size_t seed = 0;
	boost::hash<LineSegment> hasher;
	boost::hash_combine(seed, hasher(z));
	boost::hash_combine(seed, z.zcta());
	return seed;
}

#endif	/* _ZCTASEGMENT_HPP */

