#ifndef UNORDEREDSETSTREAMING_HPP
#define UNORDEREDSETSTREAMING_HPP

//implement stream operators for boost::unordered_set<T>
#include <boost/unordered_set.hpp>
#include <iosfwd>

template<typename T>
std::ostream& operator<<(std::ostream& out, const boost::unordered_set<T>& set) {
	out << set.size();
	for (typename boost::unordered_set<T>::const_iterator i = set.cbegin(); i != set.cend(); ++i)
		out << " " << *i;
	return out;
}

template<typename T>
std::istream& operator>>(std::istream& in, boost::unordered_set<T>& set) {
	set.clear();
	int elements = -1;
	in >> elements;
	for (int i = 0; i < elements; i++) {
		T t;
		in >> t;
		set.insert(t);
	}
	return in;
}

#endif //UNORDEREDSETSTREAMING_HPP
