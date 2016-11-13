#pragma once

#include "gcEffect.h"

struct gcTarget;

struct gcMove : public gcEffect
{ 
	virtual ~gcMove(){}

	virtual unsigned int getType() { return GC_MOVE; }

	gcTarget *target;
};

