#ifndef ALLOCATOR_H
#define ALLOCATOR_H

struct FrameAllocatorData;

struct AllocatorStats
{
	size_t allocationCount;
};

class FrameAllocator
{
public:
	FrameAllocator(size_t bufferSize);
	~FrameAllocator();

	void *allocate(size_t size);
	void free(void *ptr);
	void reset();

	AllocatorStats stats();

private:
	FrameAllocatorData *_data;
};

#endif
