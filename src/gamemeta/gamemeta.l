%{
    #include "gamemeta.h"
    #include "gamemetalexer.h"
%}
 
%option reentrant
%option noyywrap

%option bison-bridge
%option prefix="yymeta"
 
%x SINGLE_QUOTED
%x DOUBLE_QUOTED
%x SINGLELINE_COMMENT
%x MULTILINE_COMMENT
%x TAG
 
%%
 
"//"						{ BEGIN(SINGLELINE_COMMENT); }
<SINGLELINE_COMMENT>[^\r\n] {}
<SINGLELINE_COMMENT>\r\n	{ BEGIN(INITIAL);  yyextra->col = 0; ++yyextra->line; }
<SINGLELINE_COMMENT>\n		{ BEGIN(INITIAL); yyextra->col = 0; ++yyextra->line; }

"/*"						{ BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>[^\r\n]	{}
<MULTILINE_COMMENT>\r\n		{ yyextra->col = 0; ++yyextra->line; }
<MULTILINE_COMMENT>\n		{ yyextra->col = 0; ++yyextra->line; }
<MULTILINE_COMMENT>"*/"		{ BEGIN(INITIAL); }

"enum" {return GML_ENUM;}
"object" {return GML_OBJECT;}
":" {return GML_COLUMN;}

"{" {return GML_LBRACKET;}
"}" {return GML_RBRACKET;}
"=" {return GML_EQUAL;}


"nonzero" {return GML_NONZERO;}
"ref" {return GML_REF;}
"+inf" { return GML_PLUSINF;}
"-inf" { return GML_MINUSINF;}

"bool" { return GML_TYPEBOOL; }
"int8" { return GML_TYPEINT8; }
"uint8" { return GML_TYPEUINT8; }
"int16" { return GML_TYPEINT16; }
"uint16" { return GML_TYPEUINT16; }
"int64" { return GML_TYPEINT64; }
"uint64" { return GML_TYPEUINT64; }
"int" { return GML_TYPEINT; }
"uint" { return GML_TYPEUINT; }

"float2" { return GML_TYPEFLOAT2; }
"float3" { return GML_TYPEFLOAT3; }
"float4" { return GML_TYPEFLOAT4; }
"float" { return GML_TYPEFLOAT; }

"ufloat" { return GML_TYPEUFLOAT; }


"string" { return GML_TYPESTRING; }
"expression" { return GML_TYPEEXPRESSION; }

\r\n { yyextra->col=0; ++yyextra->line; }
\n { yyextra->col = 0; ++yyextra->line; }
[ \t,]    {}

"[]" { return GML_ARRAY; }

"true" { yylval->bValue=true; return GML_BOOL; }
"false" { yylval->bValue=false; return GML_BOOL; }

[-]?[0-9]+\.[0-9]+ { yylval->fValue=(float)atof(yytext); return GML_FLOAT; }
-[0-9]+  { yylval->iValue=_atoi64(yytext); return GML_INT; }
[0-9]+  { yylval->iValue=(int64_t)(_strtoui64(yytext,NULL,10)); return GML_INT; }
[a-zA-Z0-9_]+ {
		const size_t size = sizeof(yylval->strValue);
		strncpy(yylval->strValue,yytext,size);
		yylval->strValue[size-1] = 0;
		return GML_STRING;
	}
["][^"]*["] { return GML_STRINGVALUE; }

"<" {BEGIN(TAG); return GML_LTAG;}
<TAG>[^>,]+ {
		const size_t size = sizeof(yylval->strValue);
		strncpy(yylval->strValue,yytext,size);
		yylval->strValue[size-1] = 0;
		//printf("tag = %s\n", yylval->strValue);
		return GML_TAG;
	}
<TAG>[,]+ {}
<TAG>">" {BEGIN(INITIAL); return GML_RTAG;}

%%