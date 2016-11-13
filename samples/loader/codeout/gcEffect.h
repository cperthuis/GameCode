#pragma once

#include "gcStep.h"

struct gcEffect : public gcStep
{ 
	virtual ~gcEffect(){}

	virtual unsigned int getType() { return GC_EFFECT; }

	float start;
	unsigned int reference;
	float duration;
};

