/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 2 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"

    #include <cassert>
    #include <iostream>
	#include "codegrammar.h"
	//#define TRACE_PARSING
#line 14 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
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
#define YYNOCODE 24
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE GclParserToken
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  GclMember* yy9;
  GclValue* yy22;
  GclObject* yy28;
  GclObjectList* yy38;
  GclItemList* yy40;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL GclParserExtra* extra;
#define ParseARG_PDECL ,GclParserExtra* extra
#define ParseARG_FETCH GclParserExtra* extra = yypParser->extra
#define ParseARG_STORE yypParser->extra = extra
#define YYNSTATE             23
#define YYNRULE              32
#define YY_MAX_SHIFT         22
#define YY_MIN_SHIFTREDUCE   50
#define YY_MAX_SHIFTREDUCE   81
#define YY_MIN_REDUCE        82
#define YY_MAX_REDUCE        113
#define YY_ERROR_ACTION      114
#define YY_ACCEPT_ACTION     115
#define YY_NO_ACTION         116

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
#define YY_ACTTAB_COUNT (62)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    21,   81,   14,    8,    9,   70,   65,    7,    6,   78,
 /*    10 */    79,   80,   62,   64,   69,   67,   60,   76,   75,    3,
 /*    20 */    10,   11,   61,   63,   69,   12,   59,   76,   75,   21,
 /*    30 */    20,   51,   54,  115,   21,   20,    4,   55,   57,   21,
 /*    40 */    20,   56,   53,    5,   70,   68,   15,   16,   17,   18,
 /*    50 */    19,   82,   21,   74,   72,   84,   73,   71,    2,    1,
 /*    60 */    22,   13,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,    3,    8,    9,   20,    7,    8,    9,   10,
 /*    10 */    11,   12,   15,   16,   20,   21,   19,   20,   21,   17,
 /*    20 */     8,    9,   15,   16,   20,   14,   19,   20,   21,    1,
 /*    30 */     2,   15,    4,   22,    1,    2,   17,    4,   15,    1,
 /*    40 */     2,   18,    4,   17,   20,   21,    3,    8,    9,    8,
 /*    50 */     9,    0,    1,    8,    9,   23,    8,    9,    5,    6,
 /*    60 */     2,    3,
};
#define YY_SHIFT_USE_DFLT (-6)
#define YY_SHIFT_COUNT (22)
#define YY_SHIFT_MIN   (-5)
#define YY_SHIFT_MAX   (58)
static const signed char yy_shift_ofst[] = {
 /*     0 */    -6,   -1,   -1,   28,   33,   38,   -5,   12,   39,   41,
 /*    10 */    39,   41,   51,   -6,   -6,   -6,   45,   48,   45,   48,
 /*    20 */    53,   58,   43,
};
#define YY_REDUCE_USE_DFLT (-16)
#define YY_REDUCE_COUNT (15)
#define YY_REDUCE_MIN   (-15)
#define YY_REDUCE_MAX   (26)
static const signed char yy_reduce_ofst[] = {
 /*     0 */    11,   -3,    7,   23,   23,   23,   -6,   24,  -15,    4,
 /*    10 */   -15,    4,   16,    2,   19,   26,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    84,  114,  114,  114,  114,  114,  109,   98,  105,  103,
 /*    10 */   106,  104,  114,   90,   90,   90,  106,  104,  105,  103,
 /*    20 */   114,  114,  114,
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
  "$",             "OBJECTTYPE",    "STRING",        "LBRACKET",    
  "RBRACKET",      "EQUAL",         "PLUSEQUAL",     "BOOL",        
  "INT",           "FLOAT",         "PLUSINF",       "MINUSINF",    
  "STRINGVALUE",   "error",         "objectList",    "object",      
  "untypedobject",  "itemList",      "member",        "value",       
  "value2f",       "value3f",       "start",       
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "start ::= objectList",
 /*   1 */ "objectList ::= objectList object",
 /*   2 */ "objectList ::=",
 /*   3 */ "object ::= OBJECTTYPE STRING LBRACKET itemList RBRACKET",
 /*   4 */ "object ::= OBJECTTYPE LBRACKET itemList RBRACKET",
 /*   5 */ "untypedobject ::= LBRACKET itemList RBRACKET",
 /*   6 */ "itemList ::= itemList member",
 /*   7 */ "itemList ::= itemList object",
 /*   8 */ "itemList ::=",
 /*   9 */ "member ::= STRING EQUAL value",
 /*  10 */ "member ::= STRING PLUSEQUAL value",
 /*  11 */ "member ::= STRING EQUAL object",
 /*  12 */ "member ::= STRING PLUSEQUAL object",
 /*  13 */ "member ::= STRING EQUAL untypedobject",
 /*  14 */ "member ::= STRING PLUSEQUAL untypedobject",
 /*  15 */ "value ::= BOOL",
 /*  16 */ "value ::= INT",
 /*  17 */ "value ::= FLOAT value3f",
 /*  18 */ "value ::= INT value3f",
 /*  19 */ "value3f ::= FLOAT value2f",
 /*  20 */ "value3f ::= INT value2f",
 /*  21 */ "value2f ::= FLOAT FLOAT",
 /*  22 */ "value2f ::= INT FLOAT",
 /*  23 */ "value2f ::= FLOAT INT",
 /*  24 */ "value2f ::= INT INT",
 /*  25 */ "value ::= value3f",
 /*  26 */ "value ::= value2f",
 /*  27 */ "value ::= FLOAT",
 /*  28 */ "value ::= PLUSINF",
 /*  29 */ "value ::= MINUSINF",
 /*  30 */ "value ::= STRINGVALUE",
 /*  31 */ "value ::= STRING",
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
    case 14: /* objectList */
{
#line 23 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
gclFreeObjectList((yypminor->yy38));
#line 433 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
}
      break;
    case 15: /* object */
{
#line 26 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
gclFreeObject((yypminor->yy28));
#line 440 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
}
      break;
    case 16: /* untypedobject */
{
#line 29 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
 gclFreeObject((yypminor->yy28)); 
#line 447 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
}
      break;
    case 17: /* itemList */
{
#line 32 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
 gclFreeItemList((yypminor->yy40)); 
#line 454 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
}
      break;
    case 18: /* member */
{
#line 35 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
 gclFreeMember((yypminor->yy9)); 
#line 461 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
}
      break;
    case 19: /* value */
    case 20: /* value2f */
    case 21: /* value3f */
{
#line 38 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
 gclFreeValue((yypminor->yy22)); 
#line 470 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
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
  { 22, 1 },
  { 14, 2 },
  { 14, 0 },
  { 15, 5 },
  { 15, 4 },
  { 16, 3 },
  { 17, 2 },
  { 17, 2 },
  { 17, 0 },
  { 18, 3 },
  { 18, 3 },
  { 18, 3 },
  { 18, 3 },
  { 18, 3 },
  { 18, 3 },
  { 19, 1 },
  { 19, 1 },
  { 19, 2 },
  { 19, 2 },
  { 21, 2 },
  { 21, 2 },
  { 20, 2 },
  { 20, 2 },
  { 20, 2 },
  { 20, 2 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
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
      case 0: /* start ::= objectList */
#line 47 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	//reverse the list which was built backward due to the recommended stack optimization
	GclObjectList *l = yymsp[0].minor.yy38;
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
#line 830 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 1: /* objectList ::= objectList object */
#line 68 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy38 = gclObjectList();
	yygotominor.yy38->object = yymsp[0].minor.yy28;
	yygotominor.yy38->next = 0;
	yygotominor.yy38->parent = yymsp[-1].minor.yy38;
	yymsp[-1].minor.yy38->next = yygotominor.yy38;
}
#line 841 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 2: /* objectList ::= */
#line 77 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy38 = gclObjectList();
	yygotominor.yy38->object = 0;
	yygotominor.yy38->next = 0;
	yygotominor.yy38->parent = 0;
}
#line 851 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 3: /* object ::= OBJECTTYPE STRING LBRACKET itemList RBRACKET */
#line 85 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	#ifdef TRACE_PARSING
	printf("object name= %s\n",yymsp[-3].minor.yy0.strValue);
	#endif
	yygotominor.yy28 = gclObject();
	yygotominor.yy28->name = gclString(yymsp[-3].minor.yy0.strValue);
	yygotominor.yy28->type = gclString(yymsp[-4].minor.yy0.strValue);
	yygotominor.yy28->typeObject = yymsp[-4].minor.yy0.metaObject;
	yygotominor.yy28->compiledObjectIndex = -1;
	yygotominor.yy28->line = extra->first_line;
	yygotominor.yy28->col = extra->first_column;
	
	//reverse the list which was built backward due to the recommended stack optimization
	GclItemList *l = yymsp[-1].minor.yy40;
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
	yygotominor.yy28->itemList = l;
}
#line 886 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 4: /* object ::= OBJECTTYPE LBRACKET itemList RBRACKET */
#line 118 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
#ifdef TRACE_PARSING
	printf("object name= %s\n", "unnamed object");
#endif
	yygotominor.yy28 = gclObject();
	yygotominor.yy28->name = NULL;
	yygotominor.yy28->type = gclString(yymsp[-3].minor.yy0.strValue);
	yygotominor.yy28->typeObject = yymsp[-3].minor.yy0.metaObject;
	yygotominor.yy28->compiledObjectIndex = -1;
	yygotominor.yy28->line = extra->first_line;
	yygotominor.yy28->col = extra->first_column;

	//reverse the list which was built backward due to the recommended stack optimization
	GclItemList *l = yymsp[-1].minor.yy40;
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
	yygotominor.yy28->itemList = l;
}
#line 921 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 5: /* untypedobject ::= LBRACKET itemList RBRACKET */
#line 151 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
#ifdef TRACE_PARSING
	printf("object name= %s\n", "unnamed object");
#endif
	yygotominor.yy28 = gclObject();
	yygotominor.yy28->name = NULL;
	yygotominor.yy28->type = NULL;
	yygotominor.yy28->typeObject = NULL;
	yygotominor.yy28->compiledObjectIndex = -1;
	yygotominor.yy28->line = extra->first_line;
	yygotominor.yy28->col = extra->first_column;

	//reverse the list which was built backward due to the recommended stack optimization
	GclItemList *l = yymsp[-1].minor.yy40;
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
	yygotominor.yy28->itemList = l;
}
#line 956 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 6: /* itemList ::= itemList member */
#line 184 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy40 = gclItemList();
	yygotominor.yy40->object = 0;
	yygotominor.yy40->member = yymsp[0].minor.yy9;
	yygotominor.yy40->parent = yymsp[-1].minor.yy40;
	yygotominor.yy40->next = 0;
	yymsp[-1].minor.yy40->next = yygotominor.yy40;
}
#line 968 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 7: /* itemList ::= itemList object */
#line 194 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy40 = gclItemList();
	yygotominor.yy40->object = yymsp[0].minor.yy28;
	yygotominor.yy40->member = 0;
	yygotominor.yy40->parent = yymsp[-1].minor.yy40;
	yygotominor.yy40->next = 0;
	yymsp[-1].minor.yy40->next = yygotominor.yy40;
}
#line 980 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 8: /* itemList ::= */
#line 204 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy40 = gclItemList();
	yygotominor.yy40->object = 0;
	yygotominor.yy40->member = 0;
	yygotominor.yy40->parent = 0;
	yygotominor.yy40->next = 0;
}
#line 991 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 9: /* member ::= STRING EQUAL value */
#line 213 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", yymsp[-2].minor.yy0.strValue);
#endif
	yygotominor.yy9 = gclMember();
	yygotominor.yy9->name = gclString(yymsp[-2].minor.yy0.strValue);
	yygotominor.yy9->value = yymsp[0].minor.yy22;
	yygotominor.yy9->object = 0;
	yygotominor.yy9->line = extra->first_line;
	yygotominor.yy9->col = extra->first_column;
}
#line 1006 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 10: /* member ::= STRING PLUSEQUAL value */
#line 226 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", yymsp[-2].minor.yy0.strValue);
#endif

	//TODO
	yygotominor.yy9 = gclMember();
	yygotominor.yy9->name = gclString(yymsp[-2].minor.yy0.strValue);
	yygotominor.yy9->value = yymsp[0].minor.yy22;
	yygotominor.yy9->object = 0;
	yygotominor.yy9->line = extra->first_line;
	yygotominor.yy9->col = extra->first_column;
}
#line 1023 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 11: /* member ::= STRING EQUAL object */
      case 13: /* member ::= STRING EQUAL untypedobject */ yytestcase(yyruleno==13);
#line 241 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", yymsp[-2].minor.yy0.strValue);
#endif

	yygotominor.yy9 = gclMember();
	yygotominor.yy9->name = gclString(yymsp[-2].minor.yy0.strValue);
	yygotominor.yy9->value = 0;
	yygotominor.yy9->object = yymsp[0].minor.yy28;
	yygotominor.yy9->line = extra->first_line;
	yygotominor.yy9->col = extra->first_column;
}
#line 1040 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 12: /* member ::= STRING PLUSEQUAL object */
#line 255 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", yymsp[-2].minor.yy0.strValue);
#endif

	//TODO
	yygotominor.yy9 = gclMember();
	yygotominor.yy9->name = gclString(yymsp[-2].minor.yy0.strValue);
	yygotominor.yy9->value = 0;
	yygotominor.yy9->object = yymsp[0].minor.yy28;
	yygotominor.yy9->line = extra->first_line;
	yygotominor.yy9->col = extra->first_column;
}
#line 1057 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 14: /* member ::= STRING PLUSEQUAL untypedobject */
#line 284 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
#ifdef TRACE_PARSING
	printf("member name= %s\n", yymsp[-2].minor.yy0.strValue);
#endif

	//TODO
	yygotominor.yy9 = gclMember();
	yygotominor.yy9->name = gclString(yymsp[-2].minor.yy0.strValue);
	yygotominor.yy9->value = 0;
	yygotominor.yy9->object = yymsp[0].minor.yy28;
	yygotominor.yy9->line = extra->first_line;
	yygotominor.yy9->col = extra->first_column;
	//printf("%i,%i(%i): %s\n",extra->first_line,extra->first_column,extra->line,yygotominor.yy9->name);
}
#line 1075 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 15: /* value ::= BOOL */
#line 300 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(yymsp[0].minor.yy0.bValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1084 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 16: /* value ::= INT */
#line 306 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(yymsp[0].minor.yy0.iValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1093 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 17: /* value ::= FLOAT value3f */
#line 313 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(yymsp[-1].minor.yy0.fValue,yymsp[0].minor.yy22->f3Value[0],yymsp[0].minor.yy22->f3Value[1],yymsp[0].minor.yy22->f3Value[2]);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1102 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 18: /* value ::= INT value3f */
#line 320 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue((float)yymsp[-1].minor.yy0.iValue,yymsp[0].minor.yy22->f3Value[0],yymsp[0].minor.yy22->f3Value[1],yymsp[0].minor.yy22->f3Value[2]);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1111 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 19: /* value3f ::= FLOAT value2f */
#line 327 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(yymsp[-1].minor.yy0.fValue,yymsp[0].minor.yy22->f2Value[0],yymsp[0].minor.yy22->f2Value[1]);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1120 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 20: /* value3f ::= INT value2f */
#line 334 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue((float)yymsp[-1].minor.yy0.iValue,yymsp[0].minor.yy22->f2Value[0],yymsp[0].minor.yy22->f2Value[1]);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1129 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 21: /* value2f ::= FLOAT FLOAT */
#line 341 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(yymsp[-1].minor.yy0.fValue,yymsp[0].minor.yy0.fValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1138 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 22: /* value2f ::= INT FLOAT */
#line 348 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue((float)yymsp[-1].minor.yy0.iValue,yymsp[0].minor.yy0.fValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1147 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 23: /* value2f ::= FLOAT INT */
#line 355 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(yymsp[-1].minor.yy0.fValue,(float)yymsp[0].minor.yy0.iValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1156 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 24: /* value2f ::= INT INT */
#line 362 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue((float)yymsp[-1].minor.yy0.iValue,(float)yymsp[0].minor.yy0.iValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1165 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 25: /* value ::= value3f */
      case 26: /* value ::= value2f */ yytestcase(yyruleno==26);
#line 369 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = yymsp[0].minor.yy22;
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1175 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 27: /* value ::= FLOAT */
#line 383 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(yymsp[0].minor.yy0.fValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1184 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 28: /* value ::= PLUSINF */
#line 389 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(kValue_PLUSINF);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1193 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 29: /* value ::= MINUSINF */
#line 395 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(kValue_MINUSINF);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1202 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 30: /* value ::= STRINGVALUE */
#line 401 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(kValue_QUOTEDSTRING, yymsp[0].minor.yy0.strValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1211 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
        break;
      case 31: /* value ::= STRING */
#line 407 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"
{
	yygotominor.yy22 = gclValue(kValue_STRING, yymsp[0].minor.yy0.strValue);
	yygotominor.yy22->line = extra->first_line;
	yygotominor.yy22->col = extra->first_column;
}
#line 1220 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
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
#line 13 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"

	printf("%s(%i,%i): syntax error\n", extra->filename, (int)extra->first_line, (int)extra->first_column);
    extra->valid = false;
#line 1285 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
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
#line 18 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.y"

    //printf("parsing complete!\n");
#line 1307 "C:/work/GitHubReleases/GameCode/src/gamecode/gamecode.c"
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
