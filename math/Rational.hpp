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

std::size_t hash_value(const Rational& r);

double rational_cast(const Rational& r);
Bigint& numerator_ref(Rational& r);
const Bigint& numerator_ref(const Rational& r);
Bigint& denominator_ref(Rational& r);
const Bigint& denominator_ref(const Rational& r);

Rational atan(const Rational& x);
Rational pow(const Rational& base, unsigned int exp);
const Rational& pi();

#endif	/* _RATIONAL_H */

