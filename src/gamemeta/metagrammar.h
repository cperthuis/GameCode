#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "metacommon.h"

/*struct Node
{
	char *tmp;
};*/

struct GmlString
{
	char value[256];
};
GmlString *gmlString();
GmlString *gmlString(const char *str);
void gmlFreeString(GmlString *val);

struct GmlEnumMember
{
	GmlString *name;
	int value;
};
GmlEnumMember *gmlEnumMember();
void gmlFreeEnumMember(GmlEnumMember *val);

struct GmlEnumMemberList
{
	GmlEnumMember *member;
	GmlEnumMemberList *parent;
	GmlEnumMemberList *next;
};
GmlEnumMemberList *gmlEnumMemberList();
void gmlFreeEnumMemberList(GmlEnumMemberList *val);

struct GmlEnum
{
	GmlString *name;
	GmlEnumMemberList *memberList;
};
GmlEnum *gmlEnum();
void gmlFreeEnum(GmlEnum* val);

enum GmlTypeModifierType
{
	kTypeModifier_NONZERO,
	kTypeModifier_REF,
	kTypeModifier_ARRAY,
	kTypeModifierCount
};

struct GmlTypeModifier
{
	GmlTypeModifierType type;
};
GmlTypeModifier *gmlTypeModifer();
void gmlFreeTypeModifier(GmlTypeModifier *val);

struct GmlTypeModifierList
{
	GmlTypeModifier *modifier;
	GmlTypeModifierList *parent;
	GmlTypeModifierList *next;
};
GmlTypeModifierList *gmlTypeModifierList();
void gmlFreeTypeModifierList(GmlTypeModifierList *val);

struct GmlMemberType
{
	GmlMemberTypeType type;
	GmlString *objectName;
};
GmlMemberType *gmlMemberType();
void gmlFreeMemberType(GmlMemberType *val);

struct GmlType
{
	GmlTypeModifierList *modifierList;
	GmlMemberType *memberType;
};
GmlType *gmlType();
void gmlFreeType(GmlType * val);


GmlValue *gmlValue();
GmlValue *gmlValue(bool val);
GmlValue *gmlValue(int val);
GmlValue *gmlValue(float val);
GmlValue *gmlValue(float val, float val2);
GmlValue *gmlValue(float val, float val2, float val3);
GmlValue *gmlValue(float val, float val2, float val3, float val4);
GmlValue *gmlValue(GclValueType type);
GmlValue *gmlValue(GclValueType type, const char *val);
void gmlFreeValue(GmlValue* val);

struct GmlObjectMember
{
	GmlString *name;
	GmlType *type;
	GmlValue *defaultValue;
	bool isArray;
};
GmlObjectMember *gmlObjectMember();
void gmlFreeObjectMember(GmlObjectMember *val);

struct GmlObjectMemberList
{
	GmlObjectMember *member;
	GmlObjectMemberList *parent;
	GmlObjectMemberList *next;
};
GmlObjectMemberList *gmlObjectMemberList();
void gmlFreeObjectMemberList(GmlObjectMemberList *val);

struct GmlObject
{
	GmlString *name;
	GmlString *parentName;
	GmlObjectMemberList *memberList;
};
GmlObject *gmlObject();
void gmlFreeObject(GmlObject* val);

struct GmlItemList
{
	GmlObject *objectItem;
	GmlEnum * enumItem;
	GmlItemList *parent;
	GmlItemList *next;
};
GmlItemList *gmlItemList();
void gmlFreeItemList(GmlItemList* val);


struct GmlParserToken {
	bool bValue;
	int iValue;
	float fValue;
	char strValue[256];
};

struct GmlParserExtra {
	int line;
	int col;
	bool valid;
	GmlItemList *root;
};

void initializeAllocator(size_t bufferSize);
void resetAllocator();
void destroyAllocator();
size_t getAllocationCount();

#endif //#ifndef GRAMMAR_H
