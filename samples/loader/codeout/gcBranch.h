#ifndef GCBRANCH_H
#define GCBRANCH_H

#define GC_BRANCH 0xAA197993

#include <gc.h>

struct gcAction;

struct gcBranch
{ 
	gcAction *action;
	GcExpression condition;
	unsigned int weight;
};

#endif // BRANCH_H
