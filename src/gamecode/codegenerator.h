#ifndef GENERATOR_H
#define GENERATOR_H

struct GclObjectList;
struct Meta;

int generateOutput(GclObjectList *objectList, const Meta *meta, const char *filename, const char *outputPath, bool printResult);

#endif
