/* 
 * File:   FixedAllocator.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 7, 2009, 11:27 AM
 */

#ifndef _FIXEDALLOCATOR_HPP
#define	_FIXEDALLOCATOR_HPP

#include "Chunk.hpp"
#include <vector>
#include <boost/noncopyable.hpp>

class FixedAllocator : boost::noncopyable {
	std::size_t blockSize;
	unsigned char blocksPerChunk;
	Chunk* lastAlloc;
	Chunk* lastFree;
	std::vector<Chunk> chunks;
public:
	FixedAllocator(std::size_t blockSize_, unsigned char blocksPerChunk_);
	~FixedAllocator();
	void* allocate();
	void deallocate(void* ptr);
	std::size_t size() const;
private:
	void findForDealloc(void* ptr);
};

#endif	/* _FIXEDALLOCATOR_HPP */

