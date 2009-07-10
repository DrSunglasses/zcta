#ifndef UNORDEREDMAPSTREAMING_HPP
#define UNORDEREDMAPSTREAMING_HPP

#include <boost/unordered_map.hpp>
#include <iosfwd>
#include <utility> //for std::make_pair()

//implements stream operators for boost::unordered_map<K, V>

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const boost::unordered_map<K, V>& map) {
	out << map.size();
	for (typename boost::unordered_map<K, V>::const_iterator i = map.cbegin(); i != map.cend(); i++) {
		out << " " << i->first << " " << i->second;
	}
	return out;
}

template<typename K, typename V>
std::istream& operator>>(std::istream& in, boost::unordered_map<K, V>& map) {
	map.clear();
	int elements = -1;
	in >> elements;
	for (int i = 0; i < elements; i++) {
		K k;
		V v;
		in >> k >> v;
		map.insert(std::make_pair(k, v));
	};
	return in;
}


#endif //UNORDEREDMAPSTREAMING_HPP
