
#include "Chunk.hpp"

void Chunk::init(std::size_t blockSize, unsigned char blocks) {
	firstAvail = 0;
	blocksFree = blocks;
	data = new unsigned char[blocks*blockSize];
	unsigned char i = 0;
	for (unsigned char* p = data; i < blocks; p += blockSize)
		*p = ++i;
}

void Chunk::destroy() {
	delete[] data;
	data = 0;
	firstAvail = 255;
	blocksFree = 0;
}

void* Chunk::alloc(std::size_t blockSize) {
	unsigned char* retval = data + firstAvail * blockSize;
	firstAvail = *retval;
	--blocksFree;
	return static_cast<void*>(retval);
}

void Chunk::free(void* ptr, std::size_t blockSize) {
	unsigned char* block = static_cast<unsigned char*>(ptr);
	*block = firstAvail;
	firstAvail = static_cast<unsigned char>((block - data)/blockSize);
	++blocksFree;
}

bool Chunk::full() {
	return blocksFree == 0;
}

bool Chunk::empty(unsigned char blocksPerChunk) {
	return blocksFree == blocksPerChunk;
}

bool Chunk::responsibleFor(void* ptr, std::size_t blockSize, unsigned char blocksPerChunk) {
	return ptr >= data && ptr < (data + blockSize*blocksPerChunk);
}

