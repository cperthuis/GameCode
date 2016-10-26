#ifndef GCDEALDAMAGE_H
#define GCDEALDAMAGE_H

#include "gcStep.h"

struct gcTarget;

struct gcDealDamage : public gcStep
{ 
	virtual ~gcDealDamage(){}

	virtual unsigned int getType() { return GC_DEALDAMAGE; }

	gcTarget *target;
	int amount;
};

#endif // DEALDAMAGE_H
