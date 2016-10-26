%{
	#include "gamecode.h"
	#include "codelexer.h"
%}

%option reentrant
%option noyywrap
%option yylineno

%option bison-bridge

%x SINGLE_QUOTED
%x DOUBLE_QUOTED
%x SINGLELINE_COMMENT
%x MULTILINE_COMMENT
%x EXPRESSION

%{
#define YY_USER_ACTION yyextra->first_line = yylineno; yyextra->last_line = yylineno; \
    yyextra->first_column = yycolumn; yyextra->last_column = yycolumn+yyleng-1; \
	yycolumn += (int)yyget_leng(yyscanner);
%}

%%

"{" {return GCL_LBRACKET; }
"}" {return GCL_RBRACKET; }
"=" {return GCL_EQUAL; }
"+=" {return GCL_PLUSEQUAL; }

"+inf" { return GCL_PLUSINF; }
"-inf" { return GCL_MINUSINF; }

[ \t,]	{}
\r\n	{ yyextra->col = 0; ++yyextra->line; }
\n		{ yyextra->col = 0; ++yyextra->line; }

"//"						{ BEGIN(SINGLELINE_COMMENT); }
<SINGLELINE_COMMENT>\r\n	{ BEGIN(INITIAL);  yyextra->col = 0; ++yyextra->line; }
<SINGLELINE_COMMENT>\n		{ BEGIN(INITIAL); yyextra->col = 0; ++yyextra->line; }
<SINGLELINE_COMMENT>[^\r\n] {}

"/*"						{ BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/"		{ BEGIN(INITIAL); }
<MULTILINE_COMMENT>[^\r\n]	{}
<MULTILINE_COMMENT>\r\n		{}
<MULTILINE_COMMENT>\n		{}

[#]							{ BEGIN(EXPRESSION); yylval->strValue[0] = 0; }
<EXPRESSION>[^#\r\n]		{ strcat(yylval->strValue, yytext); }
<EXPRESSION>\r\n			{ yyextra->col = 0; ++yyextra->line; strcat(yylval->strValue, "\r\n"); }
<EXPRESSION>\n				{ yyextra->col = 0; ++yyextra->line; strcat(yylval->strValue, "\n");}
<EXPRESSION>[#]				{ BEGIN(INITIAL); return GCL_STRINGVALUE; }

[']							{ BEGIN(SINGLE_QUOTED); yylval->strValue[0] = 0; }
<SINGLE_QUOTED>[^'\r\n]		{ strcat(yylval->strValue, yytext); }
<SINGLE_QUOTED>\r\n			{ yyextra->col = 0; ++yyextra->line; strcat(yylval->strValue, "\r\n"); }
<SINGLE_QUOTED>\n			{ yyextra->col = 0; ++yyextra->line; strcat(yylval->strValue, "\n");}
<SINGLE_QUOTED>[']			{ BEGIN(INITIAL); return GCL_STRINGVALUE; }

["]							{ BEGIN(DOUBLE_QUOTED); yylval->strValue[0] = 0; }
<DOUBLE_QUOTED>[^"\r\n]		{ strcat(yylval->strValue, yytext); }
<DOUBLE_QUOTED>\r\n			{ yyextra->col = 0; ++yyextra->line; strcat(yylval->strValue, "\r\n"); }
<DOUBLE_QUOTED>\n			{ yyextra->col = 0; ++yyextra->line; strcat(yylval->strValue, "\n");}
<DOUBLE_QUOTED>["]			{ BEGIN(INITIAL); return GCL_STRINGVALUE; }

"true" { yylval->bValue=true; return GCL_BOOL; }
"false" { yylval->bValue=false; return GCL_BOOL; }

[-]?[0-9]+\.[0-9]+ { yylval->fValue = (float)atof(yytext); return GCL_FLOAT; }
[-]?[0-9]+ { yylval->iValue = atoi(yytext); return GCL_INT; }
[a-zA-Z0-9_]+ {
	const size_t size = sizeof(yylval->strValue);
	strncpy(yylval->strValue, yytext, size);
	yylval->strValue[size-1] = 0;
	GenObject *obj = getMetaObject(yyextra->meta,yylval->strValue);
	if (obj)
	{
		yylval->metaObject = obj;
		return GCL_OBJECTTYPE;
	}
	else
		return GCL_STRING;
	}

%%