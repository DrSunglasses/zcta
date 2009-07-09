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
	ZCTASegment(); //to be InputStreamable
	ZCTASegment(const LineSegment& seg, const ZCTA& z);
	const ZCTA& zcta() const {
		return zcta_;
	}
	
	friend std::istream& operator>>(std::istream& in, ZCTASegment& seg);
};

bool operator==(const ZCTASegment& l, const ZCTASegment& r);
bool operator!=(const ZCTASegment& l, const ZCTASegment& r);
bool operator<(const ZCTASegment& l, const ZCTASegment& r);

std::ostream& operator<<(std::ostream& out, const ZCTASegment& seg);
//operator>> declared as friend above

std::size_t hash_value(const ZCTASegment& z);

#endif	/* _ZCTASEGMENT_HPP */

