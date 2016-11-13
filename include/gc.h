#ifndef GC_H
#define GC_H

#include <stdint.h>

//define GC_ASSERT_FUNC to verify pointers during the memory patching
#ifndef GC_ASSERT_FUNC
#define GC_ASSERT_FUNC(cond, formatStr, ...)
#endif

class GcExpression
{
	char expression[512];
};

template<typename T> class GcArray
{
public:
	size_t size() const { return _size; }

	T& operator[](size_t i) { return _elements[i]; };
	const T& operator[](size_t i) const { return _elements[i]; };

private:
	size_t _size;
	T *_elements;
};

#ifndef CUSTOM_VECTOR_TYPES
struct Vec4
{
	float x;
	float y;
	float z;
	float w;
};

struct Vec3
{
	float x;
	float y;
	float z;
};

struct Vec2
{
	float x;
	float y;
};
#endif


struct GcBlockHeader
{
	size_t blockSize; //in bytes
	size_t objectCount;
	size_t objectTableOffset;
	size_t objectOffset;
	size_t stringTableOffset;

	//first pointer to patch when loading, each pointer location is 2 uint32_t, 
	//the first one contains the offset this pointer is pointing to, 
	//the second is the offset of the next pointer or 0
	size_t firstPointerOffset;

	//same for vtable pointer patching, each slot contains the location of the next vtable pointer to patch and the gcObject index
	size_t firstVTablePointerOffset;

	//same for strings, strings could use firstPointerOffset
	size_t firstStringPointerOffset;
};

struct GcObjectHeader
{
	size_t offset;
	size_t size;
	uint32_t typeID;
	char dbgTypeName[32];
	char name[76];
};

#define _GC_CHECK_FIRST_OFFSET(offset, name) \
	GC_ASSERT_FUNC((offset) == 0x00000000ffffffff || (header->objectOffset + (offset)) < header->blockSize, \
	"Invalid data: " ##name " pointer patch offset %I64u referenced from first offset points to an invalid location", offset);

#define GC_CHECK_FIRST_OFFSET _GC_CHECK_FIRST_OFFSET(patchOffset, "pointer")

#define _GC_CHECK_OFFSET(offset, name) \
	GC_ASSERT_FUNC((offset) == 0x00000000ffffffff || (header->objectOffset + (offset)) < header->blockSize, \
	"Invalid data: " ##name " pointer patch offset %I64u referenced from offset %I64u points to an invalid location", offset , patchOffset);

#define GC_CHECK_OFFSET _GC_CHECK_OFFSET((*patchAddress) & 0x00000000ffffffff, "pointer")

inline void gcPatchPointerChain(uint64_t patchOffset, GcBlockHeader *header)
{
	GC_CHECK_FIRST_OFFSET;
	char *bin = (char *)header;
	while (patchOffset != 0x00000000ffffffff)
	{
		uint64_t *patchAddress = (uint64_t *)(bin + header->objectOffset + patchOffset);
		GC_CHECK_OFFSET;
		patchOffset = (*patchAddress) & 0x00000000ffffffff;
		uint64_t targetOffset = ((*patchAddress) >> 32);
		uint64_t *targetAddress = (uint64_t *)(bin + header->objectOffset + targetOffset);
		*patchAddress = (uint64_t)targetAddress;
	}
}

inline void gcPatchVTablePointerChain(uint64_t patchOffset, GcBlockHeader *header, const void * const * vPtrTable)
{
	GC_CHECK_FIRST_OFFSET;
	char *bin = (char *)header;
	while (patchOffset != 0x00000000ffffffff)
	{
		uint64_t *patchAddress = (uint64_t *)(bin + header->objectOffset + patchOffset);
		GC_CHECK_OFFSET;
		patchOffset = (*patchAddress) & 0x00000000ffffffff;
		uint64_t objectIndex = ((*patchAddress) >> 32);
		const void *vPtr = vPtrTable[objectIndex];
		*patchAddress = (uint64_t)vPtr;
	}
}


inline void gcPatchBlockPointers(GcBlockHeader *header, const void * const * vPtrTable)
{
	//patch object pointers
	gcPatchPointerChain(header->firstPointerOffset, header);

	//patch vtable pointers
	gcPatchVTablePointerChain(header->firstVTablePointerOffset, header, vPtrTable);

	//patch strings
	gcPatchPointerChain(header->firstStringPointerOffset, header);
}

#endif
