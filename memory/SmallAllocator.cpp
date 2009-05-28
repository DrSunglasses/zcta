
#include <algorithm>

#include "SmallAllocator.hpp"

namespace {
	struct FindSize {
		bool operator()(const FixedAllocator* ptr, const std::size_t bytes) {
			return ptr->size() < bytes;
		}
	};
}

SmallAllocator::SmallAllocator() {
}

SmallAllocator::~SmallAllocator() {
	for (std::vector<FixedAllocator*>::size_type i = 0; i < allocators.size(); i++)
		delete allocators[i];
}

void* SmallAllocator::allocate(std::size_t bytes) {
	std::vector<FixedAllocator*>::iterator i = std::lower_bound(allocators.begin(), allocators.end(), bytes, FindSize());
	if (i != allocators.end())
		return (*i)->allocate();
	FixedAllocator* fa = new FixedAllocator(bytes, 255);
	allocators.insert(i, fa);
	return fa->allocate();
}

void SmallAllocator::deallocate(void* ptr, std::size_t bytes) {
	if (ptr != 0)
		(*std::lower_bound(allocators.begin(), allocators.end(), bytes, FindSize()))->deallocate(ptr);
}
