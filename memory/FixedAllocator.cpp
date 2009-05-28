#include "FixedAllocator.hpp"
#include <iostream>

FixedAllocator::FixedAllocator(std::size_t blockSize_, unsigned char blocksPerChunk_)
		: blockSize(blockSize_), blocksPerChunk(blocksPerChunk_), lastAlloc(0), lastFree(0) {
	Chunk c;
	c.init(blockSize, blocksPerChunk);
	chunks.push_back(c);
	lastAlloc = &chunks[0];
	lastFree = &chunks[0];
}

FixedAllocator::~FixedAllocator() {
	for (std::vector<Chunk>::size_type i = 0; i < chunks.size(); i++)
		chunks[i].destroy();
}

void* FixedAllocator::allocate() {
	if (lastAlloc->full()) {
		//linear search for free space
		for (std::vector<Chunk>::iterator i = chunks.begin(), end = chunks.end();; ++i)
			if (i == end) { //allocate a new chunk (note that we've been through the loop if we get here)
				Chunk c;
				c.init(blockSize, blocksPerChunk);
				chunks.push_back(c);
				lastAlloc = &chunks.back();
				lastFree = lastAlloc; //we may have invalidated this pointer
				break;
			} else if (!(i->full())) {
				lastAlloc = &*i;
				break;
			}
	}
	return lastAlloc->alloc(blockSize);
}

void FixedAllocator::deallocate(void* ptr) {
	if (lastFree->responsibleFor(ptr, blockSize, blocksPerChunk))
		return lastFree->free(ptr, blockSize);
	findForDealloc(ptr);
	lastFree->free(ptr, blockSize);
	//TODO: release empty chunks if there's more than one empty one
}

std::size_t FixedAllocator::size() const {
	return blockSize;
}

void FixedAllocator::findForDealloc(void* ptr) {
	Chunk* down = lastFree - 1;
	Chunk* up = lastFree + 1;
	Chunk* downLimit = &chunks.front();
	Chunk* upLimit = &chunks.back();
	
	while (true) {
		if (down >= downLimit) {
			if (down->responsibleFor(ptr, blockSize, blocksPerChunk)) {
				lastFree = down;
				return;
			}
			--down;
		}
		if (up <= upLimit) {
			if (up->responsibleFor(ptr, blockSize, blocksPerChunk)) {
				lastFree = up;
				return;
			}
			++up;
		}
	}
}
