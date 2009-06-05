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
	ZCTASegment(const LineSegment& seg, const ZCTA& z);
	const ZCTA& zcta() const {
		return zcta_;
	}
};

bool operator==(const ZCTASegment& l, const ZCTASegment& r);
bool operator!=(const ZCTASegment& l, const ZCTASegment& r);
bool operator<(const ZCTASegment& l, const ZCTASegment& r);

std::size_t hash_value(const ZCTASegment& z);

#endif	/* _ZCTASEGMENT_HPP */

