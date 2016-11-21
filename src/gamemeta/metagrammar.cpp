#include "metagrammar.h"
#include <string.h>
#include <new>

#include <shared/allocator.h>
#include <stdio.h>

#define USE_FRAME_ALLOCATOR

FrameAllocator *_allocator = NULL;

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
}

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


GmlString *gmlString()
{
	return allocate<GmlString>();
}

GmlString *gmlString(const char *str)
{
	GmlString*s = allocate<GmlString>();
	
	strncpy(s->value, str, sizeof(s->value));
	s->value[sizeof(s->value) - 1] = 0;
	//printf("alloc %p %s\n", s, s->value);
	return s;
}
void gmlFreeString(GmlString *val)
{
	//printf("free %p %s\n", val, val->value);
	deallocate<GmlString>(val);
}

GmlEnumMember *gmlEnumMember()
{
	return allocate<GmlEnumMember>();
}
void gmlFreeEnumMember(GmlEnumMember *val)
{
	gmlFreeString(val->name);
	deallocate<GmlEnumMember>(val);
}

GmlEnumMemberList *gmlEnumMemberList()
{
	return allocate<GmlEnumMemberList>();
}
void gmlFreeEnumMemberList(GmlEnumMemberList *val)
{
	GmlEnumMemberList *next = val->next;
	gmlFreeEnumMember(val->member);
	deallocate<GmlEnumMemberList>(val);
	if (next)
		gmlFreeEnumMemberList(next);
}

GmlEnum *gmlEnum()
{
	return allocate<GmlEnum>();
}
void gmlFreeEnum(GmlEnum* val)
{
	GmlEnumMemberList *next = val->memberList;
	gmlFreeString(val->name);
	deallocate<GmlEnum>(val);
	gmlFreeEnumMemberList(next);
}

GmlObjectMember *gmlObjectMember()
{
	return allocate<GmlObjectMember>();
}
void gmlFreeObjectMember(GmlObjectMember *val)
{
	gmlFreeString(val->name);
	gmlFreeType(val->type);
	if (val->defaultValue) gmlFreeValue(val->defaultValue);
	deallocate<GmlObjectMember>(val);
}

GmlObjectMemberList *gmlObjectMemberList()
{
	return allocate<GmlObjectMemberList>();
}
void gmlFreeObjectMemberList(GmlObjectMemberList *val)
{
	GmlObjectMemberList *next = val->next;
	if (val->member) gmlFreeObjectMember(val->member);
	deallocate<GmlObjectMemberList>(val);
	if (next)
		gmlFreeObjectMemberList(next);
}

GmlObject *gmlObject()
{
	return allocate<GmlObject>();
}
void gmlFreeObject(GmlObject* val)
{
	GmlObjectMemberList *next = val->memberList;
	gmlFreeString(val->name);
	if (val->parentName) gmlFreeString(val->parentName);
	deallocate<GmlObject>(val);
	if (next) gmlFreeObjectMemberList(next);
}

GmlValue *gmlValue()
{
	GmlValue *v = allocate<GmlValue>();
	v->type = kValue_INT;
	v->iValue = 0;
	return v;
}
GmlValue *gmlValue(bool val)
{
	GmlValue *v = allocate<GmlValue>();
	v->type = kValue_BOOL;
	v->bValue = val;
	return v;
}
GmlValue *gmlValue(int64_t val)
{
	GmlValue *v = allocate<GmlValue>();
	v->type = kValue_INT;
	v->iValue = val;
	return v;
}
GmlValue *gmlValue(float val)
{
	GmlValue *v = allocate<GmlValue>();
	v->type = kValue_FLOAT;
	v->fValue = val;
	return v;
}
GmlValue *gmlValue(float val, float val2)
{
	GmlValue *v = allocate<GmlValue>();
	v->type = kValue_FLOAT2;
	v->f2Value[0] = val;
	v->f2Value[1] = val2;
	return v;
}
GmlValue *gmlValue(float val, float val2, float val3)
{
	GmlValue *v = allocate<GmlValue>();
	v->type = kValue_FLOAT3;
	v->f3Value[0] = val;
	v->f3Value[1] = val2;
	v->f3Value[2] = val3;
	return v;
}
GmlValue *gmlValue(float val, float val2, float val3, float val4)
{
	GmlValue *v = allocate<GmlValue>();
	v->type = kValue_FLOAT4;
	v->f4Value[0] = val;
	v->f4Value[1] = val2;
	v->f4Value[2] = val3;
	v->f4Value[3] = val4;
	return v;
}
GmlValue *gmlValue(GclValueType type)
{
	GmlValue *v = allocate<GmlValue>();
	v->type = type;
	return v;
}
GmlValue *gmlValue(GclValueType type, const char *val)
{
	GmlValue *v = allocate<GmlValue>();
	v->type = type;
	strncpy(v->strValue, val, sizeof(v->strValue));
	v->strValue[sizeof(v->strValue) - 1] = 0;
	return v;
}
void gmlFreeValue(GmlValue *val)
{
	deallocate<GmlValue>(val);
}

GmlTypeModifier *gmlTypeModifer()
{
	return allocate<GmlTypeModifier>();
}
void gmlFreeTypeModifier(GmlTypeModifier *val)
{
	deallocate<GmlTypeModifier>(val);
}

GmlTypeModifierList *gmlTypeModifierList()
{
	GmlTypeModifierList *l = allocate<GmlTypeModifierList>();
	l->modifier = NULL;
	return l;
}
void gmlFreeTypeModifierList(GmlTypeModifierList *val)
{
	GmlTypeModifierList *next = val->next;
	if (val->modifier) gmlFreeTypeModifier(val->modifier);
	deallocate<GmlTypeModifierList>(val);
	if (next)
		gmlFreeTypeModifierList(next);
}

GmlMemberType *gmlMemberType()
{
	GmlMemberType *m = allocate<GmlMemberType>();
	m->objectName = NULL;
	m->type = kMemberType_INT;
	return m;
}
void gmlFreeMemberType(GmlMemberType *val)
{
	if (val->objectName) gmlFreeString(val->objectName);
	deallocate<GmlMemberType>(val);
}

GmlType *gmlType()
{
	return allocate<GmlType>();
}
void gmlFreeType(GmlType * val)
{
	gmlFreeTypeModifierList(val->modifierList);
	gmlFreeMemberType(val->memberType);
	deallocate<GmlType>(val);
}

GmlItemList *gmlItemList()
{
	return allocate<GmlItemList>();
}
void gmlFreeItemList(GmlItemList* val)
{
	GmlItemList* l = val;
	while (l)
	{
		GmlItemList *next = l->next;
		if (l->objectItem) gmlFreeObject(l->objectItem);
		if (l->enumItem) gmlFreeEnum(l->enumItem);
		deallocate<GmlItemList>(l);
		l = next;
	}
}
