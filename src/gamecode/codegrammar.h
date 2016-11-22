#ifndef CODEGRAMMAR_H
#define CODEGRAMMAR_H

#include <gamemeta/metacommon.h>

struct GenObject;
struct GenMember;

struct GclValueList;

struct GclString
{
	char value[256];
};
GclString *gclString();
GclString *gclString(const char *str);
void gclFreeString(GclString *val);

struct GclValue
{
	GclValueType type;
	union
	{
		bool bValue;
		int64_t iValue;
		float fValue;
		float f2Value[2];
		float f3Value[3];
		float f4Value[4];
		char strValue[256];
		GclValueList *listValue;
	};
	size_t line;
	size_t col;
};
GclValue *gclValue();
GclValue *gclValue(int64_t val);
GclValue *gclValueHex(uint64_t val);
GclValue *gclValue(float val);
GclValue *gclValue(float val, float val2);
GclValue *gclValue(float val, float val2, float val3);
GclValue *gclValue(float val, float val2, float val3, float val4);
GclValue *gclValue(GclValueType type);
GclValue *gclValue(GclValueType type, const char *val);
GclValue *gclValue(GclValueList* val);

void gclFreeValue(GclValue* val);

struct GclObject;
struct GclMember
{
	GclString *name;
	GclValue *value;
	GclObject *object;
	const GenMember *memberType;
	size_t line;
	size_t col;
};
GclMember *gclMember();
void gclFreeMember(GclMember *val);

struct GclItemList
{
	GclObject *object;
	GclMember *member;
	GclItemList *parent;
	GclItemList *next;
};
GclItemList *gclItemList();
void gclFreeItemList(GclItemList *val);

struct GclObject
{
	GclString *name;
	GclString *type;
	const GenObject *typeObject;
	GclItemList *itemList;
	size_t line;
	size_t col;
	int compiledObjectIndex;
};
GclObject *gclObject();
void gclFreeObject(GclObject *val);

struct GclObjectList
{
	GclObject *object;
	GclObjectList *parent;
	GclObjectList *next;
};
GclObjectList *gclObjectList();
void gclFreeObjectList(GclObjectList *val);

struct GclValueList
{
	GclValue *value;
	GclValueList *parent;
	GclValueList *next;
};
GclValueList *gclValueList();
void gclFreeValueList(GclValueList *val);

struct GclParserToken {
	bool bValue;
	int64_t iValue;
	float fValue;
	char strValue[256];
	GenObject *metaObject;
};

struct GclParserExtra {
	int line;
	int col;
	const char *filename;
	size_t first_column;
	size_t last_column;
	size_t first_line;
	size_t last_line;
	bool valid;
	GclObjectList *root;
	//Meta *meta;
};

void initializeAllocator(size_t bufferSize);
void resetAllocator();
void destroyAllocator();
size_t getAllocationCount();

#endif //#ifndef CODEGRAMMAR_H
