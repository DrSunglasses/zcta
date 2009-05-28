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
#include "../math/Point.hpp"

class ZCTA {
private:
	std::string id_;
	Point interiorPoint_;
public:
	ZCTA(const std::string& id, const Point& interiorPoint) : id_(id), interiorPoint_(interiorPoint) {
		if (id.length() != 5)
			throw std::logic_error("Bad ZCTA id: "+id);
	}
#ifdef MOVE
	ZCTA(std::string&& id, Point&& interiorPoint) : id_(std::move(id)), interiorPoint_(std::move(interiorPoint_)) {
	}
	ZCTA(ZCTA&& z) : id_(std::move(z.id_)), interiorPoint_(std::move(z.interiorPoint_)) {
	}

	ZCTA& operator=(ZCTA&& z) {
		id_ = std::move(z.id_);
		interiorPoint_ = std::move(z.interiorPoint_);
		return *this;
	}
#endif

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
};

inline std::size_t hash_value(const ZCTA& z) {
	return boost::hash_value(z.id());
}

#endif	/* _ZCTA_HPP */

