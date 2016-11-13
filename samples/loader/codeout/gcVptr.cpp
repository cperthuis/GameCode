#include "gcAll.h"

#define GC_OBJECT_COUNT 9
static void* _gcVptr[GC_OBJECT_COUNT];

int gcGetObjectCount() { return GC_OBJECT_COUNT; }

const void* const* gcInitializeVptrTable()
{
	{ _gcVptr[0] = NULL; } // gcAction
	{ _gcVptr[1] = NULL; } // gcBranch
	{ _gcVptr[2] = NULL; } // gcCost
	{ gcDealDamage tmp; _gcVptr[3] = *(void**)&tmp; }
	{ gcEffect tmp; _gcVptr[4] = *(void**)&tmp; }
	{ gcMove tmp; _gcVptr[5] = *(void**)&tmp; }
	{ gcPlaySequence tmp; _gcVptr[6] = *(void**)&tmp; }
	{ gcStep tmp; _gcVptr[7] = *(void**)&tmp; }
	{ gcTarget tmp; _gcVptr[8] = *(void**)&tmp; }

	return _gcVptr;
}
