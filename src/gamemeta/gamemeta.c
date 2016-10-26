/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 2 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"

    #include <cassert>
    #include <iostream>

	#include "metagrammar.h"
#line 14 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParseTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 56
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE GmlParserToken
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  GmlTypeModifierList* yy2;
  GmlType* yy5;
  GmlEnum* yy22;
  GmlObjectMember* yy26;
  GmlItemList* yy34;
  GmlObjectMemberList* yy40;
  GmlValue* yy42;
  GmlString* yy66;
  GmlEnumMemberList* yy72;
  GmlObject* yy90;
  GmlTypeModifier* yy98;
  GmlEnumMember* yy106;
  GmlMemberType* yy107;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL GmlParserExtra* extra;
#define ParseARG_PDECL ,GmlParserExtra* extra
#define ParseARG_FETCH GmlParserExtra* extra = yypParser->extra
#define ParseARG_STORE yypParser->extra = extra
#define YYNSTATE             35
#define YYNRULE              65
#define YY_MAX_SHIFT         34
#define YY_MIN_SHIFTREDUCE   89
#define YY_MAX_SHIFTREDUCE   153
#define YY_MIN_REDUCE        154
#define YY_MAX_REDUCE        218
#define YY_ERROR_ACTION      219
#define YY_ACCEPT_ACTION     220
#define YY_NO_ACTION         221

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (96)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   149,  119,  101,  116,  107,  132,  131,  100,  111,  115,
 /*    10 */   117,  118,  120,  113,  139,  140,  141,  142,  143,  144,
 /*    20 */   145,  146,  147,  148,  152,  153,  137,   33,    8,   96,
 /*    30 */     9,  105,  125,  123,  121,    7,  136,   99,  134,  135,
 /*    40 */    19,  177,    5,  154,   21,   91,   90,   17,    6,   23,
 /*    50 */    20,  220,  138,   12,  150,   14,   25,  126,  124,   33,
 /*    60 */    13,   95,   15,   26,   93,   99,   27,  126,  130,  129,
 /*    70 */     1,   31,   30,   28,  108,  128,  127,  125,   23,  104,
 /*    80 */    16,  110,   18,  103,   29,   11,   94,   34,   10,    2,
 /*    90 */    22,   24,   98,    3,    4,   32,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     4,    4,    3,    6,   47,   48,   49,    3,   11,   12,
 /*    10 */    13,   14,   15,   46,   18,   19,   20,   21,   22,   23,
 /*    20 */    24,   25,   26,   27,   28,   29,    4,   35,    6,   37,
 /*    30 */    38,   42,   48,   49,   12,   13,   14,    4,   16,   17,
 /*    40 */    51,    2,   53,    0,    1,   32,   33,    8,   31,   10,
 /*    50 */     7,   34,   52,    6,   54,    6,    6,   48,   49,   35,
 /*    60 */    13,   37,   13,   13,    3,    4,    6,   48,    6,    6,
 /*    70 */     5,   45,   45,   13,    9,   13,   13,   48,   10,    4,
 /*    80 */    40,    4,   45,    4,   43,   39,    4,   36,   44,   41,
 /*    90 */     2,    2,    6,   41,    2,    5,
};
#define YY_SHIFT_USE_DFLT (-5)
#define YY_SHIFT_COUNT (34)
#define YY_SHIFT_MIN   (-4)
#define YY_SHIFT_MAX   (92)
static const signed char yy_shift_ofst[] = {
 /*     0 */    -5,   22,   -1,    4,   33,   -4,   43,   47,   49,   61,
 /*    10 */    -3,   39,   50,   60,   50,   60,   68,   75,   77,   68,
 /*    20 */    79,   82,   -5,   -5,   -5,   62,   63,   62,   63,   65,
 /*    30 */    88,   89,   86,   90,   92,
};
#define YY_REDUCE_USE_DFLT (-44)
#define YY_REDUCE_COUNT (24)
#define YY_REDUCE_MIN   (-43)
#define YY_REDUCE_MAX   (52)
static const signed char yy_reduce_ofst[] = {
 /*     0 */    17,  -43,  -11,  -11,   -8,    0,   13,  -16,    9,   24,
 /*    10 */   -33,   26,   19,   29,   19,   29,   27,   40,   41,   37,
 /*    20 */    46,   51,   48,   44,   52,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   157,  219,  216,  216,  219,  219,  219,  198,  187,  219,
 /*    10 */   219,  167,  194,  192,  195,  193,  177,  219,  219,  177,
 /*    20 */   219,  219,  171,  179,  171,  195,  193,  194,  192,  174,
 /*    30 */   219,  219,  219,  162,  219,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "ENUM",          "LBRACKET",      "RBRACKET",    
  "STRING",        "EQUAL",         "INT",           "OBJECT",      
  "COLUMN",        "ARRAY",         "LTAG",          "RTAG",        
  "BOOL",          "FLOAT",         "STRINGVALUE",   "TAG",         
  "PLUSINF",       "MINUSINF",      "TYPEBOOL",      "TYPEUINT",    
  "TYPEINT",       "TYPEUFLOAT",    "TYPEFLOAT",     "TYPEFLOAT2",  
  "TYPEFLOAT3",    "TYPEFLOAT4",    "TYPESTRING",    "TYPEEXPRESSION",
  "NONZERO",       "REF",           "error",         "itemList",    
  "enum",          "object",        "start",         "enumTag",     
  "enumName",      "enumMember",    "enumList",      "objectName",  
  "parentName",    "memberList",    "member",        "memberName",  
  "tagList",       "tags",          "tag",           "value",       
  "value2f",       "value3f",       "value3i",       "type",        
  "memberType",    "typeModifierList",  "typeModifier",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "start ::= itemList",
 /*   1 */ "itemList ::= itemList object",
 /*   2 */ "itemList ::= itemList enum",
 /*   3 */ "itemList ::=",
 /*   4 */ "enum ::= ENUM enumName LBRACKET enumList RBRACKET",
 /*   5 */ "enumName ::= STRING",
 /*   6 */ "enumList ::= enumList enumMember",
 /*   7 */ "enumList ::= enumMember",
 /*   8 */ "enumMember ::= enumTag",
 /*   9 */ "enumMember ::= enumTag EQUAL INT",
 /*  10 */ "enumTag ::= STRING",
 /*  11 */ "object ::= OBJECT objectName tags LBRACKET memberList RBRACKET",
 /*  12 */ "object ::= OBJECT objectName COLUMN parentName tags LBRACKET memberList RBRACKET",
 /*  13 */ "object ::= OBJECT objectName",
 /*  14 */ "objectName ::= STRING",
 /*  15 */ "parentName ::= STRING",
 /*  16 */ "memberList ::= memberList member",
 /*  17 */ "memberList ::=",
 /*  18 */ "member ::= type tags memberName EQUAL value",
 /*  19 */ "member ::= type tags memberName ARRAY",
 /*  20 */ "member ::= type tags memberName",
 /*  21 */ "memberName ::= STRING",
 /*  22 */ "tags ::= LTAG tagList RTAG",
 /*  23 */ "tags ::=",
 /*  24 */ "tagList ::= tagList tag",
 /*  25 */ "tagList ::=",
 /*  26 */ "tag ::= BOOL",
 /*  27 */ "tag ::= INT",
 /*  28 */ "tag ::= FLOAT",
 /*  29 */ "tag ::= STRINGVALUE",
 /*  30 */ "tag ::= STRING",
 /*  31 */ "tag ::= TAG",
 /*  32 */ "value ::= BOOL",
 /*  33 */ "value ::= INT",
 /*  34 */ "value ::= FLOAT value3f",
 /*  35 */ "value ::= INT value3f",
 /*  36 */ "value3f ::= FLOAT value2f",
 /*  37 */ "value3f ::= INT value2f",
 /*  38 */ "value2f ::= FLOAT FLOAT",
 /*  39 */ "value2f ::= INT FLOAT",
 /*  40 */ "value2f ::= FLOAT INT",
 /*  41 */ "value2f ::= INT INT",
 /*  42 */ "value ::= value3f",
 /*  43 */ "value ::= value2f",
 /*  44 */ "value ::= FLOAT",
 /*  45 */ "value ::= PLUSINF",
 /*  46 */ "value ::= MINUSINF",
 /*  47 */ "value ::= STRINGVALUE",
 /*  48 */ "value ::= STRING",
 /*  49 */ "type ::= typeModifierList memberType",
 /*  50 */ "memberType ::= TYPEBOOL",
 /*  51 */ "memberType ::= TYPEUINT",
 /*  52 */ "memberType ::= TYPEINT",
 /*  53 */ "memberType ::= TYPEUFLOAT",
 /*  54 */ "memberType ::= TYPEFLOAT",
 /*  55 */ "memberType ::= TYPEFLOAT2",
 /*  56 */ "memberType ::= TYPEFLOAT3",
 /*  57 */ "memberType ::= TYPEFLOAT4",
 /*  58 */ "memberType ::= TYPESTRING",
 /*  59 */ "memberType ::= TYPEEXPRESSION",
 /*  60 */ "memberType ::= STRING",
 /*  61 */ "typeModifierList ::= typeModifierList typeModifier",
 /*  62 */ "typeModifierList ::=",
 /*  63 */ "typeModifier ::= NONZERO",
 /*  64 */ "typeModifier ::= REF",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    case 31: /* itemList */
{
#line 23 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeItemList((yypminor->yy34));
#line 491 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 32: /* enum */
{
#line 26 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeEnum((yypminor->yy22));
#line 498 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 33: /* object */
{
#line 29 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeObject((yypminor->yy90));
#line 505 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 35: /* enumTag */
    case 36: /* enumName */
    case 39: /* objectName */
    case 40: /* parentName */
    case 43: /* memberName */
    case 44: /* tagList */
    case 45: /* tags */
    case 46: /* tag */
{
#line 82 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeString((yypminor->yy66));
#line 519 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 37: /* enumMember */
{
#line 88 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeEnumMember((yypminor->yy106));
#line 526 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 38: /* enumList */
{
#line 91 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeEnumMemberList((yypminor->yy72));
#line 533 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 41: /* memberList */
{
#line 170 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeObjectMemberList((yypminor->yy40));
#line 540 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 42: /* member */
{
#line 173 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeObjectMember((yypminor->yy26));
#line 547 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 47: /* value */
    case 48: /* value2f */
    case 49: /* value3f */
    case 50: /* value3i */
{
#line 188 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeValue((yypminor->yy42));
#line 557 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 51: /* type */
{
#line 200 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeType((yypminor->yy5));
#line 564 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 52: /* memberType */
{
#line 203 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeMemberType((yypminor->yy107));
#line 571 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 53: /* typeModifierList */
{
#line 206 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeTypeModifierList((yypminor->yy2));
#line 578 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    case 54: /* typeModifier */
{
#line 209 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
gmlFreeTypeModifier((yypminor->yy98));
#line 585 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParseAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;

  if( stateno>=YY_MIN_REDUCE ) return stateno; 
  assert( stateno <= YY_SHIFT_COUNT );
  i = yy_shift_ofst[stateno];
  if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    int i;
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
      fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
      for(i=1; i<=yypParser->yyidx; i++)
        fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
      fprintf(yyTraceFILE,"\n");
    }else{
      fprintf(yyTraceFILE,"%sShift *\n",yyTracePrompt);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.  Return the number of errors.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 34, 1 },
  { 31, 2 },
  { 31, 2 },
  { 31, 0 },
  { 32, 5 },
  { 36, 1 },
  { 38, 2 },
  { 38, 1 },
  { 37, 1 },
  { 37, 3 },
  { 35, 1 },
  { 33, 6 },
  { 33, 8 },
  { 33, 2 },
  { 39, 1 },
  { 40, 1 },
  { 41, 2 },
  { 41, 0 },
  { 42, 5 },
  { 42, 4 },
  { 42, 3 },
  { 43, 1 },
  { 45, 3 },
  { 45, 0 },
  { 44, 2 },
  { 44, 0 },
  { 46, 1 },
  { 46, 1 },
  { 46, 1 },
  { 46, 1 },
  { 46, 1 },
  { 46, 1 },
  { 47, 1 },
  { 47, 1 },
  { 47, 2 },
  { 47, 2 },
  { 49, 2 },
  { 49, 2 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 47, 1 },
  { 47, 1 },
  { 47, 1 },
  { 47, 1 },
  { 47, 1 },
  { 47, 1 },
  { 47, 1 },
  { 51, 2 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 53, 2 },
  { 53, 0 },
  { 54, 1 },
  { 54, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s] -> state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0: /* start ::= itemList */
#line 32 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	//reverse the list which was built backward due to the recommended stack optimization
	GmlItemList *l = yymsp[0].minor.yy34;
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
#line 978 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 1: /* itemList ::= itemList object */
#line 54 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy34 = gmlItemList();
	yygotominor.yy34->objectItem = yymsp[0].minor.yy90;
	yygotominor.yy34->enumItem = 0;
	yygotominor.yy34->next = 0;
	yygotominor.yy34->parent = yymsp[-1].minor.yy34;
	yymsp[-1].minor.yy34->next = yygotominor.yy34;
}
#line 990 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 2: /* itemList ::= itemList enum */
#line 64 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy34 = gmlItemList();
	yygotominor.yy34->objectItem = 0;
	yygotominor.yy34->enumItem = yymsp[0].minor.yy22;
	yygotominor.yy34->next = 0;
	yygotominor.yy34->parent = yymsp[-1].minor.yy34;
	yymsp[-1].minor.yy34->next = yygotominor.yy34;
}
#line 1002 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 3: /* itemList ::= */
#line 74 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy34 = gmlItemList();
	yygotominor.yy34->objectItem = 0;
	yygotominor.yy34->enumItem = 0;
	yygotominor.yy34->next = 0;
}
#line 1012 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 4: /* enum ::= ENUM enumName LBRACKET enumList RBRACKET */
#line 94 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	#ifdef TRACE_PARSING
	printf("enumeration name= %s\n",yymsp[-3].minor.yy66->value);
	#endif
	yygotominor.yy22 = gmlEnum();
	yygotominor.yy22->name = yymsp[-3].minor.yy66;
	
	//reverse the list which was built backward due to the recommended stack optimization
	GmlEnumMemberList *l = yymsp[-1].minor.yy72;
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
	yygotominor.yy22->memberList = l;

}
#line 1043 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 5: /* enumName ::= STRING */
#line 122 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	#ifdef TRACE_PARSING
	printf("enum name= %s\n",yymsp[0].minor.yy0.strValue);
	#endif
	yygotominor.yy66 = gmlString(yymsp[0].minor.yy0.strValue);
}
#line 1053 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 6: /* enumList ::= enumList enumMember */
#line 129 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy72 = gmlEnumMemberList();
	yygotominor.yy72->member = yymsp[0].minor.yy106;
	yygotominor.yy72->parent = yymsp[-1].minor.yy72;
	yygotominor.yy72->next = 0;
	yymsp[-1].minor.yy72->next = yygotominor.yy72;
}
#line 1064 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 7: /* enumList ::= enumMember */
#line 137 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy72 = gmlEnumMemberList();
	yygotominor.yy72->member = yymsp[0].minor.yy106;
	yygotominor.yy72->parent = 0;
	yygotominor.yy72->next = 0;
}
#line 1074 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 8: /* enumMember ::= enumTag */
#line 144 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy106 = gmlEnumMember();
	yygotominor.yy106->name = yymsp[0].minor.yy66;
	yygotominor.yy106->value = -1;
}
#line 1083 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 9: /* enumMember ::= enumTag EQUAL INT */
#line 150 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy106 = gmlEnumMember();
	yygotominor.yy106->name = yymsp[-2].minor.yy66;
	yygotominor.yy106->value = yymsp[0].minor.yy0.iValue;
	#ifdef TRACE_PARSING
	printf("enum tag= %s value=%i\n",yygotominor.yy106->name->value,yygotominor.yy106->value);
	#endif
}
#line 1095 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 10: /* enumTag ::= STRING */
      case 21: /* memberName ::= STRING */ yytestcase(yyruleno==21);
#line 159 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy66 = gmlString(yymsp[0].minor.yy0.strValue);
}
#line 1103 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 11: /* object ::= OBJECT objectName tags LBRACKET memberList RBRACKET */
#line 212 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy90 = gmlObject();
	yygotominor.yy90->name = yymsp[-4].minor.yy66;
	yygotominor.yy90->parentName = NULL;
	
	//reverse the list which was built backward due to the recommended stack optimization
	GmlObjectMemberList *l = yymsp[-1].minor.yy40;
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
	yygotominor.yy90->memberList = l;
}
#line 1131 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 12: /* object ::= OBJECT objectName COLUMN parentName tags LBRACKET memberList RBRACKET */
#line 237 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy90 = gmlObject();
	yygotominor.yy90->name = yymsp[-6].minor.yy66;
	yygotominor.yy90->parentName = yymsp[-4].minor.yy66;

	//reverse the list which was built backward due to the recommended stack optimization
	GmlObjectMemberList *l = yymsp[-1].minor.yy40;
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
	yygotominor.yy90->memberList = l;
}
#line 1159 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 13: /* object ::= OBJECT objectName */
#line 262 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy90 = gmlObject();
	yygotominor.yy90->name = yymsp[0].minor.yy66;
	yygotominor.yy90->parentName = NULL;
	yygotominor.yy90->memberList = NULL;
}
#line 1169 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 14: /* objectName ::= STRING */
#line 269 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	#ifdef TRACE_PARSING
	printf("object name= %s\n",yymsp[0].minor.yy0.strValue);
	#endif
	yygotominor.yy66 = gmlString(yymsp[0].minor.yy0.strValue);
}
#line 1179 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 15: /* parentName ::= STRING */
#line 276 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	#ifdef TRACE_PARSING
	printf("parent name= %s\n",yymsp[0].minor.yy0.strValue);
	#endif
	yygotominor.yy66 = gmlString(yymsp[0].minor.yy0.strValue);
}
#line 1189 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 16: /* memberList ::= memberList member */
#line 284 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy40 = gmlObjectMemberList();
	yygotominor.yy40->member = yymsp[0].minor.yy26;
	yygotominor.yy40->parent = yymsp[-1].minor.yy40;
	yygotominor.yy40->next = 0;
	yymsp[-1].minor.yy40->next = yygotominor.yy40;
}
#line 1200 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 17: /* memberList ::= */
#line 292 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy40 = gmlObjectMemberList();
	yygotominor.yy40->member = 0;
	yygotominor.yy40->parent = 0;
	yygotominor.yy40->next = 0;
}
#line 1210 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 18: /* member ::= type tags memberName EQUAL value */
#line 300 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy26 = gmlObjectMember();
	yygotominor.yy26->isArray = false;
	yygotominor.yy26->name = yymsp[-2].minor.yy66;
	yygotominor.yy26->type = yymsp[-4].minor.yy5;
	yygotominor.yy26->defaultValue = yymsp[0].minor.yy42;
}
#line 1221 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 19: /* member ::= type tags memberName ARRAY */
#line 308 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy26 = gmlObjectMember();
	yygotominor.yy26->isArray = true;
	yygotominor.yy26->name = yymsp[-1].minor.yy66;
	yygotominor.yy26->type = yymsp[-3].minor.yy5;
	yygotominor.yy26->defaultValue = NULL;
}
#line 1232 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 20: /* member ::= type tags memberName */
#line 316 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy26 = gmlObjectMember();
	yygotominor.yy26->isArray = false;
	yygotominor.yy26->name = yymsp[0].minor.yy66;
	yygotominor.yy26->type = yymsp[-2].minor.yy5;
	yygotominor.yy26->defaultValue = NULL;
}
#line 1243 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 22: /* tags ::= LTAG tagList RTAG */
#line 330 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy66 = yymsp[-1].minor.yy66;
}
#line 1250 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 23: /* tags ::= */
      case 24: /* tagList ::= tagList tag */ yytestcase(yyruleno==24);
      case 25: /* tagList ::= */ yytestcase(yyruleno==25);
      case 26: /* tag ::= BOOL */ yytestcase(yyruleno==26);
      case 27: /* tag ::= INT */ yytestcase(yyruleno==27);
      case 28: /* tag ::= FLOAT */ yytestcase(yyruleno==28);
      case 29: /* tag ::= STRINGVALUE */ yytestcase(yyruleno==29);
      case 30: /* tag ::= STRING */ yytestcase(yyruleno==30);
      case 31: /* tag ::= TAG */ yytestcase(yyruleno==31);
#line 335 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy66 = 0;
}
#line 1265 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 32: /* value ::= BOOL */
#line 375 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(yymsp[0].minor.yy0.bValue);
}
#line 1272 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 33: /* value ::= INT */
#line 380 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(yymsp[0].minor.yy0.iValue);
}
#line 1279 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 34: /* value ::= FLOAT value3f */
#line 385 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(yymsp[-1].minor.yy0.fValue,yymsp[0].minor.yy42->f3Value[0],yymsp[0].minor.yy42->f3Value[1],yymsp[0].minor.yy42->f3Value[2]);
}
#line 1286 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 35: /* value ::= INT value3f */
#line 390 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue((float)yymsp[-1].minor.yy0.iValue,yymsp[0].minor.yy42->f3Value[0],yymsp[0].minor.yy42->f3Value[1],yymsp[0].minor.yy42->f3Value[2]);
}
#line 1293 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 36: /* value3f ::= FLOAT value2f */
#line 395 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(yymsp[-1].minor.yy0.fValue,yymsp[0].minor.yy42->f2Value[0],yymsp[0].minor.yy42->f2Value[1]);
}
#line 1300 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 37: /* value3f ::= INT value2f */
#line 400 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue((float)yymsp[-1].minor.yy0.iValue,yymsp[0].minor.yy42->f2Value[0],yymsp[0].minor.yy42->f2Value[1]);
}
#line 1307 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 38: /* value2f ::= FLOAT FLOAT */
#line 405 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(yymsp[-1].minor.yy0.fValue,yymsp[0].minor.yy0.fValue);
}
#line 1314 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 39: /* value2f ::= INT FLOAT */
#line 410 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue((float)yymsp[-1].minor.yy0.iValue,yymsp[0].minor.yy0.fValue);
}
#line 1321 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 40: /* value2f ::= FLOAT INT */
#line 415 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(yymsp[-1].minor.yy0.fValue,(float)yymsp[0].minor.yy0.iValue);
}
#line 1328 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 41: /* value2f ::= INT INT */
#line 420 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue((float)yymsp[-1].minor.yy0.iValue,(float)yymsp[0].minor.yy0.iValue);
}
#line 1335 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 42: /* value ::= value3f */
      case 43: /* value ::= value2f */ yytestcase(yyruleno==43);
#line 425 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = yymsp[0].minor.yy42;
}
#line 1343 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 44: /* value ::= FLOAT */
#line 435 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(yymsp[0].minor.yy0.fValue);
}
#line 1350 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 45: /* value ::= PLUSINF */
#line 439 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(kValue_PLUSINF);
}
#line 1357 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 46: /* value ::= MINUSINF */
#line 443 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(kValue_MINUSINF);
}
#line 1364 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 47: /* value ::= STRINGVALUE */
#line 447 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(kValue_QUOTEDSTRING,yymsp[0].minor.yy0.strValue);
}
#line 1371 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 48: /* value ::= STRING */
#line 451 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy42 = gmlValue(kValue_STRING,yymsp[0].minor.yy0.strValue);
}
#line 1378 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 49: /* type ::= typeModifierList memberType */
#line 456 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy5 = gmlType();
	yygotominor.yy5->memberType = yymsp[0].minor.yy107;

	//reverse the list which was built backward due to the recommended stack optimization
	GmlTypeModifierList *l = yymsp[-1].minor.yy2;
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
	yygotominor.yy5->modifierList = l;
}
#line 1405 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 50: /* memberType ::= TYPEBOOL */
#line 480 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_BOOL;
}
#line 1413 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 51: /* memberType ::= TYPEUINT */
#line 485 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_UINT;
}
#line 1421 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 52: /* memberType ::= TYPEINT */
#line 490 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_INT;
}
#line 1429 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 53: /* memberType ::= TYPEUFLOAT */
#line 495 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_UFLOAT;
}
#line 1437 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 54: /* memberType ::= TYPEFLOAT */
#line 500 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_FLOAT;
}
#line 1445 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 55: /* memberType ::= TYPEFLOAT2 */
#line 505 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_FLOAT2;
}
#line 1453 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 56: /* memberType ::= TYPEFLOAT3 */
#line 510 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_FLOAT3;
}
#line 1461 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 57: /* memberType ::= TYPEFLOAT4 */
#line 515 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_FLOAT4;
}
#line 1469 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 58: /* memberType ::= TYPESTRING */
#line 520 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_STRING;
}
#line 1477 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 59: /* memberType ::= TYPEEXPRESSION */
#line 525 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_EXPRESSION;
}
#line 1485 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 60: /* memberType ::= STRING */
#line 531 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy107 = gmlMemberType();
	yygotominor.yy107->type = kMemberType_OBJECT;
	yygotominor.yy107->objectName = gmlString(yymsp[0].minor.yy0.strValue);
}
#line 1494 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 61: /* typeModifierList ::= typeModifierList typeModifier */
#line 538 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy2 = gmlTypeModifierList();
	yygotominor.yy2->modifier = yymsp[0].minor.yy98;
	yygotominor.yy2->parent = yymsp[-1].minor.yy2;
	yygotominor.yy2->next = 0;
	yymsp[-1].minor.yy2->next = yygotominor.yy2;
}
#line 1505 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 62: /* typeModifierList ::= */
#line 546 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy2 = gmlTypeModifierList();
	yygotominor.yy2->modifier = 0;
	yygotominor.yy2->parent = 0;
	yygotominor.yy2->next = 0;
}
#line 1515 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 63: /* typeModifier ::= NONZERO */
#line 553 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy98 = gmlTypeModifer();
	yygotominor.yy98->type = kTypeModifier_NONZERO;
}
#line 1523 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      case 64: /* typeModifier ::= REF */
#line 558 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"
{
	yygotominor.yy98 = gmlTypeModifer();
	yygotominor.yy98->type = kTypeModifier_REF;
}
#line 1531 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
        break;
      default:
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    /* If the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
      yyTraceShift(yypParser, yyact);
    }else{
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 13 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"

	printf("file(%i): syntax error\n",extra->line);
    extra->valid = false;
#line 1596 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 18 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.y"

    //printf("parsing complete!\n");
#line 1618 "C:/work/GitHubReleases/GameCode/src/gamemeta/gamemeta.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      if( yyact > YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sReturn\n",yyTracePrompt);
  }
#endif
  return;
}
