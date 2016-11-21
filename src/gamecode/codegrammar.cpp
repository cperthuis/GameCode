#include "codegrammar.h"
#include <string.h>
#include <new>

#include <shared/allocator.h>
#include <stdio.h>

#define USE_FRAME_ALLOCATOR

extern FrameAllocator *_allocator;

/*static FrameAllocator *_allocator = NULL;

void initializeAllocator(size_t bufferSize)
{
	_allocator = new FrameAllocator(bufferSize);
}
void resetAllocator()
{
	if (_allocator)
		_allocator->reset();
}
void destroyAllocator()
{
	if (_allocator)
		delete _allocator;
	_allocator = NULL;
}

size_t getAllocationCount()
{
	AllocatorStats stats = _allocator->stats();
	return stats.allocationCount;
}*/

template<typename T> T* allocate()
{
#ifdef USE_FRAME_ALLOCATOR
	void *mem = _allocator->allocate(sizeof(T));
	return new(mem)T();
#else
	return new T();
#endif
}

template<typename T> void deallocate(T* val)
{
#ifdef USE_FRAME_ALLOCATOR
	_allocator->free(val);
#else
	delete val;
#endif
}


GclString *gclString()
{
	return allocate<GclString>();
}

GclString *gclString(const char *str)
{
	GclString*s = allocate<GclString>();
	
	strncpy(s->value, str, sizeof(s->value));
	s->value[sizeof(s->value) - 1] = 0;
	//printf("alloc %p %s\n", s, s->value);
	return s;
}
void gclFreeString(GclString *val)
{
	//printf("free %p %s\n", val, val->value);
	deallocate<GclString>(val);
}


GclValue *gclValue()
{
	GclValue *v = allocate<GclValue>();
	v->type = kValue_INT;
	v->iValue = 0;
	return v;
}
GclValue *gclValue(bool val)
{
	GclValue *v = allocate<GclValue>();
	v->type = kValue_BOOL;
	v->bValue = val;
	return v;
}
GclValue *gclValue(int64_t val)
{
	GclValue *v = allocate<GclValue>();
	v->type = kValue_INT;
	v->iValue = val;
	return v;
}
GclValue *gclValue(float val)
{
	GclValue *v = allocate<GclValue>();
	v->type = kValue_FLOAT;
	v->fValue = val;
	return v;
}
GclValue *gclValue(float val, float val2)
{
	GclValue *v = allocate<GclValue>();
	v->type = kValue_FLOAT2;
	v->f2Value[0] = val;
	v->f2Value[1] = val2;
	return v;
}
GclValue *gclValue(float val, float val2, float val3)
{
	GclValue *v = allocate<GclValue>();
	v->type = kValue_FLOAT3;
	v->f3Value[0] = val;
	v->f3Value[1] = val2;
	v->f3Value[2] = val3;
	return v;
}
GclValue *gclValue(float val, float val2, float val3, float val4)
{
	GclValue *v = allocate<GclValue>();
	v->type = kValue_FLOAT4;
	v->f4Value[0] = val;
	v->f4Value[1] = val2;
	v->f4Value[2] = val3;
	v->f4Value[3] = val4;
	return v;
}
GclValue *gclValue(GclValueType type)
{
	GclValue *v = allocate<GclValue>();
	v->type = type;
	return v;
}
GclValue *gclValue(GclValueType type, const char *val)
{
	GclValue *v = allocate<GclValue>();
	v->type = type;
	strncpy(v->strValue, val, sizeof(v->strValue));
	v->strValue[sizeof(v->strValue) - 1] = 0;
	return v;
}
void gclFreeValue(GclValue *val)
{
	deallocate<GclValue>(val);
}

GclMember *gclMember()
{
	return allocate<GclMember>();
}
void gclFreeMember(GclMember *val)
{
	if (val->name) gclFreeString(val->name);
	if (val->value) gclFreeValue(val->value);
	if (val->object) gclFreeObject(val->object);
	deallocate<GclMember>(val);
}

GclItemList *gclItemList()
{
	return allocate<GclItemList>();
}
void gclFreeItemList(GclItemList *val)
{
	GclItemList *next = val->next;
	if (val->member) gclFreeMember(val->member);
	if (val->object) gclFreeObject(val->object);
	deallocate<GclItemList>(val);
	if (next)
		gclFreeItemList(next);
}

GclObject *gclObject()
{
	return allocate<GclObject>();
}
void gclFreeObject(GclObject *val)
{
	if (val->name) gclFreeString(val->name);
	if (val->type) gclFreeString(val->type);
	if (val->itemList) gclFreeItemList(val->itemList);
	deallocate<GclObject>(val);
}

GclObjectList *gclObjectList()
{
	return allocate<GclObjectList>();
}
void gclFreeObjectList(GclObjectList *val)
{
	GclObjectList *next = val->next;
	if (val->object) gclFreeObject(val->object);
	deallocate<GclObjectList>(val);
	if (next)
		gclFreeObjectList(next);
}

