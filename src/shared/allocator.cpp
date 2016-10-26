#include "allocator.h"

#include <vector>

struct FrameAllocatorData
{
	size_t _bufferSize;
	std::vector<char *> _buffers;
	size_t _bufferIndex;
	size_t _pos;
	void *_last;
	size_t _prevPos;
	size_t _count;
};

FrameAllocator::FrameAllocator(size_t bufferSize)
{
	_data = new FrameAllocatorData;
	_data->_bufferSize = bufferSize;
	char *buffer = new char[bufferSize];
	_data->_buffers.push_back(buffer);
	reset();
}

FrameAllocator::~FrameAllocator()
{
	for (size_t i = 0; i < _data->_buffers.size(); i++)
	{
		delete[] _data->_buffers[i];
	}
}

void FrameAllocator::reset()
{
	_data->_pos = 0;
	_data->_bufferIndex = 0;
	_data->_prevPos = 0;
	_data->_last = NULL;
	_data->_count = 0;
}

void *FrameAllocator::allocate(size_t size)
{
	if (_data->_pos + size > _data->_bufferSize)
	{
		if (size > _data->_bufferSize)
			return NULL;
		
		_data->_bufferIndex++;
		if (_data->_bufferIndex == _data->_buffers.size())
		{
			char *buffer = new char[_data->_bufferSize];
			_data->_buffers.push_back(buffer);
		}
		_data->_pos = 0;
	}

	char *mem = _data->_buffers[_data->_bufferIndex] + _data->_pos;
	
	_data->_last = mem;
	_data->_prevPos = _data->_pos;

	_data->_pos += size;
	_data->_count++;
	return mem;
}

void FrameAllocator::free(void *ptr)
{
	//only free the simple cases
	if (ptr == _data->_last)
	{
		_data->_pos = _data->_prevPos;
	}
	_data->_count--;
}

AllocatorStats FrameAllocator::stats()
{
	AllocatorStats s;
	s.allocationCount = _data->_count;
	return s;
}
