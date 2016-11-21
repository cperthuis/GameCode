#include "meta.h"
#include <gc.h>

#include "metagrammar.h"
#include <map>

uint32_t hash32(const char *str)
{
	uint32_t hash = 5381;
	int c;

	const unsigned char *str0 = (const unsigned char *)str;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}


static unsigned int getTypeModifierMask(GmlTypeModifierType type)
{
	switch (type)
	{
	case kTypeModifier_NONZERO:
		return GML_TYPE_MODIFIER_NONZERO;
		break;
	case kTypeModifier_REF:
		return GML_TYPE_MODIFIER_REF;
		break;
	default:
		return 0;
	}
}

size_t _memberTypeSizes[kMemberTypeCount] =
{
	1, //kMemberType_BOOL,
	1, //kMemberType_UINT8,
	1, //kMemberType_INT8,
	2, //kMemberType_UINT16,
	2, //kMemberType_INT16,
	4, //kMemberType_UINT,
	4, //kMemberType_INT,
	8, //kMemberType_UINT64,
	8, //kMemberType_INT64,
	4, //kMemberType_UFLOAT,
	4, //kMemberType_FLOAT,
	8, //kMemberType_FLOAT2,
	12, //kMemberType_FLOAT3,
	16, //kMemberType_FLOAT4,
	sizeof(GcExpression), //kMemberType_EXPRESSION,
	8, //sizeof(GmlString), //kMemberType_STRING,
	0, //kMemberType_OBJECT,
	4, //kMemberType_ENUM,
};

static const char *_typeStrings[kMemberTypeCount] =
{
	"bool",
	"uint8",
	"int8",
	"uint16",
	"int16",
	"uint",
	"int",
	"uint64",
	"int64",
	"ufloat",
	"float",
	"float2",
	"float3",
	"float4",
	"expression",
	"string",
	"object",
	"enum"
};

static const char *_valueTypeStrings[kValueTypeCount] =
{
	"bool",
	"int",
	"float",
	"float2",
	"float3",
	"float4",
	"+inf",
	"-inf",
	"enum/variable",
	"string"
};

size_t _menberArraySize = sizeof(GcArray<int>);
size_t _menberReferenceSize = 8;

size_t getMemberSize(GmlMemberTypeType type, GenObject *objectType, bool isArray, bool isReference)
{
	size_t objectSize;
	if (isArray)
		objectSize = _menberArraySize;
	else if (isReference)
		objectSize = _menberReferenceSize;
	else if (type == kMemberType_OBJECT)
		objectSize = objectType->size;
	else
		objectSize = _memberTypeSizes[type];
	return objectSize;
}

struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};

static inline size_t maxValue(size_t a, size_t b)
{
	return a > b ? a : b;
}
static inline size_t alignUpper(size_t size, size_t powerOf2Alignment)
{
	size_t alignedSize = ((size + powerOf2Alignment - 1)&~(powerOf2Alignment - 1));
	return alignedSize;
}

int createMeta(Meta *meta, const GmlItemList *itemList)
{
	int enumCount = 0;
	int objectCount = 0;
	const GmlItemList *l = itemList;
	std::set<const char *, cmp_str> parentNames;
	std::map<const char *, int, cmp_str> objectNames;
	while (l)
	{
		if (l->enumItem)
			enumCount++;
		if (l->objectItem)
		{
			objectCount++;
			if (l->objectItem->parentName)
			{
				parentNames.insert(l->objectItem->parentName->value);
			}
			objectNames[l->objectItem->name->value] = 0;
		}
		l = l->next;
	}
	
	int index = 0;
	std::map<const char *, int, cmp_str>::iterator it = objectNames.begin();
	for (; it != objectNames.end(); it++)
	{
		it->second = index;
		index++;
	}

	meta->enums.reserve(enumCount);
	meta->objects.reserve(objectCount);

	l = itemList;
	while (l)
	{
		if (l->enumItem)
		{
			GmlEnum *e = l->enumItem;
			GenEnum ge;
			ge.name = e->name->value;
			GmlEnumMemberList *ml = e->memberList;
			while (ml)
			{
				GenEmumMember gem;
				gem.name = ml->member->name->value;
				int value = ml->member->value;
				if (value < 0)
				{
					size_t size = ge.members.size();
					if (size == 0)
						value = 0;
					else
						value = ge.members[size - 1].value + 1;
				}
				gem.value = value;
				ge.members.push_back(gem);
				ml = ml->next;
			}
			meta->enums.push_back(ge);
			GenEnum *pge = &meta->enums[meta->enums.size() - 1];
			meta->enumSet.insert(pge);
		}

		if (l->objectItem)
		{
			GmlObject *o = l->objectItem;
			GenObject *go = new GenObject;
			go->alignmentRequirement = 1;
			go->name = o->name->value;
			go->hash = hash32(go->name);
			go->parent = NULL;
			go->size = 0;
			go->typeIndex = objectNames[o->name->value];
			GmlString *parentName = o->parentName;
			if (parentName)
			{
				GenObject tmp;
				tmp.name = parentName->value;
				std::set<GenObject*, lessGenObject>::iterator it = meta->objectSet.find(&tmp);
				if (it != meta->objectSet.end())
				{
					go->parent = (*it);
				}
				else
				{
					printf("Object %s: Unknown parent type %s\n", go->name, parentName->value);
					delete go;
					return -1;
				}
				
				//add this child object to its top most parent child list (will be used to generate #define GC_CHILDOBJECT in the top most parent header)
				GenObject *ancestor = go->parent;
				while (ancestor->parent)
					ancestor = ancestor->parent;
				ancestor->derivedObjects.push_back(go);

				go->hasVTablePtr = true;
			}
			else
			{
				go->hasVTablePtr = parentNames.find(go->name) != parentNames.end();
			}
			
			//Visual C++ rule, the first element ( which comes second after the vtable, needs to also be aligned on the max required alignment )
			bool alignNextMemberToMaxAlignment = false;

			//reserve 8 bytes for the vTablePtr at the top
			if (go->hasVTablePtr)
			{
				go->size = go->parent ? go->parent->size : 8;
				go->alignmentRequirement = maxValue(go->alignmentRequirement, go->parent ? go->parent->alignmentRequirement : 8);
				go->vTablePtrOffsets.push_back(0);

				//if the size is 8, it means that we only have empty base classes with no parameters, so we'll have to align the respect the VS C++ rule
				if (go->size == 8)
					alignNextMemberToMaxAlignment = true;
				//technically we would need to compute the max alignment here, for now because we don't have anything > 8, 
				//we can assume that we already have it in go->alignmentRequirement
			}

			GmlObjectMemberList *ml = o->memberList;
			while (ml)
			{
				if (ml->member)
				{
					GenMember gom;
					gom.name = ml->member->name->value;
					gom.objectType = NULL;
					gom.enumType = NULL;
					gom.type = ml->member->type->memberType->type;
					gom.modifiers = 0;
					bool isArray = ml->member->isArray;
					if (isArray)
						gom.modifiers |= GML_TYPE_MODIFIER_ARRAY;
					GmlTypeModifierList *modl = ml->member->type->modifierList;
					while (modl)
					{
						if (modl->modifier)
						{
							unsigned int mask = getTypeModifierMask(modl->modifier->type);
							gom.modifiers |= mask;
						}
						modl = modl->next;
					}
					bool isReference = (gom.modifiers & GML_TYPE_MODIFIER_REF)!=0;
					gom.defaultValue = ml->member->defaultValue;
					gom.defaultObject = NULL;
					if (gom.type == kMemberType_OBJECT)
					{
						const GmlString *typeClassName = ml->member->type->memberType->objectName;

						if (!typeClassName)
						{
							printf("Error: Object %s:  Member %s: type is Object but object name is not set\n", go->name, gom.name);
							return -1;
						}
						
						bool found = false;
						
						//is the member of the same type as the object, if yes, it needs to be a reference.
						if (strcmp(go->name, typeClassName->value) == 0)
						{
							if (isReference)
							{
								gom.objectType = go;
								found = true;
							}
							else
							{
								printf("Error: Object %s:  Member %s is of the same type than the object and is not a reference, it needs to be a reference.\n", go->name, gom.name);
								delete go;
								return -1;
							}
						}
						
						//search in enums
						if (!found)
						{
							GenEnum tmp;
							tmp.name = typeClassName->value;
							std::set<GenEnum*, lessGenEnum>::iterator it = meta->enumSet.find(&tmp);
							if (it != meta->enumSet.end())
							{
								gom.type = kMemberType_ENUM;
								gom.enumType = (*it);
								found = true;
							}
						}
						//search in objects
						if (!found)
						{
							GenObject tmp;
							tmp.name = typeClassName->value;
							std::set<GenObject*, lessGenObject>::iterator it = meta->objectSet.find(&tmp);
							if (it != meta->objectSet.end())
							{
								gom.objectType = (*it);

								if (gom.modifiers & GML_TYPE_MODIFIER_ARRAY)
								{
									//do not save any default for arrays, gom.objectType->defaultObject would be the default of the type contained in the array, it's not useful to keep it around
									//it would be useful to support pre-filled arrays, but we don't have a mechanism to overwrite an array, just to add an element to it.
								}
								else
								{
									if (gom.defaultObject == NULL)
										gom.defaultObject = gom.objectType->defaultObject; 
								}
							}
							else
							{
								printf("Error: Object %s: Member %s: Unknown type %s\n", go->name, gom.name, typeClassName->value);
								delete go;
								return -1;
							}
						}
					}
					size_t objectSize = getMemberSize(gom.type, gom.objectType, isArray, isReference);
					
					//align member and update object alignment
					size_t memberAlignment = _memberTypeSizes[gom.type];
					if (gom.type == kMemberType_FLOAT2 || gom.type == kMemberType_FLOAT3 || gom.type == kMemberType_FLOAT4)
						memberAlignment = 4;
					if (gom.type == kMemberType_EXPRESSION)
						memberAlignment = 1;

					if (gom.modifiers & GML_TYPE_MODIFIER_ARRAY || gom.modifiers & GML_TYPE_MODIFIER_REF || 
						gom.type == kMemberType_STRING)
					{
						memberAlignment = 8;
					}
					else if (gom.type == kMemberType_OBJECT)
					{
						memberAlignment = gom.objectType->alignmentRequirement;
					}

					//VISUAL C++ rule: first member is aligned on the max member alignment of the class
					if (alignNextMemberToMaxAlignment)
					{
						memberAlignment = maxValue(memberAlignment, go->alignmentRequirement);
						alignNextMemberToMaxAlignment = false;
					}
					
					go->size = alignUpper(go->size, memberAlignment);
					go->alignmentRequirement = maxValue(go->alignmentRequirement, memberAlignment);
					size_t memberOffset = go->size;
					gom.offset = memberOffset;
					//printf("%s: %llu\n", gom.name, gom.offset);

					//record vtable pointers and string pointers
					if ((gom.type == kMemberType_OBJECT) && (!((gom.modifiers & GML_TYPE_MODIFIER_ARRAY) || (gom.modifiers & GML_TYPE_MODIFIER_REF))))
					{
						//if this member is an object, we add its vtable and string pointers to the current object
						const std::vector<size_t> &memberVPtrs = gom.objectType->vTablePtrOffsets;
						size_t count = memberVPtrs.size();
						for (size_t i = 0; i < count; i++)
						{
							go->vTablePtrOffsets.push_back(memberOffset + memberVPtrs[i]);
						}
						const std::vector<size_t> &memberStrings = gom.objectType->strPtrOffsets;
						count = memberStrings.size();
						for (size_t i = 0; i < count; i++)
						{
							go->strPtrOffsets.push_back(memberOffset + memberStrings[i]);
						}
					}
					else if ( (gom.type == kMemberType_STRING) && (!(gom.modifiers & GML_TYPE_MODIFIER_ARRAY)))
					{
						//if it's a string, we add the string offset
						go->strPtrOffsets.push_back(memberOffset);
					}

					gom.size = objectSize; //not strictly necessary, but could speed up generation
					go->size += objectSize;
					go->members.push_back(gom);
				}
				ml = ml->next;
			}
			go->defaultObject = NULL;
			if (o->memberList)
			{
				char *buffer = new char[go->size];
				if (go->hasVTablePtr)
				{
					//Add the virtual table
					memcpy(buffer, &go->typeIndex, 8);

					//Add the parent defaults
					if (go->parent && go->parent->size > 8)
					{
						memcpy(buffer + 8, (char*)go->parent->defaultObject + 8, go->parent->nonAlignedSize - 8);
						size_t padding = go->parent->size - go->parent->nonAlignedSize;
						if (padding)
						{
							memset(buffer + go->parent->nonAlignedSize, 0, padding);
						}
					}

				}
				for (int i = 0; i < (int)go->members.size(); i++)
				{
					const GenMember &member = go->members[i];
					if (member.defaultValue)
					{
						if (member.type == kMemberType_ENUM)
						{
							int value = member.enumType->members[0].value;
							if (member.defaultValue->type == kValue_STRING)
							{
								for (size_t j = 0; j < member.enumType->members.size(); j++)
								{
									if (strcmp(member.enumType->members[j].name, member.defaultValue->strValue) == 0)
									{
										value = member.enumType->members[j].value;
										break;
									}
								}
							}
							else
							{
								value = (int)(member.defaultValue->iValue);
							}
							memcpy(buffer + member.offset, &value, member.size);
						}
						else
						{
													
							if (member.type >= kMemberType_FLOAT && member.type <= kMemberType_FLOAT4)
							{
								bool defaultIsScalar = (member.defaultValue->type == kValue_INT || member.defaultValue->type == kValue_FLOAT);
								if (defaultIsScalar)
								{
									float defaultValue = member.defaultValue->fValue;
									if (member.defaultValue->type == kValue_INT)
										defaultValue = (float)member.defaultValue->iValue;
									float values[4];
									values[0] = defaultValue;
									values[1] = defaultValue;
									values[2] = defaultValue;
									values[3] = defaultValue;
									memcpy(buffer + member.offset, &values, member.size);
								}
								else
								{
									if ((member.type == kMemberType_FLOAT4 && member.defaultValue->type != kValue_FLOAT4) ||
										(member.type == kMemberType_FLOAT3 && member.defaultValue->type != kValue_FLOAT3) ||
										(member.type == kMemberType_FLOAT2 && member.defaultValue->type != kValue_FLOAT2))
									{
										printf("Error: Object %s: Member %s: default value of type %s is incompatible with member type %s\n", go->name, member.name, _valueTypeStrings[member.defaultValue->type], _typeStrings[member.type]);
										delete go;
										return -1;
									}
								}
							}
							else
							{
								//NOTE: we are assuming little endian here
								//for integers our internal storage is int64 and we're potentially mem copying part of that int64 into
								//a smaller type int, int16 or even int8. This has for effect to discard the high order bits since we're in 
								//little endian, and this is what a cast would do, so it works for us.
								memcpy(buffer + member.offset, &member.defaultValue->strValue, member.size);
							}
						}
					}
					else if (member.defaultObject)
					{
						memcpy(buffer + member.offset, member.defaultObject, member.size);
					}
					else
						memset(buffer + member.offset, 0, member.size);
				}
				go->defaultObject = buffer;
				go->nonAlignedSize = go->size;
				go->size = alignUpper(go->size, go->alignmentRequirement);
				//printf("%s: %i\n", go->name, go->size);
			}

			//first check if the object was forward-declared
			bool found = false;
			GenObject *pgo = NULL;
			std::set<GenObject*, lessGenObject>::iterator it = meta->objectSet.find(go);
			if (it != meta->objectSet.end())
			{
				found = true;
				if ((*it)->members.size() == 0 && go->members.size() > 0)
				{
					*(*it) = *go;
					delete go;
				}
			}

			//no, it wasn't add it
			if (!found)
			{
				meta->objects.push_back(go);
				GenObject *pgo = meta->objects[meta->objects.size() - 1];
				meta->objectSet.insert(pgo);
			}
		}
		l = l->next;
	}

	return 0;
}

GenObject* getMetaObject(const Meta *meta, const char *name)
{
	GenObject dummy;
	dummy.name = name;
	std::set<GenObject*, lessGenObject>::iterator it = meta->objectSet.find(&dummy);
	if (it != meta->objectSet.end())
		return *it;
	else
		return NULL;
}
