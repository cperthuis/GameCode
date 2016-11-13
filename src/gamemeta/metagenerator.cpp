#include "metagenerator.h"

#include "meta.h"

#include <stdio.h>
#include <stdarg.h>

#include <set>
#include <string>
#include <vector>
#include <stdint.h>

const char *getTypeString(GenMember *member)
{
	switch (member->type)
	{
	case kMemberType_BOOL:
		return "bool";
		break;
	case kMemberType_UINT:
		return "uint";
		break;
	case kMemberType_INT:
		return "int";
		break;
	case kMemberType_UFLOAT:
		return "ufloat";
		break;
	case kMemberType_FLOAT:
		return "float";
		break;
	case kMemberType_FLOAT2:
		return "float2";
		break;
	case kMemberType_FLOAT3:
		return "float3";
		break;
	case kMemberType_FLOAT4:
		return "float4";
		break;
	case kMemberType_EXPRESSION:
		return "expression";
		break;
	case kMemberType_STRING:
		return "string";
		break;
	case kMemberType_OBJECT:
		return member->objectType->name;
		break;
	case kMemberType_ENUM:
		return member->enumType->name;
		break;
	default:
		return "unknown";
	}
}

typedef std::set<const char *> IncludeSet;
const char *getCTypeString(GenMember *member, IncludeSet &objectIncludes, IncludeSet &enumIncludes, IncludeSet &extraIncludes, IncludeSet &forwardDeclarations)
{
	if (member->modifiers & GML_TYPE_MODIFIER_ARRAY)
		extraIncludes.insert("gc");
	
	switch (member->type)
	{
	case kMemberType_BOOL:
		return "bool ";
		break;
	case kMemberType_UINT:
		return "unsigned int ";
		break;
	case kMemberType_INT:
		return "int ";
		break;
	case kMemberType_UFLOAT:
		return "float ";
		break;
	case kMemberType_FLOAT:
		return "float ";
		break;
	case kMemberType_FLOAT2:
		extraIncludes.insert("gc");
		return "Vec2 ";
		break;
	case kMemberType_FLOAT3:
		extraIncludes.insert("gc");
		return "Vec3 ";
		break;
	case kMemberType_FLOAT4:
		extraIncludes.insert("gc");
		return "Vec4 ";
		break;
	case kMemberType_EXPRESSION:
		extraIncludes.insert("gc");
		return "GcExpression ";
		break;
	case kMemberType_STRING:
		return "const char *";
		break;
	case kMemberType_OBJECT:
		if (member->modifiers & GML_TYPE_MODIFIER_REF)
		{
			forwardDeclarations.insert(member->objectType->name);
			return member->objectType->name;
		}
		else
		{
			objectIncludes.insert(member->objectType->name);
			return member->objectType->name;
		}
		break;
	case kMemberType_ENUM:
		enumIncludes.insert(member->enumType->name);
		return member->enumType->name;
		break;
	default:
		return "unknown ";
	}
}

const char *getTypeModifierString(GenMember *member, int index)
{
	int index0 = 0;
	if (member->modifiers & GML_TYPE_MODIFIER_NONZERO)
	{
		if (index0 == index)
			return "nonzero";
		index0++;
	}
	if (member->modifiers & GML_TYPE_MODIFIER_REF)
	{
		if (index0 == index)
			return "ref";
		index0++;
	}
	return "";
}

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
	char *oEnd = buffer + size - 1;
	const char *p = input;
	while (*p && o<oEnd)
	{
		char c = *p++;
		*o++ = (c >= 'a' && c <= 'z') ? c + 'A' - 'a' : c;
	}
	*o = 0;
	return buffer;
}

int generateOutput(const Meta *meta, const char *outputPath, bool printResult)
{

	char buffer0[1024];
	char buffer1[1024];

#define TO_UPPER_0(a) strToUpper(buffer0,sizeof(buffer0),a)
#define FORMAT_1(format,...) strFormat(buffer1,sizeof(buffer1),format,__VA_ARGS__)
#define EOL
#define OBJ_PREFIX "gc"
#define OBJ_PREFIX_CAPITAL "GC"

	std::string all;
	all += "#ifndef " OBJ_PREFIX_CAPITAL "ALL_H\r\n#define " OBJ_PREFIX_CAPITAL "ALL_H\r\n\r\n";

	for (std::set<GenEnum*, lessGenEnum>::iterator it = meta->enumSet.begin(); it != meta->enumSet.end(); it++)
	{
		GenEnum *genEnum = (*it);
		/*printf("enum %s\n", genEnum->name);
		for (int i = 0; i < (int)genEnum->members.size(); i++)
		{
		printf("\t%s = %i\n", genEnum->members[i].name, genEnum->members[i].value);
		}*/

		std::string str;
		const char *enumName = genEnum->name;
		const char *p = TO_UPPER_0(enumName);

		/*if (enumName[0] == 'e' && enumName[1] >= 'A' && enumName[1] <= 'Z')
		{
		enumName++;
		}*/

		str += FORMAT_1("#ifndef %s_H\r\n#define %s_H\r\n\r\n"
			"namespace %s\r\n{\r\n"
			"\tenum value\r\n\t{\r\n", p, p, enumName);

		int count = (int)genEnum->members.size();
		for (int i = 0; i < count; i++)
		{
			if (i == count - 1)
				str += FORMAT_1("\t\t%s = %i\r\n", genEnum->members[i].name, genEnum->members[i].value);
			else
				str += FORMAT_1("\t\t%s = %i,\r\n", genEnum->members[i].name, genEnum->members[i].value);
		}

		str += FORMAT_1("\t};\r\n}\r\n\r\n#endif // %s_H\r\n", p);
		if (printResult)
			printf(str.c_str());

		const char *filename = FORMAT_1("%s\\%s.h", outputPath, enumName);
		FILE *fp = fopen(filename, "wb");
		if (fp)
		{
			fwrite(str.c_str(), str.size(), 1, fp);
			fclose(fp);
		}
		else
		{
			printf("Error: couldn't write out %s", filename);
		}

		all += FORMAT_1("#include \"%s.h\"\r\n", enumName);
	}
	if (meta->enumSet.size())
		all += "\r\n";

	int objectIndex = 0;
	std::string vptrObjects;
	std::string tmpObjects;

	for (std::set<GenObject*, lessGenObject>::iterator it = meta->objectSet.begin(); it != meta->objectSet.end(); it++)
	{
		GenObject *genObject = (*it);
		/*printf("object %s%s%s\n", genObject->name, genObject->parent ? " : " : "", genObject->parent ? genObject->parent->name : "");
		for (int i = 0; i < (int)genObject->members.size(); i++)
		{
		GenMember *member = &genObject->members[i];
		const char *modifier0 = getTypeModifierString(member, 0);
		const char *modifier1 = getTypeModifierString(member, 1);
		printf("\t%s%s%s%s%s %s\n",
		modifier0, (*modifier0) ? " " : "",
		modifier1, (*modifier1) ? " " : "",
		getTypeString(member),
		genObject->members[i].name);
		}*/


		IncludeSet objectIncludes;
		IncludeSet enumIncludes;
		IncludeSet extraIncludes;
		IncludeSet forwardDeclarations;

		std::string str0;
		std::string str1;
		const char *objectName = genObject->name;
		const char *p = TO_UPPER_0(objectName);
		str0 += FORMAT_1("#ifndef " OBJ_PREFIX_CAPITAL "%s_H\r\n#define " OBJ_PREFIX_CAPITAL "%s_H\r\n\r\n", p, p);

		if (genObject->parent == NULL)
		{
			str0 += FORMAT_1("#define GC_%s 0x%X\r\n", TO_UPPER_0(objectName), genObject->hash);

			const std::vector<GenObject*> &derivedObjects = genObject->derivedObjects;
			int count = (int)derivedObjects.size();
			for (size_t i = 0; i < count; i++)
			{
				str0 += FORMAT_1("#define GC_%s 0x%X\r\n", TO_UPPER_0(derivedObjects[i]->name), derivedObjects[i]->hash);
			}

			str0 += FORMAT_1("\r\n");
		}
		
		if (genObject->parent)
			str1 += FORMAT_1("struct " OBJ_PREFIX "%s : public " OBJ_PREFIX "%s\r\n{ \r\n", objectName, genObject->parent->name);
		else
			str1 += FORMAT_1("struct " OBJ_PREFIX "%s\r\n{ \r\n", objectName);

		if (genObject->hasVTablePtr)
		{
			str1 += FORMAT_1("\tvirtual ~" OBJ_PREFIX "%s(){}\r\n\r\n", objectName);
			str1 += FORMAT_1("\tvirtual unsigned int getType() { return GC_%s; }\r\n\r\n", TO_UPPER_0(objectName));
		}
		
		int count = (int)genObject->members.size();
		for (int i = 0; i < count; i++)
		{
			GenMember *member = &genObject->members[i];

			const char *typeString = getCTypeString(member, objectIncludes, enumIncludes, extraIncludes, forwardDeclarations);
			const char *memberName = genObject->members[i].name;
			if (member->modifiers & GML_TYPE_MODIFIER_ARRAY)
			{
				if (member->type == kMemberType_OBJECT)
				{
					if (member->modifiers & GML_TYPE_MODIFIER_REF)
						str1 += FORMAT_1("\tGcArray<" OBJ_PREFIX "%s *> %s;\r\n", typeString, memberName);
					else
						str1 += FORMAT_1("\tGcArray<" OBJ_PREFIX "%s> %s; \r\n", typeString, memberName);
				}
				else
				{
					if (member->type == kMemberType_ENUM)
						str1 += FORMAT_1("\tGcArray<%s::value> %s;\r\n", typeString, memberName);
					else
						str1 += FORMAT_1("\tGcArray<%s> %s;\r\n", typeString, memberName);
				}
			}
			else
			{
				if (member->type == kMemberType_OBJECT)
				{
					if (member->modifiers & GML_TYPE_MODIFIER_REF)
						str1 += FORMAT_1("\t" OBJ_PREFIX "%s *%s;\r\n", typeString, memberName);
					else
						str1 += FORMAT_1("\t" OBJ_PREFIX "%s %s;\r\n", typeString, memberName);
				}
				else
				{
					if (member->type == kMemberType_ENUM)
						str1 += FORMAT_1("\t%s::value %s;\r\n", typeString, memberName);
					else
						str1 += FORMAT_1("\t%s%s;\r\n", typeString, memberName);
				}
			}
		}

		str1 += FORMAT_1("};\r\n\r\n#endif // %s_H\r\n", p);

		for (IncludeSet::iterator it = extraIncludes.begin(); it != extraIncludes.end(); it++)
		{
			str0 += FORMAT_1("#include <%s.h>\r\n", (*it));
		}
		if (extraIncludes.size())
			str0 += "\r\n";
		for (IncludeSet::iterator it = enumIncludes.begin(); it != enumIncludes.end(); it++)
		{
			str0 += FORMAT_1("#include \"%s.h\"\r\n", (*it));
		}
		if (enumIncludes.size())
			str0 += "\r\n";
		for (IncludeSet::iterator it = objectIncludes.begin(); it != objectIncludes.end(); it++)
		{
			str0 += FORMAT_1("#include \"" OBJ_PREFIX "%s.h\"\r\n", (*it));
		}
		if (genObject->parent)
		{
			str0 += FORMAT_1("#include \"" OBJ_PREFIX "%s.h\"\r\n", genObject->parent->name);
		}
		if (objectIncludes.size() || genObject->parent)
			str0 += "\r\n";
		for (IncludeSet::iterator it = forwardDeclarations.begin(); it != forwardDeclarations.end(); it++)
		{
			str0 += FORMAT_1("struct " OBJ_PREFIX "%s;\r\n", (*it));
		}
		if (forwardDeclarations.size())
			str0 += "\r\n";

		if (printResult)
			printf((str0 + str1).c_str());
		const char *filename = FORMAT_1("%s\\" OBJ_PREFIX "%s.h", outputPath, objectName);
		FILE *fp = fopen(filename, "wb");
		if (fp)
		{
			fwrite(str0.c_str(), str0.size(), 1, fp);
			fwrite(str1.c_str(), str1.size(), 1, fp);
			fclose(fp);
		}
		else
		{
			printf("Error: couldn't write out %s", filename);
		}

		all += FORMAT_1("#include \"" OBJ_PREFIX "%s.h\"\r\n", objectName);

		vptrObjects += FORMAT_1("\tnew (vptr) " OBJ_PREFIX "%s(); _gcVptr[%i] = vptr;\r\n", objectName, objectIndex);
		tmpObjects += FORMAT_1("\t\t" OBJ_PREFIX "%s _%s;\r\n", objectName, objectName);

		objectIndex++;
	}

	all += "\r\n#endif // " OBJ_PREFIX_CAPITAL "ALL_H\r\n";

	const char *filename = FORMAT_1("%s\\" OBJ_PREFIX "All.h", outputPath);
	FILE *fp = fopen(filename, "wb");
	if (fp)
	{
		fwrite(all.c_str(), all.size(), 1, fp);
		fclose(fp);
	}
	else
	{
		printf("Error: couldn't write out %s", filename);
	}

	std::string vptr;
	vptr += "#include \"gcAll.h\"\r\n#include <new>\r\n\r\n";
	vptr += FORMAT_1("#define GC_OBJECT_COUNT %i\r\n", objectIndex);
	vptr += "static void* _gcVptr[GC_OBJECT_COUNT];\r\n\r\n";
	vptr += "namespace\r\n{\r\n\tunion GcTmpObjectsContainer {\r\n";
	vptr += tmpObjects;
	vptr += "\t};\r\n}\r\n\r\n";
	vptr += "int gcGetObjectCount() { return GC_OBJECT_COUNT; }\r\n\r\n";
	vptr += "const void* const* gcInitializeVptrTable()\r\n{\r\n\tGcTmpObjectsContainer tmp;\r\n\tvoid *vptr = (void *)&tmp;\r\n";
	vptr += vptrObjects;
	vptr += "\r\n\treturn _gcVptr;\r\n}\r\n";
	
	filename = FORMAT_1("%s\\" OBJ_PREFIX "Vptr.cpp", outputPath);
	fp = fopen(filename, "wb");
	if (fp)
	{
		fwrite(vptr.c_str(), vptr.size(), 1, fp);
		fclose(fp);
	}
	else
	{
		printf("Error: couldn't write out %s", filename);
	}

	return 0;
}

