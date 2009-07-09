/* 
 * File:   ZCTA.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 3, 2009, 3:39 PM
 */

#ifndef _ZCTA_HPP
#define	_ZCTA_HPP

#include <string>
#include <utility>
#include <iosfwd>
#include "../math/Point.hpp"
#include <boost/functional/hash.hpp>

class ZCTA {
private:
	std::string id_;
	Point interiorPoint_;
public:
	ZCTA() : id_("-BUG-"), interiorPoint_() {
		//only provided to be InputStreamable, not for general use
	}
	ZCTA(const std::string& id, const Point& interiorPoint) : id_(id), interiorPoint_(interiorPoint) {
		if (id.length() != 5)
			throw std::logic_error("Bad ZCTA id: "+id);
	}

	const std::string& id() const {
		return id_;
	}
	const Point& interiorPoint() const {
		return interiorPoint_;
	}

	bool operator==(const ZCTA& z) const {
		return id() == z.id();
	}
	bool operator!=(const ZCTA& z) const {
		return !(*this == z);
	}

	bool operator<(const ZCTA& z) const {
		return id() < z.id();
	}

	friend std::istream& operator>>(std::istream& in, ZCTA& z) {
		return in >> z.id_ >> z.interiorPoint_;
	}
};

inline std::size_t hash_value(const ZCTA& z) {
	return boost::hash<std::string>()(z.id());
}

inline std::ostream& operator<<(std::ostream& out, const ZCTA& z) {
	return out << z.id() << " " << z.interiorPoint();
}

#endif	/* _ZCTA_HPP */

