#include "gcAll.h"
#include <new>

#define GC_OBJECT_COUNT 9
static void* _gcVptr[GC_OBJECT_COUNT];

int gcGetObjectCount() { return GC_OBJECT_COUNT; }

const void* const* gcInitializeVptrTable()
{
	new (&_gcVptr[0]) gcAction();
	new (&_gcVptr[1]) gcBranch();
	new (&_gcVptr[2]) gcCost();
	new (&_gcVptr[3]) gcDealDamage();
	new (&_gcVptr[4]) gcEffect();
	new (&_gcVptr[5]) gcMove();
	new (&_gcVptr[6]) gcPlaySequence();
	new (&_gcVptr[7]) gcStep();
	new (&_gcVptr[8]) gcTarget();

	return _gcVptr;
}
