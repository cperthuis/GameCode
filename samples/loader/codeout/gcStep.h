#pragma once

#define GC_STEP 0x7C8C8DE1
#define GC_TARGET 0xD2EBE94C
#define GC_EFFECT 0xB0426512
#define GC_MOVE 0x7C89307C
#define GC_PLAYSEQUENCE 0x86C633B4
#define GC_DEALDAMAGE 0xE1CFCF5A

struct gcStep
{ 
	virtual ~gcStep(){}

	virtual unsigned int getType() { return GC_STEP; }

};

