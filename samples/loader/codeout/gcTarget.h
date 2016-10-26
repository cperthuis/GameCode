#ifndef GCTARGET_H
#define GCTARGET_H

#include "eCenter.h"
#include "eFaction.h"
#include "eTargetType.h"
#include "eSortFunction.h"
#include "eSortOrder.h"

#include "gcStep.h"

struct gcTarget;

struct gcTarget : public gcStep
{ 
	virtual ~gcTarget(){}

	virtual unsigned int getType() { return GC_TARGET; }

	eCenter::value center;
	gcTarget *target;
	float radius;
	eFaction::value faction;
	eTargetType::value type;
	unsigned int count;
	eSortFunction::value sortFunction;
	eSortOrder::value sortOrder;
};

#endif // TARGET_H
