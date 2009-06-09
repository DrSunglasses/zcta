#include "Rational.hpp"
#include <boost/functional/hash.hpp>
#include <iostream>

std::size_t hash_value(const Rational& r) {
	std::size_t seed = 0;
	boost::hash_combine(seed, r.get_num());
	boost::hash_combine(seed, r.get_den());
	return seed;
}

double rational_cast(const Rational& r) {
	return r.get_d();
}

Bigint& numerator_ref(Rational& r) {
	return r.get_num();
}

const Bigint& numerator_ref(const Rational& r) {
	return r.get_num();
}

Bigint& denominator_ref(Rational& r) {
	return r.get_den();
}

const Bigint& denominator_ref(const Rational& r) {
	return r.get_den();
}

namespace {
	const int POWER_SERIES_ITERATIONS = 15;
	Rational atan_power_series(const Rational& r) {
		Rational frac(1, 1);
		Bigint& numerator = numerator_ref(frac);
		Bigint& denominator = denominator_ref(frac);
		int power = 1;
		Rational accumulator;
		for (int i = 0; i < POWER_SERIES_ITERATIONS; i++) {
			accumulator += frac*(pow(r, power));
			numerator = -numerator;
			denominator += 2;
			power += 2;
		}
		return accumulator;
	}
}

const Rational& pi() {
	const static Rational PI = 4*(4*atan_power_series(Rational(1, 5)) - atan_power_series(Rational(1, 239)));
	return PI;
}

Rational atan(const Rational& x) {
	Rational abx = abs(x);
	if (abx < 1)
		return atan_power_series(x);
	if (abx == 1)
		return pi()/4;
	//atan(x) = pi/2 - atan(1/x)
	return pi()/2 - atan(1/x);
}

Rational pow(const Rational& base, unsigned int exp) {
	Rational r(pow(numerator_ref(base), exp), pow(denominator_ref(base), exp));
	r.canonicalize();
	return r;
}
