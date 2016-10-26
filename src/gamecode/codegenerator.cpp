#include "codegenerator.h"

#include "codegrammar.h"
#include <gamemeta/meta.h>
#include <stdio.h>
#include <stdarg.h>

#include <set>
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include <limits>

#include <gc.h>

#include <shared/allocator.h>

//#include <gcAll.h>

static const char *strFormat(char *buffer, size_t size, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	vsnprintf_s(buffer, size, _TRUNCATE, format, args);

	va_end(args);

	return buffer;
}

static const char *strToUpper(char *buffer, size_t size, const char *input)
{
	char *o = buffer;
	char *oEnd = buffer + size-1;
	const char *p = input;
	while (*p && o<oEnd)
	{
		char c = *p++;
		*o++ = (c >= 'a' && c <= 'z') ? c + 'A' - 'a' : c;
	}
	*o = 0;
	return buffer;
}

class GenericGcArray
{
public:
	size_t _size;
	uint64_t _elements;
};

struct DataObject
{
	const GclObject *object;
	GenericGcArray *gcArray;
	const GclMember *gcArrayInstanceMember;
	char *buffer;
	size_t size;
	//std::vector<Reference> references;
	std::vector<std::vector<uint64_t>> references; //dataObject[objectIndex].buffer + offset
};

struct Reference
{
	union
	{
		struct {
			uint32_t objectIndex; //object index of the reference
			uint32_t offset; //offset in object of the reference
		};
		uint64_t objectIndexAndOffset;
	};

	Reference(){}
	Reference(uint64_t objectIndexAndOffset) :objectIndexAndOffset(objectIndexAndOffset){}
	Reference(uint32_t objectIndex, uint32_t offset) :objectIndex(objectIndex), offset(offset){}
	uint64_t *address(const std::vector<DataObject> &dataObjects) { return (uint64_t *)(dataObjects[objectIndex].buffer + offset); }
};



class StringTable
{
public:
	StringTable()
	{
		_buffer.reserve(128 * 1024);
		_buffer.push_back(0); //0 is empty string
	}
	size_t allocateString(const char *str);
	std::vector<char> _buffer;
};

size_t StringTable::allocateString(const char *str)
{
	if (str[0] == 0)
		return 0;
	size_t len = strlen(str);
	size_t size = _buffer.size();
	_buffer.resize(size + len + 1);
	memcpy(&_buffer[0] + size, str, len + 1);
	return size;
}

struct GeneratorContext
{
	const char *filename;
	int errorCount;
	std::vector<DataObject> dataObjects;
	StringTable stringTable;
	std::vector<bool> referenced;
	std::vector<int> rootObjects;
	std::vector<std::vector<uint64_t>> references;
};

static const char *_typeStrings[kMemberTypeCount] =
{
	"bool",
	"uint",
	"int",
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

void outputTypeCompatibilityError(const GclMember *member, const GenObject *objectType, const GenMember *memberType, GeneratorContext &context)
{
	GclValue *value = member->value;
	printf("%s(%i,%i): Error: value of type %s expected for property %s::%s, type %s is invalid\n", context.filename, (int)value->line, (int)value->col, _typeStrings[memberType->type], objectType->name, memberType->name, _valueTypeStrings[value->type]);
}

struct ConvertedValue
{
	bool converted;
	union
	{
		int intValue;
		float floatValue;
		float floatValues[4];
		uint64_t uint64Value;
	};
};

bool checkValueType(const GclMember *member, const GenObject *objectType, const GenMember *memberType, GeneratorContext &context, ConvertedValue* convertedValue)
{
	GclValue *value = member->value;
	if (convertedValue)
	{
		convertedValue->converted = false;
		convertedValue->intValue = 0;
	}

	bool ret = true;

	GclValueType valueType = value->type;
	float fValue = value->f4Value[0];
	float fValue1 = value->f4Value[1];
	float fValue2 = value->f4Value[2];
	float fValue3 = value->f4Value[3];
	int iValue = value->iValue;
	if (valueType == kValue_MINUSINF || valueType == kValue_PLUSINF)
	{
		if (memberType->type == kMemberType_INT || memberType->type == kMemberType_UINT)
		{
			valueType = kValue_INT;
			iValue = (valueType == kValue_MINUSINF ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max());
		}
		else if (memberType->type == kMemberType_FLOAT || memberType->type == kMemberType_UFLOAT)
		{
			valueType = kValue_FLOAT;
			fValue = (valueType == kValue_MINUSINF ? std::numeric_limits<float>::min() : std::numeric_limits<float>::max());
		}
	}
	
	switch (memberType->type)
	{
		case kMemberType_UINT:
		{
			ret = (valueType == kValue_INT && iValue >= 0);
			if (ret)
			{
				convertedValue->converted = true;
				convertedValue->intValue = iValue;
			}
			else
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		}
		case kMemberType_INT:
		{
			ret = (valueType == kValue_INT);
			if (ret)
			{
				convertedValue->converted = true;
				convertedValue->intValue = iValue;
			}
			else
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		}
		case kMemberType_UFLOAT:
		{
			ret = (valueType == kValue_INT && iValue >= 0) || (valueType == kValue_FLOAT && fValue >= 0);
			if (ret)
			{
				convertedValue->converted = true;
				convertedValue->floatValue = valueType == kValue_FLOAT ? fValue : (float)iValue;
			}
			else
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		}
		case kMemberType_FLOAT:
		{
			ret = (valueType == kValue_INT || valueType == kValue_FLOAT);
			if (ret)
			{
				convertedValue->converted = true;
				convertedValue->floatValue = valueType == kValue_INT ? (float)iValue : fValue;
			}
			else
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		}
		case kMemberType_FLOAT2:
		{
			ret = (valueType == kValue_FLOAT || valueType == kValue_INT || valueType == kValue_FLOAT2);
			if (ret)
			{
				if (valueType == kValue_FLOAT2)
				{
					convertedValue->converted = true;
					convertedValue->floatValues[0] = fValue;
					convertedValue->floatValues[1] = fValue1;
				}
				else
				{
					float f = valueType == kValue_INT ? (float)iValue : fValue;
					convertedValue->floatValues[0] = f;
					convertedValue->floatValues[1] = f;
				}
			}
			else
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		}
		case kMemberType_FLOAT3:
		{
			ret = (valueType == kValue_FLOAT || valueType == kValue_INT || valueType == kValue_FLOAT3);
			if (ret)
			{
				convertedValue->converted = true;
				if (valueType == kValue_FLOAT3)
				{
					convertedValue->floatValues[0] = fValue;
					convertedValue->floatValues[1] = fValue1;
					convertedValue->floatValues[2] = fValue2;
				}
				else
				{
					float f = valueType == kValue_INT ? (float)iValue : fValue;
					convertedValue->floatValues[0] = f;
					convertedValue->floatValues[1] = f;
					convertedValue->floatValues[2] = f;
				}
			}
			else
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		}
		case kMemberType_FLOAT4:
		{
			ret = (valueType == kValue_FLOAT || valueType == kValue_INT || valueType == kValue_FLOAT4);
			if (ret)
			{
				convertedValue->converted = true;
				if (valueType == kValue_FLOAT4)
				{
					convertedValue->floatValues[0] = fValue;
					convertedValue->floatValues[1] = fValue1;
					convertedValue->floatValues[2] = fValue2;
					convertedValue->floatValues[3] = fValue3;
				}
				else
				{
					float f = valueType == kValue_INT ? (float)iValue : fValue;
					convertedValue->floatValues[0] = f;
					convertedValue->floatValues[1] = f;
					convertedValue->floatValues[2] = f;
					convertedValue->floatValues[3] = f;
				}
			}
			else
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		}
		case kMemberType_EXPRESSION:
			//ret = (valueType == kMemberType_EXPRESSION);
			ret = (valueType == kMemberType_STRING);
			if (!ret)
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		case kMemberType_STRING:
			ret = (valueType == kMemberType_STRING);
			if (ret)
			{
				convertedValue->converted = true;
				convertedValue->uint64Value = context.stringTable.allocateString(value->strValue);
			}
			else 
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		case kMemberType_OBJECT:
			ret = (valueType == kMemberType_ENUM || valueType == kMemberType_OBJECT);
			if (!ret)
				outputTypeCompatibilityError(member, objectType, memberType, context);
			break;
		case kMemberType_ENUM:
		{
			GenEnum* enumType = memberType->enumType;
			int enumValue = -1;
			if (valueType == kValue_STRING)
			{
				const char *valueString = value->strValue;
				for (int i = 0; i < (int)enumType->members.size(); i++)
				{
					if (strcmp(enumType->members[i].name, valueString) == 0)
					{
						enumValue = i;
						if (convertedValue)
						{
							convertedValue->converted = true;
							convertedValue->intValue = enumValue;
						}
						break;
					}
				}
			}
			if (enumValue == -1)
			{
				printf("%s(%i,%i): Error: value of type %s expected for property %s::%s, possible values: ", context.filename, (int)value->line, (int)value->col, enumType->name, objectType->name, memberType->name);
				for (int i = 0; i < (int)enumType->members.size(); i++)
				{
					printf((i == 0) ? "%s" : ", %s", enumType->members[i].name);
				}
				printf(".\n");
				ret = false;
			}
			break;
		}
		default:
			break;
	}
	return ret;
}

bool isChildOf(const GenObject *child, const GenObject *parent)
{
	while (child && child != parent)
	{
		child = child->parent;
	}
	return (child == parent);
}

struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};

struct ObjectTableIndex
{
	int index;
	const GenObject *typeObject;
	ObjectTableIndex() {}
	ObjectTableIndex(const ObjectTableIndex &o) :index(o.index), typeObject(o.typeObject){}
	ObjectTableIndex(int index, const GenObject *typeObject) :index(index), typeObject(typeObject){}
};
typedef std::map<const char*, ObjectTableIndex, cmp_str> ObjectTable;
extern FrameAllocator *_allocator;

void addDataObject(DataObject &data, GeneratorContext &context, char *buffer, size_t size, const GclObject *object, GenericGcArray *gcArray, const GclMember *instanceMember)
{
	data.object = object;
	data.gcArray = gcArray;
	data.gcArrayInstanceMember = instanceMember;
	data.buffer = buffer;
	data.size = size;
}

void setMember(GeneratorContext &context, char *buffer, const GenMember *member, const GclItemList* l, const GclMember *instanceMember, const void *data)
{
	char *dest = buffer + member->offset;

	size_t size = (member->modifiers & GML_TYPE_MODIFIER_REF) ? 8 : member->size;
	if (member->modifiers & GML_TYPE_MODIFIER_ARRAY)
	{
		size = getMemberSize(member->type, member->objectType, false, (member->modifiers & GML_TYPE_MODIFIER_REF) != 0);
		
		GenericGcArray *gcArray = (GenericGcArray *)(buffer + member->offset);
		//check if we need to allocate this array
		if (gcArray->_elements == 0)
		{
			//count the number of elements in the array
			size_t count = 0;
			while (l)
			{
				if (l->member)
					count += (l->member->memberType == member) ? 1 : 0;
				l = l->next;
			}
			char *buffer = (char*)_allocator->allocate(count * size);
			gcArray->_size = 0;

			context.dataObjects.emplace_back();
			size_t referenceIndex = context.dataObjects.size() - 1;
			DataObject &data = context.dataObjects[referenceIndex];
			addDataObject(data, context, buffer, count * size, NULL, gcArray, instanceMember);
			gcArray->_elements = referenceIndex;

			//context.references[context.dataObjects.size() - 1].push_back((uint64_t*)&gcArray->_elements);
			//data.references.emplace_back(member->offset + offsetof(GenericGcArray, _elements), referenceIndex);
			//TODO to verify, I am not sure I have the right data object, I might  need to store this in the parent, in the owner of this member
		}
		dest = context.dataObjects[gcArray->_elements].buffer + size*gcArray->_size;
		gcArray->_size++;
	}

	memcpy(dest, data, size);
}

int convertObject(const GclObject *object, ObjectTable &globals, GeneratorContext &context)
{
	int objectIndex = -1;
	
	//we might have already pre-allocated the slot for that object.
	if (object->name && object->name->value)
	{
		const ObjectTable::iterator it = globals.find(object->name->value);
		if (it != globals.end())
		{
			objectIndex = it->second.index;
		}
	}
	if (objectIndex == -1)
	{
		context.dataObjects.emplace_back();
		objectIndex = (int)context.dataObjects.size() - 1;
	}
	
	int errorCount = 0;
	
	const GenObject *objectType = object->typeObject;
	if (!objectType)
		return 1; //for now
	char *buffer = NULL;
	
	//allocate memory for an instance of the object and initialize with the default values for each member
	size_t objectSize = objectType->size;
	if (objectSize)
	{
		buffer = new char[objectSize];
		if (objectType->size)
			memcpy(buffer, objectType->defaultObject, objectSize);
	}
	//gcAction *action = (gcAction*)buffer;
	
	//override the default with the values defined for this instance
	GclItemList *l = object->itemList;

	//go through the list a first time to resolve the member name and find their type
	//it's done in advance to find the size of an array by counting the array elements when handling the first array item
	//we'll emit errors only after, so that they're interleaved with the other code generation errors
	while (l)
	{
		if (l->member)
		{
			const char *name = l->member->name->value;
			l->member->memberType = NULL;
			const GenObject *objectType0 = objectType;
			while (objectType0)
			{
				for (int i = 0; i < (int)objectType0->members.size(); i++)
				{
					if (strcmp(name, objectType0->members[i].name) == 0)
					{
						l->member->memberType = &objectType0->members[i];
						break;
					}
				}
				if (l->member->memberType)
					break;
				objectType0 = objectType0->parent;
			}
		}
		l = l->next;
	}

	l = object->itemList;

	ObjectTable locals;
	if (!globals.empty())
		locals.insert(globals.begin(), globals.end());
	while (l)
	{
		if (l->object)
		{
			//object defined in the local space of the current object
			//instantiate it and add it to the list of locals, one or multiple members will probably reference it
			int subObjectIndex = convertObject(l->object, locals, context);
		}
		else while (l->member) //should an if, but I want to be able to break in case of error to go to the next member
		{
			
			const GenMember *member = l->member->memberType;
			if (!member)
			{
				//unknown member name
				errorCount++;
				printf("file:(%i): Error: %s is not a member of %s\n", (int)l->member->line, l->member->name->value, objectType->name);
				break;
			}
			else
			{
				//found the member, check the type and try to assign it.
				GclValue *value = l->member->value;
				GmlMemberTypeType type = member->type;
				if (type != kMemberType_OBJECT)
				{
					if (!value)
					{
						errorCount++;
						printf("%s(%i,%i): Error: expected a value of type %s but object given\n", context.filename, (int)l->member->value->line, (int)l->member->value->col,_typeStrings[type]);
						break;
					}
					
					ConvertedValue convertedValue;
					memset(&convertedValue, 0, sizeof(convertedValue));
					bool valid = checkValueType(l->member, objectType, member, context, &convertedValue);
					if (!valid)
					{
						errorCount++;
						break;
					}
					
					if (convertedValue.converted)
						setMember(context, buffer, member, l, l->member, &convertedValue.intValue); //memcpy(buffer + member->offset, &convertedValue.intValue, member->size);
					else
						setMember(context, buffer, member, l, l->member, value->strValue); //memcpy(buffer + member->offset, value->strValue, member->size);
				}
				else
				{
					DataObject *memberData = NULL;
					int subObjectIndex = -1;
					const GenObject *subObjectTypeObject = NULL;
					const char *subObjectName = "no name";
					size_t line;
					size_t col;
					
					//the gamecode either inlines or references the object
					if (value) //reference
					{
						line = value->line;
						col = value->col;
						//referenced should be a string, should correspond to a known object
						if (value->type != kValue_STRING)
						{
							errorCount++;
							printf("%s(%i,%i): Error: expected inlined object or reference to object\n", context.filename, (int)l->member->value->line, (int)l->member->value->col);
							break;
						}
						ObjectTable::iterator it = locals.find(value->strValue);
						if (it == locals.end())
						{
							errorCount++;
							printf("%s(%i,%i): Error: unknown instance %s\n", context.filename, (int)l->member->value->line, (int)l->member->value->col, value->strValue);
							break;
						}
						subObjectIndex = it->second.index;
						subObjectTypeObject = it->second.typeObject;
						subObjectName = value->strValue;
						memberData = &context.dataObjects[subObjectIndex];
					}
					else if (l->member->object) //inline
					{
						if (l->member->object->typeObject == NULL)
						{
							l->member->object->typeObject = member->objectType;
						}
						
						line = l->member->object->line;
						col = l->member->object->col;

						subObjectIndex = convertObject(l->member->object, locals, context);
						memberData = &context.dataObjects[subObjectIndex];
						subObjectTypeObject = l->member->object->typeObject;
					}

					if (memberData)
					{
						//the member is either a reference on a object or the object itself
						if (member->modifiers & GML_TYPE_MODIFIER_REF)
						{
							if (!isChildOf(subObjectTypeObject, member->objectType))
							{
								errorCount++;
								if (subObjectName)
									printf("%s(%i,%i): Error: value of type inherited from %s expected for property %s::%s, %s is of type %s which is not a child of %s.\n", 
									context.filename, (int)line, (int)col, member->objectType->name, objectType->name, member->name, subObjectName, subObjectTypeObject->name, member->objectType->name);
								else
									printf("%s(%i,%i): Error: value of type inherited from %s expected for property %s::%s, object passed is of type %s which is not a child of %s.\n", 
									context.filename, (int)line, (int)col, member->objectType->name, objectType->name, member->name, subObjectTypeObject->name, member->objectType->name);
								break;
							}
							uint64_t ref = subObjectIndex;
							setMember(context, buffer, member, l, l->member, &ref);
							//context.references[subObjectIndex].push_back((uint64_t*)dest);
							//data->references.emplace_back(offset, subObjectIndex);
							//memcpy(buffer + member->offset, &ref, member->size);
						}
						else
						{
							if (subObjectTypeObject != member->objectType)
							{
								errorCount++;
								if (memberData->object->name)
									printf("%s(%i,%i): Error: value of type %s expected for non ref property %s::%s, %s is of type %s, child types not accepted for non ref.\n",
										context.filename, (int)line, (int)col, member->objectType->name, objectType->name, member->name, memberData->object->name->value, memberData->object->typeObject->name);
								else
									printf("%s(%i,%i): Error: value of type %s expected for non ref property %s::%s, object passed is of type %s, child types not accepted for non ref.\n", 
										context.filename, (int)line, (int)col, member->objectType->name, objectType->name, member->name, memberData->object->typeObject->name);
								break;
							}
							if (memberData->object == NULL)
							{
								printf("%s(%i,%i): Error: value of type %s expected for non ref property %s::%s, object passed is of the correct type but has not been defined yet, child types not accepted for non ref.\n",
									context.filename, (int)line, (int)col, member->objectType->name, objectType->name, member->name);
							}
							setMember(context, buffer, member, l, l->member, memberData->buffer);
							//memcpy(buffer + member->offset, data->buffer, member->size);
						}
					}
				}
			}
			break;
		}
		l = l->next;
	}
	
	DataObject &data = context.dataObjects[objectIndex];
	addDataObject(data, context, buffer, object->typeObject->size, object, NULL, NULL);
	
	if (object->name && object->name->value)
		globals[object->name->value] = ObjectTableIndex(objectIndex, object->typeObject);

	context.errorCount += errorCount;
	return objectIndex;
}

void buildReferences(GeneratorContext &context, size_t index);
void buildObjectReferences(GeneratorContext &context, char *objectBuffer, const GenObject *typeObject, char *dataObjectBuffer, int dataObjectIndex);

void buildMemberReferences(GeneratorContext &context, char *memberBuffer, unsigned int modifier, GmlMemberTypeType memberType, const GenObject *typeObject, char *dataObjectBuffer, int dataObjectIndex)
{
	if (modifier & GML_TYPE_MODIFIER_ARRAY)
	{
		GenericGcArray *gcArray = (GenericGcArray*)memberBuffer;
		if (gcArray->_size)
		{
			uint64_t *refPtr = &gcArray->_elements;
			uint64_t ref = gcArray->_elements;
			size_t offset = (char*)refPtr - dataObjectBuffer;
			Reference r((uint32_t)dataObjectIndex, (uint32_t)offset);
			context.references[ref].push_back(r.objectIndexAndOffset); //register the reference pointing from objectIndexAndOffset to ref
			buildReferences(context, ref);
		}
	}
	else if (modifier & GML_TYPE_MODIFIER_REF)
	{
		uint64_t *refPtr = (uint64_t *)memberBuffer;
		uint64_t ref = *refPtr;
		if (ref)
		{
			size_t offset = (char*)refPtr - dataObjectBuffer;
			Reference r((uint32_t)dataObjectIndex, (uint32_t)offset);
			context.references[ref].push_back(r.objectIndexAndOffset); //register the reference pointing from objectIndexAndOffset to ref
			buildReferences(context, ref);
		}
	}
	else if (memberType == kMemberType_OBJECT)
	{
		buildObjectReferences(context, memberBuffer, typeObject, dataObjectBuffer, dataObjectIndex);
	}
}

void buildObjectReferences(GeneratorContext &context, char *objectBuffer, const GenObject *typeObject, char *dataObjectBuffer, int dataObjectIndex)
{
	const std::vector<GenMember> &members = typeObject->members;
	size_t memberCount = members.size();
	for (int i = 0; i < memberCount; i++)
	{
		const GenMember &member = members[i];
		buildMemberReferences(context, objectBuffer + member.offset, member.modifiers, member.type, member.objectType, dataObjectBuffer, dataObjectIndex);
	}
}

void buildReferences(GeneratorContext &context, size_t index)
{
	if (context.referenced[index]) //if the objects is already referenced, it means that we have already processed its dependencies, skip it
		return;
	context.referenced[index] = true;

	DataObject &data = context.dataObjects[index];
	if (data.object)
	{
		buildObjectReferences(context, data.buffer, data.object->typeObject, data.buffer, (int)index);
	}
	else if (data.gcArray)
	{
		if (data.gcArrayInstanceMember->memberType->type == kMemberType_OBJECT)
		{
			unsigned int modifiers = data.gcArrayInstanceMember->memberType->modifiers;
			bool isRef = (modifiers & GML_TYPE_MODIFIER_REF) != 0;
			//bool isArray = (modifiers & GML_TYPE_MODIFIER_ARRAY) != 0;
			int count = (int)data.gcArray->_size;
			//size_t size = isRef ? 8 : (isArray ? sizeof(GenericGcArray) : data.gcArrayInstanceMember->memberType->size);
			size_t size = isRef ? 8 : data.gcArrayInstanceMember->memberType->objectType->size;
			for (int i = 0; i < count; i++)
			{
				char *buffer = data.buffer + size*i;
				//buildObjectReferences(context, buffer, data.gcArrayInstanceMember->memberType->objectType);
				buildMemberReferences(context, buffer, modifiers & GML_TYPE_MODIFIER_REF, kMemberType_OBJECT, data.gcArrayInstanceMember->memberType->objectType,data.buffer,(int)index);
			}
		}
	}
}

void buildReferences(GeneratorContext &context)
{
	size_t rootObjectCount = context.rootObjects.size();

	std::vector<bool> referenced;
	context.referenced.resize(context.dataObjects.size());
	
	for (size_t i = 0; i < rootObjectCount; i++)
	{
		int index = context.rootObjects[i];
		buildReferences(context, index);
	}
}

//create blocks of memory

static inline size_t alignUpper(size_t size, uint64_t powerOf2Alignment)
{
	size_t alignedSize = ((size + powerOf2Alignment - 1)&~(powerOf2Alignment-1));
	return alignedSize;
}

void writeVTablePtrPatch(const std::vector<size_t> &vTablePtrOffsets, char *objectPtr, size_t objectsRelativeOffset, uint64_t* &previousVTablePointer)
{
	size_t count = vTablePtrOffsets.size();
	for (size_t j = 0; j < count; j++)
	{
		size_t offset = vTablePtrOffsets[j];

		uint64_t *refPtr = (uint64_t *)(objectPtr + offset);
		uint64_t typeIndex = *refPtr;

		*refPtr = 0x00000000ffffffff | (typeIndex << 32);
		if (previousVTablePointer)
		{
			size_t currentReferenceObjectsRelativeOffset = objectsRelativeOffset + offset;
			*previousVTablePointer = ((*previousVTablePointer) & 0xffffffff00000000) | (uint32_t)currentReferenceObjectsRelativeOffset;
			previousVTablePointer = refPtr;
		}
	}
}

void writeStringPatch(const std::vector<size_t> &strPtrOffsets, char *objectPtr, size_t objectsRelativeOffset, size_t stringTableObjectOffset, uint64_t* &previousStringPointer)
{
	size_t count = strPtrOffsets.size();
	for (size_t j = 0; j < count; j++)
	{
		size_t offset = strPtrOffsets[j];

		uint64_t *refPtr = (uint64_t *)(objectPtr + offset);
		uint64_t stringObjectOffset = stringTableObjectOffset + *refPtr;

		*refPtr = 0x00000000ffffffff | (stringObjectOffset << 32);

		if (previousStringPointer)
		{
			size_t currentReferenceObjectsRelativeOffset = objectsRelativeOffset + offset;
			*previousStringPointer = ((*previousStringPointer) & 0xffffffff00000000) | (uint32_t)currentReferenceObjectsRelativeOffset;
			previousStringPointer = refPtr;
		}
	}
}

char* serializeOutput(GeneratorContext &context, size_t *size)
{
	size_t objectCount = 0;
	size_t objectsSize = 0;
	size_t stringTableSize = context.stringTable._buffer.size();

	for (size_t i = 0; i < context.rootObjects.size(); i++)
	{
		context.references[context.rootObjects[i]].push_back(-1);
	}

	for (size_t i = 0; i < context.references.size(); i++)
	{
		if (context.references[i].size())
		{
			objectCount++;
			size_t objectSize = context.dataObjects[i].size;
			objectSize = ((objectSize + 15)&~15);
			objectsSize += objectSize;
		}
	}

	//TODO, I am not cleaning the string table, it can contain strings from unreferenced objects

	size_t objectTableoffset = alignUpper(sizeof(GcBlockHeader),16);
	size_t objectOffset = alignUpper(objectTableoffset + sizeof(GcObjectHeader)*objectCount, 16);
	size_t stringTableOffset = alignUpper(objectOffset + objectsSize, 16);
	size_t blockSize = alignUpper(stringTableOffset + stringTableSize,16);
	
	char *buffer = (char*)_allocator->allocate(blockSize);
	memset(buffer, 0, objectOffset);

	GcBlockHeader *header = (GcBlockHeader *)buffer;
	header->blockSize = blockSize;
	header->objectCount = objectCount;
	header->objectTableOffset = objectTableoffset;
	header->objectOffset = objectOffset;
	header->stringTableOffset = stringTableOffset;
	header->firstPointerOffset = 0x00000000ffffffff;
	header->firstVTablePointerOffset = 0x00000000ffffffff;
	header->firstStringPointerOffset = 0x00000000ffffffff;

	//compute offsets and patch references
	GcObjectHeader *objectHeaders = (GcObjectHeader *)(buffer + header->objectTableOffset);
	size_t index = 0;
	size_t offset = 0;
	
	std::vector<int> objectsExportIndices;
	objectsExportIndices.resize(context.dataObjects.size());
	std::vector<int> exportedObjects;

	//for (int i = (int)context.references.size()-1; i >= 0; i--) //I don't know why I was doing that???
	for (int i = 0; i<(int)context.references.size(); i++) //I don't know why I was doing that???
	{
		if (context.references[i].size())
		{
			size_t objectSize = context.dataObjects[i].size;
			objectHeaders[index].offset = offset + header->objectOffset;
			objectHeaders[index].size = objectSize;
			if (context.dataObjects[i].object)
			{
				if (context.dataObjects[i].object->name)
				{
					strncpy(objectHeaders[index].name, context.dataObjects[i].object->name->value, sizeof(objectHeaders[index].name) - 1);
					objectHeaders[index].name[sizeof(objectHeaders[index].name) - 1] = 0;
				}
				const GenObject *objectType = context.dataObjects[i].object->typeObject;
				objectHeaders[index].typeID = objectType->hash;
				strncpy(objectHeaders[index].dbgTypeName, objectType->name, sizeof(objectHeaders[index].dbgTypeName) - 1);
				objectHeaders[index].dbgTypeName[sizeof(objectHeaders[index].dbgTypeName) - 1] = 0;
			}
			else
			{
				objectHeaders[index].typeID = -1;
				if (context.dataObjects[i].gcArrayInstanceMember->object)
				{
					const GenObject *objectType = context.dataObjects[i].gcArrayInstanceMember->object->typeObject;
					strncpy(objectHeaders[index].dbgTypeName, objectType->name, sizeof(objectHeaders[index].dbgTypeName) - 1);
					objectHeaders[index].dbgTypeName[sizeof(objectHeaders[index].dbgTypeName) - 1] = 0;
				}
				else
				{
					const char *typeName = _typeStrings[context.dataObjects[i].gcArrayInstanceMember->memberType->type];
					strncpy(objectHeaders[index].dbgTypeName, typeName, sizeof(objectHeaders[index].dbgTypeName) - 1);
					objectHeaders[index].dbgTypeName[sizeof(objectHeaders[index].dbgTypeName) - 1] = 0;
				}
				strcpy(objectHeaders[index].name, "@array");
			}

			offset += alignUpper(objectSize, 16);
			
			exportedObjects.push_back(i);
			objectsExportIndices[i] = (int)index;
			index++;
		}
	}

	
	//process the references.
	//for storage, each reference is a uint64_t, 
	//high uint32_t: the offset of the object referenced relative to the start of the objects array
	//low uint32_t: the offset of the next reference relative to the start of the objects array
	uint64_t *previousReference = &header->firstPointerOffset;
	uint64_t *previousVTablePointer = &header->firstVTablePointerOffset;
	uint64_t *previousStringPointer = &header->firstStringPointerOffset;

	size_t count = exportedObjects.size();
	for (index = 0; index < count; index++)
	{
		int i = exportedObjects[index];
		size_t objectsRelativeOffset = objectHeaders[index].offset - header->objectOffset;
		
		//object references
		std::vector<uint64_t> &references = context.references[i];
		size_t count1 = references.size();
		for (size_t j = 0; j < count1; j++)
		{
			uint64_t refObjectIndexAndOffset = references[j];
			if (refObjectIndexAndOffset != -1) //if -1 , it's a root object, ignore
			{
				Reference r(refObjectIndexAndOffset);
				uint64_t *refPtr = r.address(context.dataObjects);
				*refPtr = 0x00000000ffffffff | (objectsRelativeOffset<<32);
				if (previousReference)
				{
					size_t currentReferenceObjectsRelativeOffset = objectHeaders[objectsExportIndices[r.objectIndex]].offset - header->objectOffset + r.offset;
					*previousReference = ((*previousReference) & 0xffffffff00000000) | (uint32_t)currentReferenceObjectsRelativeOffset;
					previousReference = refPtr;
				}
			}
		}

		//patch chain for the vtable pointers
		const DataObject &dataObject = context.dataObjects[i];
		if (dataObject.object)
		{
			const std::vector<size_t> &vTablePtrOffsets = dataObject.object->typeObject->vTablePtrOffsets;
			writeVTablePtrPatch(vTablePtrOffsets, dataObject.buffer, objectsRelativeOffset, previousVTablePointer);
		}
		else if (dataObject.gcArray)
		{
			if (dataObject.gcArrayInstanceMember->memberType->type == kMemberType_OBJECT)
			{
				bool isRef = (dataObject.gcArrayInstanceMember->memberType->modifiers & GML_TYPE_MODIFIER_REF) != 0;
				if (!isRef)
				{
					const std::vector<size_t> &vTablePtrOffsets = dataObject.gcArrayInstanceMember->memberType->objectType->vTablePtrOffsets;
					for (int k = 0; k < dataObject.gcArray->_size; k++)
					{
						size_t arrayElementOffset = dataObject.gcArrayInstanceMember->memberType->objectType->size*k;
						writeVTablePtrPatch(vTablePtrOffsets, dataObject.buffer + arrayElementOffset, objectsRelativeOffset + arrayElementOffset, previousVTablePointer);
					}
				}
			}
		}

		//patch chain for the string pointers
		size_t stringTableObjectOffset = header->stringTableOffset - header->objectOffset;
		if (dataObject.object)
		{
			const std::vector<size_t> &strPtrOffsets = dataObject.object->typeObject->strPtrOffsets;
			writeStringPatch(strPtrOffsets, dataObject.buffer, objectsRelativeOffset, stringTableObjectOffset, previousStringPointer);
		}
		else if (dataObject.gcArray)
		{
			if (dataObject.gcArrayInstanceMember->memberType->type == kMemberType_OBJECT)
			{
				bool isRef = (dataObject.gcArrayInstanceMember->memberType->modifiers & GML_TYPE_MODIFIER_REF)!=0;
				if (!isRef)
				{
					const std::vector<size_t> &strPtrOffsets = dataObject.gcArrayInstanceMember->memberType->objectType->strPtrOffsets;
					for (int k = 0; k < dataObject.gcArray->_size; k++)
					{
						size_t arrayElementOffset = dataObject.gcArrayInstanceMember->memberType->objectType->size*k;
						writeStringPatch(strPtrOffsets, dataObject.buffer + arrayElementOffset, objectsRelativeOffset + arrayElementOffset, stringTableObjectOffset, previousStringPointer);
					}
				}
			}
		}
	}

	for (index = 0; index < exportedObjects.size(); index++)
	{
		int i = exportedObjects[index];

		size_t objectOffset = objectHeaders[index].offset;
		size_t objectSize = objectHeaders[index].size;
			
		size_t alignedSize = alignUpper(objectSize, 16);
		char *dest = buffer + objectOffset;
		memcpy(dest, context.dataObjects[i].buffer, objectSize);
		size_t emptySpace = alignedSize - objectSize;
		if (emptySpace)
			memset(dest + objectSize, 0, emptySpace);
	}

	char *dest = buffer + header->stringTableOffset;
	memcpy(dest, &context.stringTable._buffer[0], stringTableSize);
	size_t emptySpace = blockSize - stringTableOffset;
	if (emptySpace)
		memset(dest + stringTableSize, 0, emptySpace);

	*size = blockSize;
	return buffer;
}

int generateOutput(GclObjectList *objectList, const Meta *meta, const char *filename, const char *outputPath, bool printResult)
{
	ObjectTable globals;

	GeneratorContext context;
	context.filename = filename;
	context.errorCount = 0;
	//add an object at slot 0, to avoid having references set to 0, which is also the NULL reference, it won't be exported.
	context.dataObjects.emplace_back();
	size_t referenceIndex = context.dataObjects.size() - 1;
	DataObject &data = context.dataObjects[referenceIndex];
	addDataObject(data, context, NULL, 0, NULL, NULL, NULL);
	
	//build the list of all the global objects for referencing (because we don't have forward declaration)
	//this means that we only support referencing a not yet declared object if that object is global
	std::map<std::string, int> globalObjects;
	GclObjectList *l = objectList;
	while (l)
	{
		if (l->object->name && l->object->name->value)
		{
			//TODO: I am doing doing anything with it here, did I mean to check if 2 globals had the same name?
			ObjectTable::iterator it = globals.find(l->object->name->value); 
			if (it != globals.end())
			{
				printf("%s: duplicate object name\n", l->object->name->value);
				return -1;
			}
			context.dataObjects.emplace_back();
			size_t referenceIndex = context.dataObjects.size() - 1;
			DataObject &data = context.dataObjects[referenceIndex];
			addDataObject(data, context, NULL, 0, NULL, NULL, NULL);
			globals[l->object->name->value] = ObjectTableIndex((int)referenceIndex, l->object->typeObject);
			l = l->next;
		}
	}

	l = objectList;
	while (l)
	{
		if (l->object->name && l->object->name->value)
		{
			printf("    %s(%s)\n", l->object->typeObject->name, l->object->name->value);
		}

		int index = convertObject(l->object, globals, context);
		if (index >= 0)
		{
			if (context.dataObjects[index].object->name)
				globals[context.dataObjects[index].object->name->value] = ObjectTableIndex((int)index, context.dataObjects[index].object->typeObject);
			context.rootObjects.emplace_back(index);
		}
		l = l->next;
	}
	context.references.resize(context.dataObjects.size());

	//go over the data and follow the dependency to determine the list of objects to write (and the dependency tree maybe?)
	buildReferences(context);
	
	if (!context.errorCount)
	{
		size_t size = 0;
		char *bin = serializeOutput(context, &size);
		if (bin)
		{
			FILE *fp = fopen(outputPath, "wb");
			fwrite(bin, size, 1, fp);
			fclose(fp);
		}
		else
		{
			context.errorCount++;
		}
	}

	/*if (context.errorCount)
	{
		printf("========== Build: 0 succeeded, 1 failed, %i errors ==========\n", context.errorCount);
	}
	else
	{
		printf("========== Build: 1 succeeded, 0 failed, 0 error ==========\n");
	}*/
	
	return context.errorCount;
}
