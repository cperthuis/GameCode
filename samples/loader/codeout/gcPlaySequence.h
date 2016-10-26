#ifndef GCPLAYSEQUENCE_H
#define GCPLAYSEQUENCE_H

#include "eCenter.h"

#include "gcStep.h"

struct gcTarget;

struct gcPlaySequence : public gcStep
{ 
	virtual ~gcPlaySequence(){}

	virtual unsigned int getType() { return GC_PLAYSEQUENCE; }

	const char *file;
	gcTarget *target;
	float duration;
	eCenter::value center;
};

#endif // PLAYSEQUENCE_H
