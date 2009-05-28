#include "GMPMemory.hpp"
#include "SmallAllocator.hpp"
#include "gmp.h"
#include <cstring>

static const size_t MAX_SMALL_ALLOC_BYTES = 128;

static SmallAllocator alloc;

void* gmp_malloc(size_t bytes) {
//	if (bytes > MAX_SMALL_ALLOC_BYTES)
//		return static_cast<void*>(new char[bytes]);
	return alloc.allocate(bytes);
}

void gmp_free(void* ptr, size_t bytes) {
//	if (bytes > MAX_SMALL_ALLOC_BYTES)
//		return delete[] static_cast<char*>(ptr);
	alloc.deallocate(ptr, bytes);
}

void* gmp_realloc(void* ptr, size_t oldBytes, size_t newBytes) {
	void* retval = 0;
//	if (newBytes > MAX_SMALL_ALLOC_BYTES)
//		retval = static_cast<void*>(new char[newBytes]);
//	else
		retval = alloc.allocate(newBytes);
	std::memmove(retval, ptr, std::min(oldBytes, newBytes));
//	if (oldBytes > MAX_SMALL_ALLOC_BYTES)
//		delete[] static_cast<char*>(ptr);
//	else
		alloc.deallocate(ptr, oldBytes);
	return retval;
}

void initGMPMemoryFunctions() {
	mp_set_memory_functions(&gmp_malloc, &gmp_realloc, &gmp_free);
}

