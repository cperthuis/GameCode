%include
{
    #include <cassert>
    #include <iostream>
	#include "codegrammar.h"
	//#define TRACE_PARSING
}
 
%token_prefix   GCL_
%token_type {GclParserToken}
%extra_argument {GclParserExtra* extra}

%syntax_error {
	printf("%s(%i,%i): syntax error\n", extra->filename, (int)extra->first_line, (int)extra->first_column);
    extra->valid = false;
}

%parse_accept {
    //printf("parsing complete!\n");
}

%type		objectList	{GclObjectList*}
%destructor objectList	{gclFreeObjectList($$);}

%type		object	{GclObject*}
%destructor	object	{gclFreeObject($$);}

%type		untypedobject{ GclObject* }
%destructor	untypedobject{ gclFreeObject($$); }

%type		itemList{ GclItemList* }
%destructor	itemList{ gclFreeItemList($$); }

%type		member{ GclMember* }
%destructor	member{ gclFreeMember($$); }

%type		value{ GclValue* }
%destructor	value{ gclFreeValue($$); }

%type		value2f{ GclValue* }
%destructor	value2f{ gclFreeValue($$); }

%type		value3f{ GclValue* }
%destructor	value3f{ gclFreeValue($$); }

start ::= objectList(L).
{
	//reverse the list which was built backward due to the recommended stack optimization
	GclObjectList *l = L;
	while(l->parent)
	{
		if (l->parent->object)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gclFreeObjectList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	extra->root = l;
}
objectList(L) ::= objectList(L0) object(O) .
{
	L = gclObjectList();
	L->object = O;
	L->next = 0;
	L->parent = L0;
	L0->next = L;
}

objectList(L) ::= .
{
	L = gclObjectList();
	L->object = 0;
	L->next = 0;
	L->parent = 0;
}

object(O) ::= OBJECTTYPE(T) STRING(N) LBRACKET itemList(L) RBRACKET .
{
	#ifdef TRACE_PARSING
	printf("object name= %s\n",N.strValue);
	#endif
	O = gclObject();
	O->name = gclString(N.strValue);
	O->type = gclString(T.strValue);
	O->typeObject = T.metaObject;
	O->compiledObjectIndex = -1;
	O->line = extra->first_line;
	O->col = extra->first_column;
	
	//reverse the list which was built backward due to the recommended stack optimization
	GclItemList *l = L;
	while(l->parent)
	{
		if (l->parent->member || l->parent->object)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gclFreeItemList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	O->itemList = l;
}

object(O) ::= OBJECTTYPE(T) LBRACKET itemList(L) RBRACKET .
{
#ifdef TRACE_PARSING
	printf("object name= %s\n", "unnamed object");
#endif
	O = gclObject();
	O->name = NULL;
	O->type = gclString(T.strValue);
	O->typeObject = T.metaObject;
	O->compiledObjectIndex = -1;
	O->line = extra->first_line;
	O->col = extra->first_column;

	//reverse the list which was built backward due to the recommended stack optimization
	GclItemList *l = L;
	while (l->parent)
	{
		if (l->parent->member || l->parent->object)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gclFreeItemList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	O->itemList = l;
}

untypedobject(O) ::= LBRACKET itemList(L) RBRACKET .
{
#ifdef TRACE_PARSING
	printf("object name= %s\n", "unnamed object");
#endif
	O = gclObject();
	O->name = NULL;
	O->type = NULL;
	O->typeObject = NULL;
	O->compiledObjectIndex = -1;
	O->line = extra->first_line;
	O->col = extra->first_column;

	//reverse the list which was built backward due to the recommended stack optimization
	GclItemList *l = L;
	while (l->parent)
	{
		if (l->parent->member || l->parent->object)
		{
			l = l->parent;
		}
		else
		{
			//remove the first item of the list which is empty
			l->parent->next = NULL;
			gclFreeItemList(l->parent);
			l->parent = NULL;
			break;
		}
	}
	O->itemList = l;
}

itemList(L) ::= itemList(L0) member(M).
{
	L = gclItemList();
	L->object = 0;
	L->member = M;
	L->parent = L0;
	L->next = 0;
	L0->next = L;
}

itemList(L) ::= itemList(L0) object(O).
{
	L = gclItemList();
	L->object = O;
	L->member = 0;
	L->parent = L0;
	L->next = 0;
	L0->next = L;
}

itemList(L) ::= .
{
	L = gclItemList();
	L->object = 0;
	L->member = 0;
	L->parent = 0;
	L->next = 0;
}

member(M) ::= STRING(N) EQUAL value(V) .
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", N.strValue);
#endif
	M = gclMember();
	M->name = gclString(N.strValue);
	M->value = V;
	M->object = 0;
	M->line = extra->first_line;
	M->col = extra->first_column;
}

member(M) ::= STRING(N) PLUSEQUAL value(V) .
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", N.strValue);
#endif

	//TODO
	M = gclMember();
	M->name = gclString(N.strValue);
	M->value = V;
	M->object = 0;
	M->line = extra->first_line;
	M->col = extra->first_column;
}

member(M) ::= STRING(N) EQUAL object(O) .
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", N.strValue);
#endif

	M = gclMember();
	M->name = gclString(N.strValue);
	M->value = 0;
	M->object = O;
	M->line = extra->first_line;
	M->col = extra->first_column;
}

member(M) ::= STRING(N) PLUSEQUAL object(O) .
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", N.strValue);
#endif

	//TODO
	M = gclMember();
	M->name = gclString(N.strValue);
	M->value = 0;
	M->object = O;
	M->line = extra->first_line;
	M->col = extra->first_column;
}

member(M) ::= STRING(N) EQUAL untypedobject(O) .
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", N.strValue);
#endif

	M = gclMember();
	M->name = gclString(N.strValue);
	M->value = 0;
	M->object = O;
	M->line = extra->first_line;
	M->col = extra->first_column;
}

member(M) ::= STRING(N) PLUSEQUAL untypedobject(O) .
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", N.strValue);
#endif

	//TODO
	M = gclMember();
	M->name = gclString(N.strValue);
	M->value = 0;
	M->object = O;
	M->line = extra->first_line;
	M->col = extra->first_column;
	//printf("%i,%i(%i): %s\n",extra->first_line,extra->first_column,extra->line,M->name);
}

value(V)::= BOOL(X) .
{
	V = gclValue((int64_t)X.bValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}
value(V)::= INT(X) .
{
	V = gclValue(X.iValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value(V)::= FLOAT(X) value3f(V1) .
{
	V = gclValue(X.fValue,V1->f3Value[0],V1->f3Value[1],V1->f3Value[2]);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value(V)::= INT(X) value3f(V1) .
{
	V = gclValue((float)X.iValue,V1->f3Value[0],V1->f3Value[1],V1->f3Value[2]);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value3f(V)::= FLOAT(X) value2f(V1) .
{
	V = gclValue(X.fValue,V1->f2Value[0],V1->f2Value[1]);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value3f(V)::= INT(X) value2f(V1) .
{
	V = gclValue((float)X.iValue,V1->f2Value[0],V1->f2Value[1]);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value2f(V)::= FLOAT(X) FLOAT(Y) .
{
	V = gclValue(X.fValue,Y.fValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value2f(V)::= INT(X) FLOAT(Y) .
{
	V = gclValue((float)X.iValue,Y.fValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value2f(V)::= FLOAT(X) INT(Y) .
{
	V = gclValue(X.fValue,(float)Y.iValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value2f(V)::= INT(X) INT(Y) .
{
	V = gclValue((float)X.iValue,(float)Y.iValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value(V)::= value3f(V1) .
{
	V = V1;
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value(V)::= value2f(V1) .
{
	V = V1;
	V->line = extra->first_line;
	V->col = extra->first_column;
}

value(V)::= FLOAT(X) .
{
	V = gclValue(X.fValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}
value(V)::= PLUSINF .
{
	V = gclValue(kValue_PLUSINF);
	V->line = extra->first_line;
	V->col = extra->first_column;
}
value(V)::= MINUSINF .
{
	V = gclValue(kValue_MINUSINF);
	V->line = extra->first_line;
	V->col = extra->first_column;
}
value(V)::= STRINGVALUE(X) .
{
	V = gclValue(kValue_QUOTEDSTRING, X.strValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}
value(V)::= STRING(X).
{
	V = gclValue(kValue_STRING, X.strValue);
	V->line = extra->first_line;
	V->col = extra->first_column;
}
