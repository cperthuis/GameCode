#ifndef LEXER_H
#define LEXER_H

struct GenObject;
struct Meta;

#ifndef YYSTYPE

struct yystype {
	bool bValue;
	int iValue;
	float fValue;
	char strValue[256];
	GenObject* metaObject;
};

# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1

#endif

#ifndef YY_EXTRA_TYPE

struct CodeYYExtraType {
	int line;
	int col;
	size_t first_column;
	size_t last_column;
	size_t first_line;
	size_t last_line;
	const Meta *meta;
};

GenObject* getMetaObject(const Meta *meta, const char *name); //defined in meta.c

# define YY_EXTRA_TYPE CodeYYExtraType*
# define YY_EXTRA_TYPE_IS_TRIVIAL 1
#endif

#define ECHO do { printf( "%s",yytext); } while (0)

#endif //#ifndef LEXER_H
