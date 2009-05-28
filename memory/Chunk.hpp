/* 
 * File:   Chunk.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 6, 2009, 9:19 PM
 */

#ifndef _CHUNK_HPP
#define	_CHUNK_HPP

#include <cstddef>

struct Chunk {
	unsigned char firstAvail;
	unsigned char blocksFree;
	unsigned char* data;
	//
	void init(std::size_t blockSize_, unsigned char blocks);
	void destroy();
	void* alloc(std::size_t blockSize);
	void free(void* ptr, std::size_t blockSize);
	bool full();
	bool empty(unsigned char blocksPerChunk);
	bool responsibleFor(void* ptr, std::size_t blockSize, unsigned char blocksPerChunk);
};

#endif	/* _CHUNK_HPP */

