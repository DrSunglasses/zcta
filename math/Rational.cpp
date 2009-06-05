#include "Rational.hpp"
#include <boost/functional/hash.hpp>

std::size_t hash_value(const Rational& r) {
	std::size_t seed = 0;
	boost::hash_combine(seed, r.get_num());
	boost::hash_combine(seed, r.get_den());
	return seed;
}

double rational_cast(const Rational& r) {
	return r.get_d();
}
