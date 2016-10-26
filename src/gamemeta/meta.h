#ifndef META_H
#define META_H

#include "metacommon.h"
#include <set>
#include <vector>

struct GmlItemList;

struct GenEmumMember
{
	const char *name;
	int value;
};

struct GenEnum
{
	const char *name;
	std::vector<GenEmumMember> members;
};

struct GenObject;
struct GenMember
{
	const char *name;
	GenObject *objectType;
	GenEnum *enumType;
	GmlMemberTypeType type;
	unsigned int modifiers;
	GmlValue *defaultValue;
	void *defaultObject;
	size_t offset;
	size_t size;
};

struct GenVTablePtr
{
	GenVTablePtr();
	GenVTablePtr(size_t offset, size_t objectTypeIndex) : offset(offset), objectTypeIndex(objectTypeIndex){}
	size_t offset;
	size_t objectTypeIndex;
};

struct GenObject
{
	const char *name;
	GenObject *parent;
	std::vector<GenMember> members;
	std::vector<size_t> vTablePtrOffsets;
	std::vector<size_t> strPtrOffsets;
	std::vector<GenObject*> derivedObjects; //all the objects derived from this type, only filled for the top most parent
	size_t size; //this is the full size of the object as given by a C++ sizeof(), it's padded to the multiple of the required alignment.
	size_t nonAlignedSize; //this is the size of the object when it's embedded in a derived class
	void *defaultObject; //memory block with the layout of the C object initialized with the default values
	size_t alignmentRequirement;
	size_t typeIndex; //index in the list of GenObject
	bool hasVTablePtr;
	unsigned int hash;
};

struct lessGenEnum {
	bool operator()(GenEnum* const &lhs, GenEnum* const &rhs) const {
		return strcmp(lhs->name, rhs->name) < 0;
	}
};

struct lessGenObject {
	bool operator()(GenObject* const &lhs, GenObject* const &rhs) const {
		return strcmp(lhs->name, rhs->name) < 0;
	}
};

struct Meta
{
	std::vector<GenEnum> enums;
	std::vector<GenObject*> objects;
	std::set<GenEnum*, lessGenEnum> enumSet;
	std::set<GenObject*, lessGenObject> objectSet;
};

int compileMeta(Meta *meta, bool generateMetaCode, const char *metaFilepath = NULL, const char *metaCodeOutputPath = NULL);
int createMeta(Meta *meta, const GmlItemList *itemList);

GenObject* getMetaObject(const Meta *meta, const char *name);
inline bool isMetaObjectType(const Meta *meta, const char *name) { return (getMetaObject(meta, name) != NULL); }

size_t getMemberSize(GmlMemberTypeType type, GenObject *objectType, bool isArray, bool isReference);

#endif
