#ifndef GCACTION_H
#define GCACTION_H

#define GC_ACTION 0xA6BF38E3

#include <gc.h>

#include "eBranchOrder.h"

#include "gcCost.h"
#include "gcBranch.h"

struct gcStep;

struct gcAction
{ 
	gcCost cost;
	GcExpression condition;
	float cooldown;
	GcArray<gcStep *> conditionSteps;
	GcArray<gcStep *> steps;
	eBranchOrder::value branchOrder;
	GcArray<gcBranch> branches; 
};

#endif // ACTION_H
