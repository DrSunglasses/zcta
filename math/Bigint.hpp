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

std::size_t hash_value(const Bigint& f);

Bigint pow(const Bigint& base, unsigned int exp);

#endif	/* BIGINT_HPP */

