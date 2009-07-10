#ifndef VECTORSTREAMING_HPP
#define VECTORSTREAMING_HPP

//defines stream operators for std::vector<T, Alloc>
//TODO: this may be technically non-portable because these templates are in
//the global namespace; adding to std causes undefined behavior except for
//specializations of existing templates (such as std::swap) for user-defined
//types, so I can't put it in std either.  not sure what to do, but this
//works for me oin GCC 4.4.
#include <vector>
#include <iosfwd>

template<typename T, typename Alloc>
std::ostream& operator<<(std::ostream& out, const std::vector<T, Alloc>& vec) {
	out << vec.size();
	for (int i = 0; i < vec.size(); i++)
		out << " " << vec[i];
	return out;
}

template<typename T, typename Alloc>
std::istream& operator>>(std::istream& in, std::vector<T, Alloc>& vec) {
	vec.clear();
	int elements = -1;
	in >> elements;
	for (int i = 0; i < elements; i++) {
		T t;
		in >> t;
		vec.push_back(t);
	}
	return in;
}

#endif //VECTORSTREAMING_HPP
