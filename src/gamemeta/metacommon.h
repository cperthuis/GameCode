#ifndef METACOMMON_H
#define METACOMMON_H

enum GmlMemberTypeType
{
	kMemberType_BOOL,
	kMemberType_UINT,
	kMemberType_INT,
	kMemberType_UFLOAT,
	kMemberType_FLOAT,
	kMemberType_FLOAT2,
	kMemberType_FLOAT3,
	kMemberType_FLOAT4,
	kMemberType_EXPRESSION,
	kMemberType_STRING,
	kMemberType_OBJECT,
	kMemberType_ENUM,
	kMemberTypeCount
};

#define GML_TYPE_MODIFIER_NONZERO 1
#define GML_TYPE_MODIFIER_REF 2
#define GML_TYPE_MODIFIER_ARRAY 4

enum GclValueType
{
	kValue_BOOL,
	kValue_INT,
	kValue_FLOAT,
	kValue_FLOAT2,
	kValue_FLOAT3,
	kValue_FLOAT4,
	kValue_PLUSINF,
	kValue_MINUSINF,
	kValue_STRING,
	kValue_QUOTEDSTRING,
	kValueTypeCount
};

struct GmlValue
{
	GclValueType type;
	union
	{
		bool bValue;
		int iValue;
		float fValue;
		float f2Value[2];
		float f3Value[3];
		float f4Value[4];
		char strValue[256];
	};
};

#endif
