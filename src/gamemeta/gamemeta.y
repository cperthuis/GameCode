%include
{
    #include <cassert>
    #include <iostream>

	#include "metagrammar.h"
}
 
%token_prefix   GML_
%token_type {GmlParserToken}
%extra_argument {GmlParserExtra* extra}

%syntax_error {
	printf("file(%i): syntax error\n",extra->line);
    extra->valid = false;
}

%parse_accept {
    //printf("parsing complete!\n");
}

%type		itemList	{GmlItemList*}
%destructor	itemList	{gmlFreeItemList($$);}

%type		enum	{GmlEnum*}
%destructor	enum	{gmlFreeEnum($$);}

%type		object	{GmlObject*}
%destructor	object	{gmlFreeObject($$);}

start ::= itemList(L).
{
	//reverse the list which was built backward due to the recommended stack optimization
	GmlItemList *l = L;
	while(l->parent)
	{
		if (l->parent->objectItem || l->parent->enumItem)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gmlFreeItemList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	extra->root = l;
}

itemList(L) ::= itemList(L0) object(O).
{
	L = gmlItemList();
	L->objectItem = O;
	L->enumItem = 0;
	L->next = 0;
	L->parent = L0;
	L0->next = L;
}

itemList(L) ::= itemList(L0) enum(E).
{
	L = gmlItemList();
	L->objectItem = 0;
	L->enumItem = E;
	L->next = 0;
	L->parent = L0;
	L0->next = L;
}

itemList(L) ::= .
{
	L = gmlItemList();
	L->objectItem = 0;
	L->enumItem = 0;
	L->next = 0;
}

%type		enumTag		{GmlString*}
%destructor	enumTag		{gmlFreeString($$);}

%type		enumName	{GmlString*}
%destructor	enumName	{gmlFreeString($$);}

%type		enumMember	{GmlEnumMember*}
%destructor	enumMember	{gmlFreeEnumMember($$);}

%type		enumList	{GmlEnumMemberList*}
%destructor	enumList	{gmlFreeEnumMemberList($$);}

enum(E) ::= ENUM enumName(N) LBRACKET enumList(L) RBRACKET .
{
	#ifdef TRACE_PARSING
	printf("enumeration name= %s\n",N->value);
	#endif
	E = gmlEnum();
	E->name = N;
	
	//reverse the list which was built backward due to the recommended stack optimization
	GmlEnumMemberList *l = L;
	while(l->parent)
	{
		if (l->parent->member)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gmlFreeEnumMemberList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	E->memberList = l;

}
enumName(N) ::= STRING(X).
{
	#ifdef TRACE_PARSING
	printf("enum name= %s\n",X.strValue);
	#endif
	N = gmlString(X.strValue);
}
enumList(L) ::= enumList(L0) enumMember(M).
{
	L = gmlEnumMemberList();
	L->member = M;
	L->parent = L0;
	L->next = 0;
	L0->next = L;
}
enumList(L) ::= enumMember(M) .
{
	L = gmlEnumMemberList();
	L->member = M;
	L->parent = 0;
	L->next = 0;
}
enumMember(M) ::= enumTag(T) .
{
	M = gmlEnumMember();
	M->name = T;
	M->value = -1;
}
enumMember(M) ::= enumTag(T) EQUAL INT(X) .
{
	M = gmlEnumMember();
	M->name = T;
	M->value = X.iValue;
	#ifdef TRACE_PARSING
	printf("enum tag= %s value=%i\n",M->name->value,M->value);
	#endif
}
enumTag(T) ::= STRING(X) .
{
	T = gmlString(X.strValue);
}

%type		objectName	{GmlString*}
%destructor	objectName	{gmlFreeString($$);}

%type		parentName	{GmlString*}
%destructor	parentName	{gmlFreeString($$);}

%type		memberList	{GmlObjectMemberList*}
%destructor	memberList	{gmlFreeObjectMemberList($$);}

%type		member	{GmlObjectMember*}
%destructor	member	{gmlFreeObjectMember($$);}

%type		memberName	{GmlString*}
%destructor	memberName	{gmlFreeString($$);}

%type		tagList		{GmlString*}
%destructor	tagList		{gmlFreeString($$);}

%type		tags		{GmlString*}
%destructor	tags		{gmlFreeString($$);}

%type		tag	{GmlString*}
%destructor	tag	{gmlFreeString($$);}

%type		value	{GmlValue*}
%destructor	value	{gmlFreeValue($$);}

%type		value2f	{GmlValue*}
%destructor	value2f	{gmlFreeValue($$);}

%type		value3f	{GmlValue*}
%destructor	value3f	{gmlFreeValue($$);}

%type		value3i	{GmlValue*}
%destructor	value3i	{gmlFreeValue($$);}

%type		type	{GmlType*}
%destructor	type	{gmlFreeType($$);}

%type		memberType	{GmlMemberType*}
%destructor	memberType	{gmlFreeMemberType($$);}

%type		typeModifierList	{GmlTypeModifierList*}
%destructor	typeModifierList	{gmlFreeTypeModifierList($$);}

%type		typeModifier	{GmlTypeModifier*}
%destructor	typeModifier	{gmlFreeTypeModifier($$);}

object(O) ::= OBJECT objectName(N) tags LBRACKET memberList(L) RBRACKET .
{
	O = gmlObject();
	O->name = N;
	O->parentName = NULL;
	
	//reverse the list which was built backward due to the recommended stack optimization
	GmlObjectMemberList *l = L;
	while(l->parent)
	{
		if (l->parent->member)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gmlFreeObjectMemberList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	O->memberList = l;
}
object(O) ::= OBJECT objectName(N) COLUMN parentName(P) tags LBRACKET memberList(L) RBRACKET .
{
	O = gmlObject();
	O->name = N;
	O->parentName = P;

	//reverse the list which was built backward due to the recommended stack optimization
	GmlObjectMemberList *l = L;
	while(l->parent)
	{
		if (l->parent->member)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gmlFreeObjectMemberList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	O->memberList = l;
}
object(O) ::= OBJECT objectName(N) .
{
	O = gmlObject();
	O->name = N;
	O->parentName = NULL;
	O->memberList = NULL;
}
objectName(N) ::= STRING(X) .
{
	#ifdef TRACE_PARSING
	printf("object name= %s\n",X.strValue);
	#endif
	N = gmlString(X.strValue);
}
parentName(P) ::= STRING(X) .
{
	#ifdef TRACE_PARSING
	printf("parent name= %s\n",X.strValue);
	#endif
	P = gmlString(X.strValue);
}

memberList(L) ::= memberList(L0) member(M) .
{
	L = gmlObjectMemberList();
	L->member = M;
	L->parent = L0;
	L->next = 0;
	L0->next = L;
}
memberList(L) ::= .
{
	L = gmlObjectMemberList();
	L->member = 0;
	L->parent = 0;
	L->next = 0;
}

member(M) ::= type(T) tags memberName(N) EQUAL value(V) .
{
	M = gmlObjectMember();
	M->isArray = false;
	M->name = N;
	M->type = T;
	M->defaultValue = V;
}
member(M) ::= type(T) tags memberName(N) ARRAY .
{
	M = gmlObjectMember();
	M->isArray = true;
	M->name = N;
	M->type = T;
	M->defaultValue = NULL;
}
member(M) ::= type(T) tags memberName(N) .
{
	M = gmlObjectMember();
	M->isArray = false;
	M->name = N;
	M->type = T;
	M->defaultValue = NULL;
}
memberName(N) ::= STRING(X) .
{
	N = gmlString(X.strValue);
}


tags(G) ::= LTAG tagList(L) RTAG .
{
	G = L;
}

tags(G) ::= .
{
	G = 0;
}

tagList(L) ::= tagList tag .
{
	L = 0;
}
tagList(L) ::= .
{
	L = 0;
}

tag(T)::= BOOL .
{
	T = 0;
}

tag(T)::= INT .
{
	T = 0;
}
tag(T)::= FLOAT .
{
	T = 0;
}
tag(T)::= STRINGVALUE .
{
	T = 0;
}
tag(T)::= STRING .
{
	T = 0;
}
tag(T)::= TAG .
{
	T = 0;
}

value(V)::= BOOL(X) .
{
	V = gmlValue(X.bValue);
}

value(V)::= INT(X) .
{
	V = gmlValue(X.iValue);
}

value(V)::= FLOAT(X) value3f(V1) .
{
	V = gmlValue(X.fValue,V1->f3Value[0],V1->f3Value[1],V1->f3Value[2]);
}

value(V)::= INT(X) value3f(V1) .
{
	V = gmlValue((float)X.iValue,V1->f3Value[0],V1->f3Value[1],V1->f3Value[2]);
}

value3f(V)::= FLOAT(X) value2f(V1) .
{
	V = gmlValue(X.fValue,V1->f2Value[0],V1->f2Value[1]);
}

value3f(V)::= INT(X) value2f(V1) .
{
	V = gmlValue((float)X.iValue,V1->f2Value[0],V1->f2Value[1]);
}

value2f(V)::= FLOAT(X) FLOAT(Y) .
{
	V = gmlValue(X.fValue,Y.fValue);
}

value2f(V)::= INT(X) FLOAT(Y) .
{
	V = gmlValue((float)X.iValue,Y.fValue);
}

value2f(V)::= FLOAT(X) INT(Y) .
{
	V = gmlValue(X.fValue,(float)Y.iValue);
}

value2f(V)::= INT(X) INT(Y) .
{
	V = gmlValue((float)X.iValue,(float)Y.iValue);
}

value(V)::= value3f(V1) .
{
	V = V1;
}

value(V)::= value2f(V1) .
{
	V = V1;
}

value(V)::= FLOAT(X) .
{
	V = gmlValue(X.fValue);
}
value(V)::= PLUSINF .
{
	V = gmlValue(kValue_PLUSINF);
}
value(V)::= MINUSINF .
{
	V = gmlValue(kValue_MINUSINF);
}
value(V)::= STRINGVALUE(X) .
{
	V = gmlValue(kValue_QUOTEDSTRING,X.strValue);
}
value(V)::= STRING(X).
{
	V = gmlValue(kValue_STRING,X.strValue);
}

type(T)::= typeModifierList(L) memberType(M) .
{
	T = gmlType();
	T->memberType = M;

	//reverse the list which was built backward due to the recommended stack optimization
	GmlTypeModifierList *l = L;
	while(l->parent)
	{
		if (l->parent->modifier)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gmlFreeTypeModifierList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	T->modifierList = l;
}
memberType(M)::= TYPEBOOL .
{
	M = gmlMemberType();
	M->type = kMemberType_BOOL;
}
memberType(M)::= TYPEUINT .
{
	M = gmlMemberType();
	M->type = kMemberType_UINT;
}
memberType(M)::= TYPEINT .
{
	M = gmlMemberType();
	M->type = kMemberType_INT;
}
memberType(M)::= TYPEUFLOAT .
{
	M = gmlMemberType();
	M->type = kMemberType_UFLOAT;
}
memberType(M)::= TYPEFLOAT .
{
	M = gmlMemberType();
	M->type = kMemberType_FLOAT;
}
memberType(M)::= TYPEFLOAT2 .
{
	M = gmlMemberType();
	M->type = kMemberType_FLOAT2;
}
memberType(M)::= TYPEFLOAT3 .
{
	M = gmlMemberType();
	M->type = kMemberType_FLOAT3;
}
memberType(M)::= TYPEFLOAT4 .
{
	M = gmlMemberType();
	M->type = kMemberType_FLOAT4;
}
memberType(M)::= TYPESTRING .
{
	M = gmlMemberType();
	M->type = kMemberType_STRING;
}
memberType(M)::= TYPEEXPRESSION .
{
	M = gmlMemberType();
	M->type = kMemberType_EXPRESSION;
}

memberType(M)::= STRING(X) .
{
	M = gmlMemberType();
	M->type = kMemberType_OBJECT;
	M->objectName = gmlString(X.strValue);
}

typeModifierList(L) ::= typeModifierList(L0) typeModifier(M) .
{
	L = gmlTypeModifierList();
	L->modifier = M;
	L->parent = L0;
	L->next = 0;
	L0->next = L;
}
typeModifierList(L) ::=  .
{
	L = gmlTypeModifierList();
	L->modifier = 0;
	L->parent = 0;
	L->next = 0;
}
typeModifier(M) ::= NONZERO.
{
	M = gmlTypeModifer();
	M->type = kTypeModifier_NONZERO;
}
typeModifier(M) ::= REF.
{
	M = gmlTypeModifer();
	M->type = kTypeModifier_REF;
}
