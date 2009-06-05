#include "Bigint.hpp"
#include <boost/functional/hash.hpp>

std::size_t hash_value(const Bigint& f) {
	//hash a Bigint by hashing the values of its limbs together
	mpz_srcptr underlier = f.get_mpz_t();
	std::size_t seed = 0;
	for (int iterMax = mpz_size(underlier), i = 0; i < iterMax; i++)
		boost::hash_combine(seed, mpz_getlimbn(underlier, i));
	return seed;
}

