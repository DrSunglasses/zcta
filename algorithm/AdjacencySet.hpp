/* 
 * File:   AdjacencySet.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 9, 2009, 3:10 PM
 */

#ifndef _ADJACENCYSET_HPP
#define	_ADJACENCYSET_HPP

#include <boost/unordered_map.hpp>
#include "ZCTA.hpp"

class AdjacencySet {
	boost::unordered_multimap<ZCTA, ZCTA> map;
public:
	typedef boost::unordered_multimap<ZCTA, ZCTA>::const_iterator iterator;
	void addAdjacency(const ZCTA& a, const ZCTA& b) {
		if (a == b)
			throw std::logic_error("Self-adjacency?");
		if (a < b)
			map.insert(std::make_pair(a, b));
		else
			map.insert(std::make_pair(b, a));
	}
	bool areAdjacent(const ZCTA& a, const ZCTA& b) const {
		if (a < b) {
			std::pair<iterator, iterator> foo = map.equal_range(a);
			for (; foo.first != foo.second; ++foo.first)
				if (foo.first->first == a && foo.first->second == b)
					return true;
			return false;
		} else {
			std::pair<iterator, iterator> foo = map.equal_range(b);
			for (; foo.first != foo.second; ++foo.first)
				if (foo.first->first == b && foo.first->second == a)
					return true;
			return false;
		}
		return false;
	}
	void addAll(const AdjacencySet& s) {
		for (iterator i = s.begin(); i != s.end(); ++i)
			addAdjacency(i->first, i->second);
	}
	iterator begin() const {
		return map.begin();
	}
	iterator end() const {
		return map.end();
	}
	int size() const {
		return map.size();
	}
};

#endif	/* _ADJACENCYSET_HPP */

