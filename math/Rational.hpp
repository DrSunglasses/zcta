/* 
 * File:   Rational.h
 * Author: Jeffrey Bosboom
 *
 * Created on April 21, 2009, 9:22 PM
 */

#ifndef _RATIONAL_H
#define	_RATIONAL_H

#include "Bigint.hpp"
#include <boost/functional/hash_fwd.hpp>

typedef mpq_class Rational;

inline std::size_t hash_value(const Rational& r) {
	std::size_t seed = 0;
	boost::hash_combine(seed, r.get_num());
	boost::hash_combine(seed, r.get_den());
	return seed;
}

inline double rational_cast(const Rational& r) {
	return r.get_d();
}

#if 0

class Rational {
private:
	Bigint numerator, denominator;
public:
	Rational() : numerator(0), denominator(1) {
	}
	Rational(long numerato) : numerator(numerato), denominator(1) {
		canonicalize();
	}
	Rational(long numerato, long denominato) : numerator(numerato), denominator(denominato) {
		canonicalize();
	}
	Rational(Bigint numerato) : numerator(numerato), denominator(1) {
		canonicalize();
	}
	Rational(Bigint numerato, Bigint denominato) : numerator(numerato), denominator(denominato) {
		canonicalize();
	}

	Rational& operator=(const Rational& other) {
		numerator = other.numerator;
		denominator = other.denominator;
		return *this;
	}

	bool operator==(const Rational& other) const {
		return numerator == other.numerator && denominator == other.denominator;
	}
	bool operator!=(const Rational& other) const {
		return !(*this == other);
	}
	bool operator<(const Rational& other) const {
		return numerator*other.denominator < other.numerator*denominator;
	}
	bool operator>(const Rational& other) const {
		return numerator*other.denominator > other.numerator*denominator;
	}
	bool operator<=(const Rational& other) const {
		return numerator*other.denominator <= other.numerator*denominator;
	}
	bool operator>=(const Rational& other) const {
		return numerator*other.denominator >= other.numerator*denominator;
	}

	Rational operator+(const Rational& other) const {
		Rational retval(*this);
		retval += other;
		return retval;
	}
	Rational operator-(const Rational& other) const {
		Rational retval(*this);
		retval -= other;
		return retval;
	}
	Rational operator*(const Rational& other) const {
		Rational retval(*this);
		retval *= other;
		return retval;
	}
	Rational operator/(const Rational& other) const {
		Rational retval(*this);
		retval /= other;
		return retval;
	}
	Rational operator-() const {
		return Rational(-numerator, denominator);
	}

	void operator+=(const Rational& other) {
		numerator = numerator*other.denominator + other.numerator*denominator;
		denominator *= other.denominator;
		canonicalize();
	}
	void operator-=(const Rational& other) {
		numerator = numerator*other.denominator - other.numerator*denominator;
		denominator *= other.denominator;
		canonicalize();
	}
	void operator*=(const Rational& other) {
		numerator *= other.numerator;
		denominator *= other.denominator;
		canonicalize();
	}
	void operator/=(const Rational& other) {
		numerator *= other.denominator;
		denominator *= other.numerator;
		canonicalize();
	}

//	Bigint numerator() {
//		return numerator;
//	}
//	Bigint denominator() {
//		return denominator;
//	}
	Rational reciprocal() const {
		return Rational(denominator, numerator);
	}

	friend void swap(Rational& a, Rational& b);
private:
	void canonicalize() {
		const static Bigint zero(0), one(1);
		if (denominator < 0) {
			numerator = -numerator;
			denominator = -denominator;
		}
		if (numerator == zero)
			denominator = one;
		else {
			Bigint divisor = gcd(numerator, denominator);
			numerator /= divisor;
			denominator /= divisor;
		}
	}
};

void swap(Rational& a, Rational& b) {
	swap(a.numerator, b.numerator);
	swap(a.denominator, b.denominator);
}

namespace std {
	void swap(Rational& a, Rational& b) {
		::swap(a, b);
	}
}
#endif //0

#endif	/* _RATIONAL_H */

