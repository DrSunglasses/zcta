/* 
 * File:   SmallAllocator.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 8, 2009, 3:27 PM
 */

#ifndef _SMALLALLOCATOR_HPP
#define	_SMALLALLOCATOR_HPP

#include "FixedAllocator.hpp"
#include <boost/noncopyable.hpp>
#include <vector>

class SmallAllocator : boost::noncopyable {
	std::vector<FixedAllocator*> allocators;
public:
	SmallAllocator();
	~SmallAllocator();
	void* allocate(std::size_t bytes);
	void deallocate(void* ptr, std::size_t bytes);
};

#endif	/* _SMALLALLOCATOR_HPP */

