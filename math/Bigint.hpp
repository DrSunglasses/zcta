/* 
 * File:   Bigint.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on April 21, 2009, 10:08 PM
 */

#ifndef BIGINT_HPP
#define	BIGINT_HPP

#include <gmpxx.h>
#include <boost/functional/hash_fwd.hpp>

typedef mpz_class Bigint;

inline std::size_t hash_value(const Bigint& f) {
	//hash a Bigint by hashing the values of its limbs together
	mpz_srcptr underlier = f.get_mpz_t();
	std::size_t seed = 0;
	for (int iterMax = mpz_size(underlier), i = 0; i < iterMax; i++)
		boost::hash_combine(seed, mpz_getlimbn(underlier, i));
	return seed;
}

#ifdef OLD_BIGINT

#include "gmp.h"
#include <limits>
#include <iostream>
#include <boost/functional/hash.hpp>

class Bigint {
private:
	mpz_t z;
public:
	Bigint() {
		mpz_init(z);
	}
	Bigint(long a) {
		mpz_init_set_si(z, a);
	}
	Bigint(const Bigint& other) {
		mpz_init_set(z, other.z);
	}
#ifdef MOVE
	Bigint(Bigint&& other) {
		z[0] = other.z[0];
		other.z[0]._mp_d = 0;
	}
#endif
	
	~Bigint() {
		mpz_clear(z);
	}

	Bigint& operator=(const Bigint& other) {
		mpz_set(z, other.z);
		return *this;
	}
#ifdef MOVE
	Bigint& operator=(Bigint&& other) {
		if (this != &other) {
			mpz_clear(z);
			z[0] = other.z[0];
			other.z[0]._mp_d = 0;
		}
		return *this;
	}
#endif

	double toDouble() {
		return mpz_get_d(z);
	}

	bool operator==(const Bigint& other) const {
		return mpz_cmp(z, other.z) == 0;
	}
	bool operator!=(const Bigint& other) const {
		return !(*this == other);
	}
	bool operator<(const Bigint& other) const {
		return mpz_cmp(z, other.z) < 0;
	}
	bool operator>(const Bigint& other) const {
		return mpz_cmp(z, other.z) > 0;
	}
	bool operator<=(const Bigint& other) const {
		return mpz_cmp(z, other.z) <= 0;
	}
	bool operator>=(const Bigint& other) const {
		return mpz_cmp(z, other.z) >= 0;
	}

	Bigint operator+(const Bigint& other) const {
		Bigint temp;
		mpz_add(temp.z, z, other.z);
		return temp;
	}
	Bigint operator-(const Bigint& other) const {
		Bigint temp;
		mpz_sub(temp.z, z, other.z);
		return temp;
	}
	Bigint operator*(const Bigint& other) const {
		Bigint temp;
		mpz_mul(temp.z, z, other.z);
		return temp;
	}
	Bigint operator/(const Bigint& other) const {
		Bigint temp;
		mpz_div(temp.z, z, other.z);
		return temp;
	}
	Bigint operator%(const Bigint& other) const {
		Bigint temp;
		mpz_mod(temp.z, z, other.z);
		return temp;
	}
	Bigint operator-() const {
		Bigint temp;
		mpz_neg(temp.z, z);
		return temp;
	}

	Bigint& operator+=(const Bigint& other) {
		mpz_add(z, z, other.z);
		return *this;
	}
	Bigint& operator-=(const Bigint& other) {
		mpz_sub(z, z, other.z);
		return *this;
	}
	Bigint& operator*=(const Bigint& other) {
		mpz_mul(z, z, other.z);
		return *this;
	}
	Bigint& operator/=(const Bigint& other) {
		mpz_div(z, z, other.z);
		return *this;
	}
	Bigint& operator%=(const Bigint& other) {
		mpz_mod(z, z, other.z);
		return *this;
	}

	Bigint& operator--() {
		mpz_sub_ui(z, z, 1);
		return *this;
	}
	Bigint operator--(int) {
		Bigint copy(*this);
		--(*this);
		return copy;
	}
	bool operator!() const {
		return mpz_sgn(z) == 0;
	}

	friend Bigint gcd(const Bigint& a, const Bigint& b);
	friend void swap(Bigint& a, Bigint& b);
	friend std::ostream& operator<<(std::ostream& out, const Bigint& bigint);
	friend std::istream& operator>>(std::istream& in, Bigint& bigint);
	friend std::size_t hash_value(const Bigint& b);
};

inline std::ostream& operator<<(std::ostream& out, const Bigint& bigint) {
	return out << bigint.z;
}

inline std::istream& operator>>(std::istream& in, Bigint& bigint) {
	return in >> bigint.z;
}

inline Bigint gcd(const Bigint& a, const Bigint& b) {
	Bigint retval;
	mpz_gcd(retval.z, a.z, b.z);
	return retval;
}

inline void swap(Bigint& a, Bigint& b) {
	mpz_swap(a.z, b.z);
}

inline std::size_t hash_value(const Bigint& b) {
	std::size_t seed = 0;
	for (int iterMax = mpz_size(b.z), i = 0; i < iterMax; i++)
		boost::hash_combine(seed, mpz_getlimbn(b.z, i));
	return seed;
}

namespace std {
	inline void swap(Bigint& a, Bigint& b) {
		::swap(a, b);
	}
	template<>
	struct numeric_limits<Bigint> {
		static const bool is_specialized = true;
		static const bool is_signed = true;
		static const bool is_exact = true;
		static const int radix = 10;
		static const int digits = 0;
		static const int digits10 = 0;
	};
}

#endif //OLD_BIGINT

#endif	/* BIGINT_HPP */

