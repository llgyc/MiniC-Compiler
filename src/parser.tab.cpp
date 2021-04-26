/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y" /* yacc.c:337  */


#include <memory>

#include <stdio.h>
#include <stdint.h>

#include "context.hpp"
#include "operator.hpp"

extern int yylineno;
extern int yylex(void);
extern void yyerror(BaseASTNode **, const char *);
extern int yyparse(BaseASTNode **);


#line 87 "parser.tab.cpp" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.hpp".  */
#ifndef YY_YY_PARSER_TAB_HPP_INCLUDED
# define YY_YY_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CONST = 258,
    INT = 259,
    VOID = 260,
    IF = 261,
    WHILE = 262,
    BREAK = 263,
    CONTINUE = 264,
    RETURN = 265,
    THEN = 266,
    ELSE = 267,
    INT_CONST = 268,
    IDENT = 269,
    OR_OP = 270,
    AND_OP = 271,
    EQ_OP = 272,
    NE_OP = 273,
    LE_OP = 274,
    GE_OP = 275
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 20 "parser.y" /* yacc.c:352  */

    char *str_val;
    int int_val;
    BaseASTNode *node_ptr;

#line 157 "parser.tab.cpp" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (BaseASTNode **root_ptr_ptr);

#endif /* !YY_YY_PARSER_TAB_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   207

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  167

#define YYUNDEFTOK  2
#define YYMAXUTOK   275

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    32,     2,     2,     2,    35,     2,     2,
      28,    29,    33,    30,    22,    31,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    21,
      36,    23,    37,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    24,     2,    25,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,    27,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    44,    44,    49,    53,    57,    61,    68,    69,    73,
      78,    82,    89,    98,    99,   106,   107,   109,   114,   118,
     125,   130,   134,   141,   146,   154,   155,   157,   162,   166,
     173,   179,   185,   191,   200,   204,   211,   215,   223,   229,
     230,   237,   238,   242,   246,   248,   249,   250,   254,   258,
     262,   264,   266,   268,   275,   279,   283,   292,   293,   300,
     302,   303,   308,   309,   313,   318,   322,   326,   333,   337,
     344,   345,   350,   355,   363,   364,   369,   377,   378,   383,
     388,   393,   401,   402,   407,   415,   416,   424,   425,   433
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONST", "INT", "VOID", "IF", "WHILE",
  "BREAK", "CONTINUE", "RETURN", "THEN", "ELSE", "INT_CONST", "IDENT",
  "OR_OP", "AND_OP", "EQ_OP", "NE_OP", "LE_OP", "GE_OP", "';'", "','",
  "'='", "'['", "']'", "'{'", "'}'", "'('", "')'", "'+'", "'-'", "'!'",
  "'*'", "'/'", "'%'", "'<'", "'>'", "$accept", "ProgramStart", "CompUnit",
  "Decl", "ConstDecl", "ConstDefList", "ConstDef", "ConstIndexList",
  "ConstInitVal", "ConstInitValList", "VarDecl", "VarDefList", "VarDef",
  "InitVal", "InitValList", "FuncDef", "FuncFParams", "FuncFParam",
  "Block", "BlockItems", "BlockItem", "Stmt", "Exp", "Cond", "LVal",
  "IndexList", "PrimaryExp", "UnaryExp", "FuncRParams", "MulExp", "AddExp",
  "RelExp", "EqExp", "LAndExp", "LOrExp", "ConstExp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    59,    44,    61,    91,    93,   123,   125,    40,    41,
      43,    45,    33,    42,    47,    37,    60,    62
};
# endif

#define YYPACT_NINF -108

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-108)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      16,    18,    22,    24,    48,    16,  -108,  -108,  -108,  -108,
      61,    79,    44,  -108,    93,  -108,  -108,  -108,  -108,    68,
    -108,     5,    39,  -108,    64,     6,    70,  -108,    61,   126,
     115,   -11,  -108,   125,   161,  -108,  -108,   115,     2,   136,
    -108,   121,  -108,  -108,   148,   115,  -108,   130,    71,   161,
     161,   161,   161,  -108,  -108,  -108,  -108,  -108,    47,    97,
      97,   129,  -108,   115,    78,  -108,  -108,   134,   116,  -108,
    -108,    41,   137,  -108,  -108,    30,   131,  -108,  -108,  -108,
     161,   161,   161,   161,   161,  -108,  -108,  -108,  -108,    34,
    -108,    64,   135,   143,   144,   151,   156,  -108,  -108,  -108,
    -108,  -108,  -108,   152,   153,  -108,  -108,    31,   161,   125,
    -108,  -108,  -108,  -108,  -108,    47,    47,   136,  -108,   157,
     161,   161,  -108,  -108,  -108,   162,  -108,   161,   161,  -108,
     155,  -108,  -108,   165,    97,   159,   114,   166,   170,   168,
    -108,   169,  -108,  -108,    19,   161,   161,   161,   161,   161,
     161,   161,   161,    19,  -108,   186,    97,    97,    97,    97,
     159,   159,   114,   166,  -108,    19,  -108
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     3,     7,     8,     4,
       0,    13,     0,    21,     0,     1,     5,     6,    13,     0,
      10,     0,    23,    20,     0,     0,     0,     9,     0,     0,
       0,     0,    34,     0,     0,    13,    22,     0,     0,     0,
      11,    36,    39,    31,     0,     0,    61,    57,     0,     0,
       0,     0,     0,    24,    25,    60,    62,    70,    74,    54,
      89,     0,    30,     0,     0,    12,    15,     0,     0,    35,
      33,     0,    56,    26,    28,     0,     0,    65,    66,    67,
       0,     0,     0,     0,     0,    14,    32,    16,    18,     0,
      13,     0,     0,     0,     0,     0,     0,    44,    38,    41,
      46,    40,    42,     0,    60,    63,    68,     0,     0,     0,
      27,    59,    71,    72,    73,    75,    76,     0,    17,    37,
       0,     0,    50,    51,    52,     0,    45,     0,     0,    64,
       0,    29,    19,     0,    77,    82,    85,    87,    55,     0,
      53,     0,    69,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,    47,    80,    81,    78,    79,
      83,    84,    86,    88,    49,     0,    48
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -108,  -108,  -108,    -1,  -108,  -108,   171,   -16,   -58,  -108,
    -108,  -108,   176,   -41,  -108,   196,   177,   160,   -22,  -108,
    -108,  -107,   -32,    82,   -65,  -108,  -108,   -38,  -108,    50,
     -34,   -14,    54,    55,  -108,   172
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    19,    20,    22,    65,    89,
       8,    12,    13,    53,    75,     9,    31,    32,   100,    68,
     101,   102,   103,   133,    55,    72,    56,    57,   107,    58,
      59,   135,   136,   137,   138,    66
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      60,    54,    26,   104,    16,    60,    88,    74,    43,    29,
      29,    44,    77,    78,    79,    62,    54,    76,    45,     1,
       2,     3,    10,    70,    44,    92,    93,    94,    95,    96,
      60,    63,    46,    47,    30,    37,    11,   155,    14,   106,
      97,    86,   112,   113,   114,    42,   164,    49,    15,    50,
      51,    52,   109,   128,    46,    47,   117,   110,   166,   132,
     129,   118,    33,    34,   125,    23,    24,    99,   131,    49,
     105,    50,    51,    52,   119,    18,   130,    54,    35,   104,
      80,    81,    82,    60,    46,    47,   134,   134,   104,    27,
      28,    46,    47,    39,    34,   141,   142,    48,    73,    49,
     104,    50,    51,    52,    64,    87,    49,    21,    50,    51,
      52,   156,   157,   158,   159,   134,   134,   134,   134,     1,
      91,    25,    92,    93,    94,    95,    96,    83,    84,    46,
      47,   149,   150,   115,   116,   160,   161,    97,    46,    47,
      41,    42,    42,    98,    49,    67,    50,    51,    52,    46,
      47,    48,    29,    49,    85,    50,    51,    52,    71,    90,
     111,   108,    64,   120,    49,   122,    50,    51,    52,    46,
      47,   121,   123,   126,    46,    47,   127,   124,   145,   146,
     143,    34,   151,   140,    49,   152,    50,    51,    52,    49,
     154,    50,    51,    52,   144,   147,   148,   153,   165,    40,
      36,    17,    38,   139,    69,   162,    61,   163
};

static const yytype_uint8 yycheck[] =
{
      34,    33,    18,    68,     5,    39,    64,    48,    30,     4,
       4,    22,    50,    51,    52,    37,    48,    49,    29,     3,
       4,     5,     4,    45,    22,     6,     7,     8,     9,    10,
      64,    29,    13,    14,    29,    29,    14,   144,    14,    71,
      21,    63,    80,    81,    82,    26,   153,    28,     0,    30,
      31,    32,    22,    22,    13,    14,    22,    27,   165,   117,
      29,    27,    23,    24,    96,    21,    22,    68,   109,    28,
      29,    30,    31,    32,    90,    14,   108,   109,    14,   144,
      33,    34,    35,   117,    13,    14,   120,   121,   153,    21,
      22,    13,    14,    23,    24,   127,   128,    26,    27,    28,
     165,    30,    31,    32,    26,    27,    28,    28,    30,    31,
      32,   145,   146,   147,   148,   149,   150,   151,   152,     3,
       4,    28,     6,     7,     8,     9,    10,    30,    31,    13,
      14,    17,    18,    83,    84,   149,   150,    21,    13,    14,
      14,    26,    26,    27,    28,    24,    30,    31,    32,    13,
      14,    26,     4,    28,    25,    30,    31,    32,    28,    25,
      29,    24,    26,    28,    28,    21,    30,    31,    32,    13,
      14,    28,    21,    21,    13,    14,    23,    21,    19,    20,
      25,    24,    16,    21,    28,    15,    30,    31,    32,    28,
      21,    30,    31,    32,    29,    36,    37,    29,    12,    28,
      24,     5,    25,   121,    44,   151,    34,   152
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,    39,    40,    41,    42,    48,    53,
       4,    14,    49,    50,    14,     0,    41,    53,    14,    43,
      44,    28,    45,    21,    22,    28,    45,    21,    22,     4,
      29,    54,    55,    23,    24,    14,    50,    29,    54,    23,
      44,    14,    26,    56,    22,    29,    13,    14,    26,    28,
      30,    31,    32,    51,    60,    62,    64,    65,    67,    68,
      68,    73,    56,    29,    26,    46,    73,    24,    57,    55,
      56,    28,    63,    27,    51,    52,    60,    65,    65,    65,
      33,    34,    35,    30,    31,    25,    56,    27,    46,    47,
      25,     4,     6,     7,     8,     9,    10,    21,    27,    41,
      56,    58,    59,    60,    62,    29,    60,    66,    24,    22,
      27,    29,    65,    65,    65,    67,    67,    22,    27,    45,
      28,    28,    21,    21,    21,    60,    21,    23,    22,    29,
      60,    51,    46,    61,    68,    69,    70,    71,    72,    61,
      21,    60,    60,    25,    29,    19,    20,    36,    37,    17,
      18,    16,    15,    29,    21,    59,    68,    68,    68,    68,
      69,    69,    70,    71,    59,    12,    59
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    40,    40,    40,    40,    41,    41,    42,
      43,    43,    44,    45,    45,    46,    46,    46,    47,    47,
      48,    49,    49,    50,    50,    51,    51,    51,    52,    52,
      53,    53,    53,    53,    54,    54,    55,    55,    56,    57,
      57,    58,    58,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    60,    61,    62,    63,    63,    64,
      64,    64,    65,    65,    65,    65,    65,    65,    66,    66,
      67,    67,    67,    67,    68,    68,    68,    69,    69,    69,
      69,    69,    70,    70,    70,    71,    71,    72,    72,    73
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     1,     1,     4,
       1,     3,     4,     0,     4,     1,     2,     3,     1,     3,
       3,     1,     3,     2,     4,     1,     2,     3,     1,     3,
       5,     5,     6,     6,     1,     3,     2,     5,     3,     0,
       2,     1,     1,     4,     1,     2,     1,     5,     7,     5,
       2,     2,     2,     3,     1,     1,     2,     0,     4,     3,
       1,     1,     1,     3,     4,     2,     2,     2,     1,     3,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     1,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (root_ptr_ptr, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, root_ptr_ptr); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, BaseASTNode **root_ptr_ptr)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (root_ptr_ptr);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, BaseASTNode **root_ptr_ptr)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, root_ptr_ptr);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, BaseASTNode **root_ptr_ptr)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , root_ptr_ptr);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, root_ptr_ptr); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, BaseASTNode **root_ptr_ptr)
{
  YYUSE (yyvaluep);
  YYUSE (root_ptr_ptr);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (BaseASTNode **root_ptr_ptr)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 45 "parser.y" /* yacc.c:1652  */
    { *root_ptr_ptr = (yyvsp[0].node_ptr); }
#line 1371 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 3:
#line 50 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new CompUnitASTNode(std::move(ptr));
          }
#line 1379 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 4:
#line 54 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new CompUnitASTNode(std::move(ptr));
          }
#line 1387 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 5:
#line 58 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-1].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new CompUnitASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1395 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 6:
#line 62 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-1].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new CompUnitASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1403 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 9:
#line 74 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = (yyvsp[-1].node_ptr); }
#line 1409 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 10:
#line 79 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new ConstDefListASTNode(std::move(ptr));
          }
#line 1417 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 11:
#line 83 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new ConstDefListASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1425 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 12:
#line 90 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-3].str_val));
            ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new ConstDefASTNode(ident, std::move(ptr1), std::move(ptr2));
          }
#line 1434 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 13:
#line 98 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new ConstIndexListASTNode(); }
#line 1440 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 14:
#line 100 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-3].node_ptr)), ptr2((yyvsp[-1].node_ptr));
            (yyval.node_ptr) = new ConstIndexListASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1448 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 16:
#line 108 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new ConstInitValListASTNode(); }
#line 1454 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 17:
#line 110 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = (yyvsp[-1].node_ptr); }
#line 1460 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 18:
#line 115 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new ConstInitValListASTNode(std::move(ptr));
          }
#line 1468 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 19:
#line 119 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new ConstInitValListASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1476 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 20:
#line 126 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = (yyvsp[-1].node_ptr); }
#line 1482 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 21:
#line 131 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new VarDefListASTNode(std::move(ptr));
          }
#line 1490 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 22:
#line 135 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new VarDefListASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1498 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 23:
#line 142 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-1].str_val));
            ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new VarDefASTNode(ident, std::move(ptr));
          }
#line 1507 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 24:
#line 147 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-3].str_val));
            ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new VarDefASTNode(ident, std::move(ptr1), std::move(ptr2));
          }
#line 1516 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 26:
#line 156 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new InitValListASTNode(); }
#line 1522 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 27:
#line 158 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = (yyvsp[-1].node_ptr); }
#line 1528 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 28:
#line 163 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new InitValListASTNode(std::move(ptr1));
          }
#line 1536 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 29:
#line 167 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new InitValListASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1544 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 30:
#line 174 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-3].str_val));
            ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncDefASTNode(ReturnType::kVoidType, 
                ident, std::move(ptr));
          }
#line 1554 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 31:
#line 180 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-3].str_val));
            ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncDefASTNode(ReturnType::kIntType,
                ident, std::move(ptr));
          }
#line 1564 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 32:
#line 186 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-4].str_val));
            ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncDefASTNode(ReturnType::kVoidType,
                ident, std::move(ptr1), std::move(ptr2));
          }
#line 1574 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 33:
#line 192 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-4].str_val));
            ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncDefASTNode(ReturnType::kIntType,
                ident, std::move(ptr1), std::move(ptr2));
          }
#line 1584 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 34:
#line 201 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncFParamsASTNode(std::move(ptr1));
          }
#line 1592 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 35:
#line 205 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncFParamsASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1600 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 36:
#line 212 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[0].str_val));
            (yyval.node_ptr) = new FuncFParamASTNode(ident);
          }
#line 1608 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 37:
#line 216 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-3].str_val));
            ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncFParamASTNode(ident, std::move(ptr));
          }
#line 1617 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 38:
#line 224 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = (yyvsp[-1].node_ptr); }
#line 1623 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 39:
#line 229 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new BlockItemsASTNode(); }
#line 1629 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 40:
#line 231 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-1].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BlockItemsASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1637 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 43:
#line 243 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-3].node_ptr)), ptr2((yyvsp[-1].node_ptr));
            (yyval.node_ptr) = new AssignASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1645 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 44:
#line 247 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = nullptr; }
#line 1651 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 47:
#line 251 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new IfASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1659 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 48:
#line 255 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-4].node_ptr)), ptr2((yyvsp[-2].node_ptr)), ptr3((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new IfASTNode(std::move(ptr1), std::move(ptr2), std::move(ptr3));
          }
#line 1667 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 49:
#line 259 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new WhileASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1675 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 50:
#line 263 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new BreakASTNode(); }
#line 1681 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 51:
#line 265 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new ContinueASTNode(); }
#line 1687 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 52:
#line 267 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new ReturnASTNode(); }
#line 1693 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 53:
#line 269 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-1].node_ptr));
            (yyval.node_ptr) = new ReturnASTNode(std::move(ptr1));
          }
#line 1701 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 56:
#line 284 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-1].str_val));
            ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new LValASTNode(ident, std::move(ptr));
          }
#line 1710 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 57:
#line 292 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new IndexListASTNode(); }
#line 1716 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 58:
#line 294 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-3].node_ptr)), ptr2((yyvsp[-1].node_ptr));
            (yyval.node_ptr) = new IndexListASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1724 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 59:
#line 301 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = (yyvsp[-1].node_ptr); }
#line 1730 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 61:
#line 304 "parser.y" /* yacc.c:1652  */
    { (yyval.node_ptr) = new IntASTNode((yyvsp[0].int_val)); }
#line 1736 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 63:
#line 310 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-2].str_val));
            (yyval.node_ptr) = new FunCallASTNode(yylineno, ident);
          }
#line 1744 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 64:
#line 314 "parser.y" /* yacc.c:1652  */
    { std::string ident((yyvsp[-3].str_val));
            ASTNodePtr ptr((yyvsp[-1].node_ptr));
            (yyval.node_ptr) = new FunCallASTNode(yylineno, ident, std::move(ptr));
          }
#line 1753 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 65:
#line 319 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new UnaryOpASTNode(Operator::kAdd, std::move(ptr));
          }
#line 1761 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 66:
#line 323 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new UnaryOpASTNode(Operator::kSub, std::move(ptr));
          }
#line 1769 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 67:
#line 327 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new UnaryOpASTNode(Operator::kNot, std::move(ptr));
          }
#line 1777 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 68:
#line 334 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncRParamsASTNode(std::move(ptr1));
          }
#line 1785 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 69:
#line 338 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new FuncRParamsASTNode(std::move(ptr1), std::move(ptr2));
          }
#line 1793 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 71:
#line 346 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kMul, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1802 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 72:
#line 351 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kDiv, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1811 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 73:
#line 356 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kMod, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1820 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 75:
#line 365 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kAdd, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1829 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 76:
#line 370 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kSub, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1838 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 78:
#line 379 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kLess, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1847 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 79:
#line 384 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kGreater, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1856 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 80:
#line 389 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kLessEq, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1865 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 81:
#line 394 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kGreaterEq, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1874 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 83:
#line 403 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kEq, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1883 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 84:
#line 408 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kNeq, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1892 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 86:
#line 417 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kAnd, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1901 "parser.tab.cpp" /* yacc.c:1652  */
    break;

  case 88:
#line 426 "parser.y" /* yacc.c:1652  */
    { ASTNodePtr ptr1((yyvsp[-2].node_ptr)), ptr2((yyvsp[0].node_ptr));
            (yyval.node_ptr) = new BinaryExpASTNode(Operator::kOr, 
                std::move(ptr1), std::move(ptr2));
          }
#line 1910 "parser.tab.cpp" /* yacc.c:1652  */
    break;


#line 1914 "parser.tab.cpp" /* yacc.c:1652  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (root_ptr_ptr, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (root_ptr_ptr, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, root_ptr_ptr);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, root_ptr_ptr);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (root_ptr_ptr, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, root_ptr_ptr);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, root_ptr_ptr);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 436 "parser.y" /* yacc.c:1918  */


void
yyerror (BaseASTNode **root_ptr_ptr, const char *s)
{
    fprintf(stderr, "%s\n", s);
}
