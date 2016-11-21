#ifndef LEXER_H
#define LEXER_H

#ifndef YYSTYPE

typedef union {
	bool bValue;
	int64_t iValue;
	float fValue;
	char strValue[256];
} yystype;

# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1

#endif

#ifndef YY_EXTRA_TYPE
typedef struct {
	int line;
	int col;
} MetaYYExtraType;
# define YY_EXTRA_TYPE MetaYYExtraType*
# define YY_EXTRA_TYPE_IS_TRIVIAL 1
#endif

#endif //#ifndef LEXER_H
