#include "ZCTASegment.hpp"
#include <boost/functional/hash.hpp>

ZCTASegment::ZCTASegment(const LineSegment& seg, const ZCTA& z) : LineSegment(seg.a(), seg.b()), zcta_(z) {
}

bool operator==(const ZCTASegment& l, const ZCTASegment& r) {
	const LineSegment& base_l = static_cast<const LineSegment&>(l);
	const LineSegment& base_r = static_cast<const LineSegment&>(r);
	return base_l == base_r && l.zcta() == r.zcta();
}

bool operator!=(const ZCTASegment& l, const ZCTASegment& r) {
	return !(l == r);
}

bool operator<(const ZCTASegment& l, const ZCTASegment& r) {
	const LineSegment& base_l = static_cast<const LineSegment&>(l);
	const LineSegment& base_r = static_cast<const LineSegment&>(r);
	return base_l < base_r;
}

std::size_t hash_value(const ZCTASegment& z) {
	std::size_t seed = 0;
	boost::hash<LineSegment> hasher;
	boost::hash_combine(seed, hasher(z));
	boost::hash_combine(seed, z.zcta());
	return seed;
}
