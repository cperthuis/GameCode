// loader
// This sample loads a compiled gamedata file and print some info 
// about its content thanks to the gamecode meta linked in into the executable.

// The meta file gametypes.txt is compiled via a custom rule.
// The gamecode compiler generates c++ headers corresponding to the objects defined in the meta.
// The output can be found in the codeout folder

// The gamecode data file thunderAttack.txt is also compiled via a custom rule.
// The gamecode compiler compiles the data into a binary file.
// The output thunderAttack.gcb is in the folder dataout.

//#include gamecode basic types
#include <gc.h>

//#include the generated code
#include <gcAll.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <Shlwapi.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <gcVptr.cpp>

char gExePath[260];

static char *loadBuffer(const char *filename)
{
	strncpy_s(gExePath, __argv[0], sizeof(gExePath));
	GetFullPathNameA(__argv[0], sizeof(gExePath), gExePath, NULL);

	gExePath[sizeof(gExePath) - 1] = 0;
	char *p = strrchr(gExePath, '\\');
	if (p && *p)
	{
		*p = 0;
		SetCurrentDirectory(gExePath);
	}

	char buffer[1024];
	char fullpath[1024];

	sprintf(buffer, "%s\\..\\dataout\\%s", gExePath, filename);

	PathCanonicalizeA(fullpath, buffer);
	FILE* fp = fopen(fullpath, "rb");
	if (!fp)
	{
		printf("Error: couldn't open %s\n", fullpath);
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *bin = new char[size];
	fread(bin, size, 1, fp);
	fclose(fp);
	
	return bin;
}

static void releaseBuffer(char *buffer)
{
	if (buffer)
	{
		delete[] buffer;
		*buffer = 0;
	}
}

void dumpGamedata(const char *filename, const void * const * vPtrTable)
{
	//load the compiled data in memory
	char *bin = loadBuffer(filename);
	if (!bin)
		return;

	//we're loading the data in place, so patch the pointers
	GcBlockHeader *header = (GcBlockHeader *)bin;
	gcPatchBlockPointers(header, vPtrTable);

	//check that we have 
	if (!header->objectCount)
	{
		printf("The data file %s doesn't contain any object.\n", filename);
		return;
	}

	const GcObjectHeader *objects = (const GcObjectHeader *)(bin + header->objectTableOffset);

	size_t objectCount = header->objectCount;
	for (size_t i = 0; i < objectCount; i++)
	{
		GcObjectHeader obj = objects[i];
		if (obj.typeID == GC_ACTION)
		{
			gcAction* action = (gcAction*)(bin + obj.offset);

			size_t stepCount = action->steps.size();
			printf("\naction %s: mana cost: %i, %llu steps\n", obj.name, action->cost.mana, stepCount);
			for (size_t j = 0; j < stepCount; j++)
			{
				gcStep *step = action->steps[j];
				uint32_t type = step->getType();
				switch (type)
				{
				case GC_STEP:
					printf("%llu: step\n", j);
					break;
				case GC_TARGET:
					printf("%llu: target\n", j);
					break;
				case GC_EFFECT:
					printf("%llu: effect\n", j);
					break;
				case GC_MOVE:
					printf("%llu: move\n", j);
					break;
				case GC_PLAYSEQUENCE:
					{
						gcPlaySequence* playSequence = (gcPlaySequence*)step;
						printf("%llu: playsequence %s\n", j, playSequence->file);
						break;
					}
				case GC_DEALDAMAGE:
					{
						gcDealDamage *dealDamage = (gcDealDamage *)step;
						printf("%llu: dealdamage %i\n", j, dealDamage->amount);
						break;
					}
				}
			}
		}
	}

	releaseBuffer(bin);
}

int main()
{
	//the code generated from the meta was included via #include <gcAll.h>

	//verify that the meta has some objects defined
	int gcObjectCount = gcGetObjectCount();
	printf("The meta has %i objects registered.\n", gcObjectCount);

	//initialize the vptr table from the meta
	const void * const * vPtrTable = gcInitializeVptrTable();

	dumpGamedata("thunderAttack.gcb", vPtrTable);

	return 0;
}