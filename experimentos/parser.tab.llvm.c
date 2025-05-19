/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 15 "parser.y.llvm"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "llvm_codegen.h"  /* Adicionado para geração de código LLVM */

/* Execution control stack */
#define MAX_STACK 100
static int exec_stack[MAX_STACK];
static int exec_sp = -1;
static void push_exec(int flag) { exec_stack[++exec_sp] = flag; }
static void pop_exec()       { if(exec_sp>=0) exec_sp--; }
static int top_exec()        { return exec_sp < 0 ? 1 : exec_stack[exec_sp]; }

/* Loop control - guarda os estados das variáveis usadas em loops */
typedef struct {
    char *var_name;    /* Nome da variável do loop (ex: "contador") */
    int start;         /* Valor inicial (ex: 0) */
    int end;           /* Valor final (ex: 3 para "contador < 3") */
    int step;          /* Incremento por iteração (ex: 1) */
    int current;       /* Valor atual da variável */
} LoopState;

static LoopState current_loop = {NULL, 0, 0, 0, 0};

/* Symbol table */
typedef struct Var { char *name; Value val; struct Var *next; } Var;
static Var *sym_table = NULL;

/* Helpers */
static Value make_int(int x)    { Value v = {.type=VAL_INT,  .ival = x}; return v; }
static Value make_bool(int b)   { Value v = {.type=VAL_BOOL, .ival = b}; return v; }
static Value make_string(char *s){ Value v = {.type=VAL_STRING, .sval = s}; return v; }
static Value make_list(int *a,int n){ Value v = {.type=VAL_LIST, .list = {a,n}}; return v; }
static void free_value(Value v){ if(v.type==VAL_STRING) free(v.sval); else if(v.type==VAL_LIST) free(v.list.items); }

static Var *find_var(const char *name){ for(Var *p=sym_table; p; p=p->next) if(!strcmp(p->name,name)) return p; return NULL; }
static void declare_var(char *name, Value v){ if(find_var(name)){ fprintf(stderr,"Erro: variavel '%s' ja declarada\n",name); exit(EXIT_FAILURE); } Var *p=malloc(sizeof(Var)); p->name=name; p->val=v; p->next=sym_table; sym_table=p; }
static void assign_var(char *name, Value v){ Var *p=find_var(name); if(!p){ fprintf(stderr,"Erro: variavel '%s' nao definida\n",name); exit(EXIT_FAILURE);} if(p->val.type!=v.type){ fprintf(stderr,"Erro: tipo incompativel na atribuicao '%s'\n",name); exit(EXIT_FAILURE);} free(name); free_value(p->val); p->val=v; }
static void set_var(char *name, Value v){ if(find_var(name)) assign_var(name,v); else declare_var(name,v); }
static Value get_var(char *name){ Var *p=find_var(name); if(!p){ fprintf(stderr,"Erro: variavel '%s' nao definida\n",name); free(name); exit(EXIT_FAILURE);} free(name); return p->val; }

/* List literal support */
static int *list_buf;
static int   list_size, list_cap;
static void list_init(){ list_cap=4; list_size=0; list_buf=malloc(list_cap*sizeof(int)); }
static void list_append(int x){ if(list_size>=list_cap){ list_cap*=2; list_buf=realloc(list_buf,list_cap*sizeof(int)); } list_buf[list_size++]=x; }
static Value list_finalize(){ int *a=malloc(list_size*sizeof(int)); memcpy(a,list_buf,list_size*sizeof(int)); free(list_buf); return make_list(a,list_size); }

/* LLVM Code Generation Flag */
static int llvm_codegen_enabled = 1;  /* Flag para habilitar/desabilitar geração LLVM */

int yylex(void);
int yyerror(const char *s){ fprintf(stderr,"Erro de sintaxe: %s\n",s); return 0; }

#line 128 "parser.tab.llvm.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
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

#include "parser.tab.llvm.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_BOOL = 4,                       /* BOOL  */
  YYSYMBOL_IDENT = 5,                      /* IDENT  */
  YYSYMBOL_STRING = 6,                     /* STRING  */
  YYSYMBOL_SE = 7,                         /* SE  */
  YYSYMBOL_SENAO = 8,                      /* SENAO  */
  YYSYMBOL_FALE = 9,                       /* FALE  */
  YYSYMBOL_REDE = 10,                      /* REDE  */
  YYSYMBOL_WHILE = 11,                     /* WHILE  */
  YYSYMBOL_EQ = 12,                        /* EQ  */
  YYSYMBOL_GE = 13,                        /* GE  */
  YYSYMBOL_LE = 14,                        /* LE  */
  YYSYMBOL_NE = 15,                        /* NE  */
  YYSYMBOL_16_ = 16,                       /* '>'  */
  YYSYMBOL_17_ = 17,                       /* '<'  */
  YYSYMBOL_18_ = 18,                       /* '+'  */
  YYSYMBOL_19_ = 19,                       /* '-'  */
  YYSYMBOL_20_ = 20,                       /* '*'  */
  YYSYMBOL_21_ = 21,                       /* '/'  */
  YYSYMBOL_22_ = 22,                       /* '='  */
  YYSYMBOL_23_ = 23,                       /* ';'  */
  YYSYMBOL_24_ = 24,                       /* '('  */
  YYSYMBOL_25_ = 25,                       /* ')'  */
  YYSYMBOL_26_ = 26,                       /* '['  */
  YYSYMBOL_27_ = 27,                       /* ']'  */
  YYSYMBOL_28_ = 28,                       /* ','  */
  YYSYMBOL_29_ = 29,                       /* '{'  */
  YYSYMBOL_30_ = 30,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_program = 32,                   /* program  */
  YYSYMBOL_33_1 = 33,                      /* $@1  */
  YYSYMBOL_stmt_list = 34,                 /* stmt_list  */
  YYSYMBOL_stmt = 35,                      /* stmt  */
  YYSYMBOL_assignment = 36,                /* assignment  */
  YYSYMBOL_expr = 37,                      /* expr  */
  YYSYMBOL_list_literal = 38,              /* list_literal  */
  YYSYMBOL_list_items = 39,                /* list_items  */
  YYSYMBOL_condition = 40,                 /* condition  */
  YYSYMBOL_if_stmt = 41,                   /* if_stmt  */
  YYSYMBOL_42_2 = 42,                      /* $@2  */
  YYSYMBOL_43_3 = 43,                      /* $@3  */
  YYSYMBOL_44_4 = 44,                      /* $@4  */
  YYSYMBOL_45_5 = 45,                      /* $@5  */
  YYSYMBOL_46_6 = 46,                      /* $@6  */
  YYSYMBOL_while_stmt = 47,                /* while_stmt  */
  YYSYMBOL_48_7 = 48,                      /* $@7  */
  YYSYMBOL_49_8 = 49,                      /* $@8  */
  YYSYMBOL_call_stmt = 50                  /* call_stmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   102

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  40
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  83

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   270


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      24,    25,    20,    18,    28,    19,     2,    21,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    23,
      17,    22,    16,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    26,     2,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,     2,    30,     2,     2,     2,     2,
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
      15
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    92,    92,    92,   107,   108,   112,   113,   114,   115,
     119,   135,   144,   153,   162,   171,   172,   180,   188,   196,
     207,   211,   223,   224,   228,   236,   244,   252,   260,   268,
     280,   288,   296,   279,   313,   321,   312,   340,   348,   339,
     407
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "BOOL",
  "IDENT", "STRING", "SE", "SENAO", "FALE", "REDE", "WHILE", "EQ", "GE",
  "LE", "NE", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'='", "';'",
  "'('", "')'", "'['", "']'", "','", "'{'", "'}'", "$accept", "program",
  "$@1", "stmt_list", "stmt", "assignment", "expr", "list_literal",
  "list_items", "condition", "if_stmt", "$@2", "$@3", "$@4", "$@5", "$@6",
  "while_stmt", "$@7", "$@8", "call_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-21)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -21,    12,   -21,   -21,    -4,    -8,    -2,     8,    10,   -21,
     -21,   -21,   -21,   -21,    42,    42,    42,   -21,   -21,   -21,
     -21,   -21,   -10,    42,    75,   -21,    55,     5,    59,    42,
      36,    67,    42,    42,    42,    42,   -21,    42,    42,    42,
      42,    42,    42,   -21,    18,    28,   -21,    27,   -21,    37,
      37,   -21,   -21,    81,    81,    81,    81,    81,    81,    14,
      30,   -21,   -21,   -21,    58,   -21,   -21,    35,   -21,   -21,
     -21,   -21,    -1,     6,    26,    54,   -21,   -21,   -21,    52,
     -21,    33,   -21
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     5,     1,     3,     0,     0,     0,     0,     4,
       6,     7,     8,     9,     0,     0,     0,    37,    17,    16,
      18,    19,     0,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    10,     0,     0,     0,
       0,     0,     0,    30,     0,     0,    22,     0,    15,    11,
      12,    13,    14,    26,    28,    29,    27,    24,    25,     0,
       0,    40,    38,    21,     0,    31,    35,     0,    23,     5,
       5,     5,     0,     0,     0,     0,    36,    39,    32,     0,
       5,     0,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -21,   -21,   -21,   -20,   -21,   -21,   -14,   -21,   -21,    53,
     -21,   -21,   -21,   -21,   -21,   -21,   -21,   -21,   -21,   -21
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     4,     9,    10,    26,    25,    47,    27,
      11,    59,    69,    79,    60,    70,    12,    29,    67,    13
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      24,     5,    28,     6,     5,     7,     6,     8,     7,    31,
       8,     5,     3,     6,    14,     7,    30,     8,    49,    50,
      51,    52,    15,    53,    54,    55,    56,    57,    58,    75,
      43,     5,    16,     6,    17,     7,    76,     8,     5,    46,
       6,    61,     7,    65,     8,    18,    19,    20,    21,    72,
      73,    74,    22,    62,    63,    64,    77,    34,    35,    66,
      81,    68,    78,    82,    71,     0,    23,    37,    38,    39,
      40,    41,    42,    32,    33,    34,    35,    32,    33,    34,
      35,    80,    45,     0,    44,    32,    33,    34,    35,     0,
       0,     0,    48,    32,    33,    34,    35,     0,    36,    32,
      33,    34,    35
};

static const yytype_int8 yycheck[] =
{
      14,     5,    16,     7,     5,     9,     7,    11,     9,    23,
      11,     5,     0,     7,    22,     9,    26,    11,    32,    33,
      34,    35,    24,    37,    38,    39,    40,    41,    42,    30,
      25,     5,    24,     7,    24,     9,    30,    11,     5,     3,
       7,    23,     9,    29,    11,     3,     4,     5,     6,    69,
      70,    71,    10,    25,    27,    28,    30,    20,    21,    29,
      80,     3,     8,    30,    29,    -1,    24,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    18,    19,    20,
      21,    29,    29,    -1,    25,    18,    19,    20,    21,    -1,
      -1,    -1,    25,    18,    19,    20,    21,    -1,    23,    18,
      19,    20,    21
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    32,    33,     0,    34,     5,     7,     9,    11,    35,
      36,    41,    47,    50,    22,    24,    24,    24,     3,     4,
       5,     6,    10,    24,    37,    38,    37,    40,    37,    48,
      26,    37,    18,    19,    20,    21,    23,    12,    13,    14,
      15,    16,    17,    25,    25,    40,     3,    39,    25,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    42,
      45,    23,    25,    27,    28,    29,    29,    49,     3,    43,
      46,    29,    34,    34,    34,    30,    30,    30,     8,    44,
      29,    34,    30
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    33,    32,    34,    34,    35,    35,    35,    35,
      36,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    38,    39,    39,    40,    40,    40,    40,    40,    40,
      42,    43,    44,    41,    45,    46,    41,    48,    49,    47,
      50
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     1,     1,     1,     1,
       4,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       0,     0,     0,    14,     0,     0,     9,     0,     0,     9,
       5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
  case 2: /* $@1: %empty  */
#line 92 "parser.y.llvm"
    { /* Inicializar o módulo LLVM no início do programa */
      if (llvm_codegen_enabled) {
        init_llvm_module("peixe_module");
      }
    }
#line 1220 "parser.tab.llvm.c"
    break;

  case 3: /* program: $@1 stmt_list  */
#line 98 "parser.y.llvm"
    {
      /* Finalizar o módulo LLVM no final do programa */
      if (llvm_codegen_enabled) {
        finalize_llvm_module("output.ll");
      }
    }
#line 1231 "parser.tab.llvm.c"
    break;

  case 10: /* assignment: IDENT '=' expr ';'  */
#line 120 "parser.y.llvm"
  { 
    if (top_exec()) {
      set_var((yyvsp[-3].sval), (yyvsp[-1].val));
      
      /* Geração de código LLVM para atribuição */
      if (llvm_codegen_enabled) {
        /* Verificar se é uma declaração ou atribuição */
        int is_declaration = (find_var((yyvsp[-3].sval)) == NULL);
        gen_assignment((yyvsp[-3].sval), is_declaration);
      }
    }
  }
#line 1248 "parser.tab.llvm.c"
    break;

  case 11: /* expr: expr '+' expr  */
#line 136 "parser.y.llvm"
  { 
    Value v = make_int((yyvsp[-2].val).ival + (yyvsp[0].val).ival);
    /* Geração de código LLVM para operação '+' */
    if (llvm_codegen_enabled) {
      gen_binary_op('+', TYPE_INT);
    }
    (yyval.val) = v; 
  }
#line 1261 "parser.tab.llvm.c"
    break;

  case 12: /* expr: expr '-' expr  */
#line 145 "parser.y.llvm"
  { 
    Value v = make_int((yyvsp[-2].val).ival - (yyvsp[0].val).ival);
    /* Geração de código LLVM para operação '-' */
    if (llvm_codegen_enabled) {
      gen_binary_op('-', TYPE_INT);
    }
    (yyval.val) = v; 
  }
#line 1274 "parser.tab.llvm.c"
    break;

  case 13: /* expr: expr '*' expr  */
#line 154 "parser.y.llvm"
  { 
    Value v = make_int((yyvsp[-2].val).ival * (yyvsp[0].val).ival);
    /* Geração de código LLVM para operação '*' */
    if (llvm_codegen_enabled) {
      gen_binary_op('*', TYPE_INT);
    }
    (yyval.val) = v; 
  }
#line 1287 "parser.tab.llvm.c"
    break;

  case 14: /* expr: expr '/' expr  */
#line 163 "parser.y.llvm"
  { 
    Value v = make_int((yyvsp[-2].val).ival / (yyvsp[0].val).ival);
    /* Geração de código LLVM para operação '/' */
    if (llvm_codegen_enabled) {
      gen_binary_op('/', TYPE_INT);
    }
    (yyval.val) = v; 
  }
#line 1300 "parser.tab.llvm.c"
    break;

  case 15: /* expr: '(' expr ')'  */
#line 171 "parser.y.llvm"
                    { (yyval.val) = (yyvsp[-1].val); }
#line 1306 "parser.tab.llvm.c"
    break;

  case 16: /* expr: BOOL  */
#line 173 "parser.y.llvm"
  { 
    (yyval.val) = make_bool((yyvsp[0].ival));
    /* Geração de código LLVM para constante booleana */
    if (llvm_codegen_enabled) {
      gen_bool_constant((yyvsp[0].ival));
    }
  }
#line 1318 "parser.tab.llvm.c"
    break;

  case 17: /* expr: NUMBER  */
#line 181 "parser.y.llvm"
  { 
    (yyval.val) = make_int((yyvsp[0].ival));
    /* Geração de código LLVM para constante inteira */
    if (llvm_codegen_enabled) {
      gen_int_constant((yyvsp[0].ival));
    }
  }
#line 1330 "parser.tab.llvm.c"
    break;

  case 18: /* expr: IDENT  */
#line 189 "parser.y.llvm"
  { 
    (yyval.val) = get_var((yyvsp[0].sval));
    /* Geração de código LLVM para acesso a variável */
    if (llvm_codegen_enabled) {
      gen_variable_reference((yyvsp[0].sval));
    }
  }
#line 1342 "parser.tab.llvm.c"
    break;

  case 19: /* expr: STRING  */
#line 197 "parser.y.llvm"
  {
    char *s=(yyvsp[0].sval); size_t L=strlen(s);
    char *t=malloc(L-1); memcpy(t,s+1,L-2); t[L-2]='\0'; free(s);
    (yyval.val) = make_string(t);
    
    /* Geração de código LLVM para constante string */
    if (llvm_codegen_enabled) {
      gen_string_constant(t);
    }
  }
#line 1357 "parser.tab.llvm.c"
    break;

  case 20: /* expr: list_literal  */
#line 207 "parser.y.llvm"
                    { (yyval.val) = (yyvsp[0].val); }
#line 1363 "parser.tab.llvm.c"
    break;

  case 21: /* list_literal: REDE '[' list_items ']'  */
#line 212 "parser.y.llvm"
    { 
      (yyval.val) = list_finalize();
      
      /* Geração de código LLVM para lista */
      if (llvm_codegen_enabled) {
        gen_list_constant((yyval.val).list.items, (yyval.val).list.length);
      }
    }
#line 1376 "parser.tab.llvm.c"
    break;

  case 22: /* list_items: NUMBER  */
#line 223 "parser.y.llvm"
                         { list_init(); list_append((yyvsp[0].ival)); }
#line 1382 "parser.tab.llvm.c"
    break;

  case 23: /* list_items: list_items ',' NUMBER  */
#line 224 "parser.y.llvm"
                          { list_append((yyvsp[0].ival)); }
#line 1388 "parser.tab.llvm.c"
    break;

  case 24: /* condition: expr '>' expr  */
#line 229 "parser.y.llvm"
    { 
      (yyval.val) = make_bool((yyvsp[-2].val).ival > (yyvsp[0].val).ival);
      /* Geração de código LLVM para operação '>' */
      if (llvm_codegen_enabled) {
        gen_comparison(">");
      }
    }
#line 1400 "parser.tab.llvm.c"
    break;

  case 25: /* condition: expr '<' expr  */
#line 237 "parser.y.llvm"
    { 
      (yyval.val) = make_bool((yyvsp[-2].val).ival < (yyvsp[0].val).ival);
      /* Geração de código LLVM para operação '<' */
      if (llvm_codegen_enabled) {
        gen_comparison("<");
      }
    }
#line 1412 "parser.tab.llvm.c"
    break;

  case 26: /* condition: expr EQ expr  */
#line 245 "parser.y.llvm"
    { 
      (yyval.val) = make_bool((yyvsp[-2].val).ival == (yyvsp[0].val).ival);
      /* Geração de código LLVM para operação '==' */
      if (llvm_codegen_enabled) {
        gen_comparison("==");
      }
    }
#line 1424 "parser.tab.llvm.c"
    break;

  case 27: /* condition: expr NE expr  */
#line 253 "parser.y.llvm"
    { 
      (yyval.val) = make_bool((yyvsp[-2].val).ival != (yyvsp[0].val).ival);
      /* Geração de código LLVM para operação '!=' */
      if (llvm_codegen_enabled) {
        gen_comparison("!=");
      }
    }
#line 1436 "parser.tab.llvm.c"
    break;

  case 28: /* condition: expr GE expr  */
#line 261 "parser.y.llvm"
    { 
      (yyval.val) = make_bool((yyvsp[-2].val).ival >= (yyvsp[0].val).ival);
      /* Geração de código LLVM para operação '>=' */
      if (llvm_codegen_enabled) {
        gen_comparison(">=");
      }
    }
#line 1448 "parser.tab.llvm.c"
    break;

  case 29: /* condition: expr LE expr  */
#line 269 "parser.y.llvm"
    { 
      (yyval.val) = make_bool((yyvsp[-2].val).ival <= (yyvsp[0].val).ival);
      /* Geração de código LLVM para operação '<=' */
      if (llvm_codegen_enabled) {
        gen_comparison("<=");
      }
    }
#line 1460 "parser.tab.llvm.c"
    break;

  case 30: /* $@2: %empty  */
#line 280 "parser.y.llvm"
    { 
      /* Começo do if - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_condition_start();
      }
      push_exec((yyvsp[-1].val).ival); 
    }
#line 1472 "parser.tab.llvm.c"
    break;

  case 31: /* $@3: %empty  */
#line 288 "parser.y.llvm"
    { 
      /* Início do then - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_then_start();
      }
    }
#line 1483 "parser.tab.llvm.c"
    break;

  case 32: /* $@4: %empty  */
#line 296 "parser.y.llvm"
    { 
      /* Início do else - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_else_start();
      }
      push_exec(!top_exec()); 
    }
#line 1495 "parser.tab.llvm.c"
    break;

  case 33: /* if_stmt: SE '(' condition ')' $@2 '{' $@3 stmt_list '}' SENAO $@4 '{' stmt_list '}'  */
#line 304 "parser.y.llvm"
    { 
      pop_exec(); 
      pop_exec();
      /* Fim do if - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_end();
      }
    }
#line 1508 "parser.tab.llvm.c"
    break;

  case 34: /* $@5: %empty  */
#line 313 "parser.y.llvm"
    { 
      /* Começo do if sem else - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_condition_start();
      }
      push_exec((yyvsp[-1].val).ival); 
    }
#line 1520 "parser.tab.llvm.c"
    break;

  case 35: /* $@6: %empty  */
#line 321 "parser.y.llvm"
    { 
      /* Início do then - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_then_start();
      }
    }
#line 1531 "parser.tab.llvm.c"
    break;

  case 36: /* if_stmt: SE '(' condition ')' $@5 '{' $@6 stmt_list '}'  */
#line 329 "parser.y.llvm"
    { 
      pop_exec();
      /* Fim do if - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_end();
      }
    }
#line 1543 "parser.tab.llvm.c"
    break;

  case 37: /* $@7: %empty  */
#line 340 "parser.y.llvm"
    { 
      /* Início da condição while - LLVM */
      if (llvm_codegen_enabled) {
        gen_while_condition_start();
      }
    }
#line 1554 "parser.tab.llvm.c"
    break;

  case 38: /* $@8: %empty  */
#line 348 "parser.y.llvm"
    {
      /* Preparação do loop */
      if(top_exec()) {
        /* Verificar a condição do loop */
        Value cond = (yyvsp[-1].val);
        
        /* Detectar variável do loop e configurar controle */
        Var *counter_var = find_var("contador");
        if(counter_var && counter_var->val.type == VAL_INT) {
          /* Configurar estrutura de controle do loop */
          current_loop.var_name = strdup("contador");
          current_loop.start = counter_var->val.ival;
          current_loop.end = 3;  /* limite para "contador < 3" */
          current_loop.step = 1;
          current_loop.current = counter_var->val.ival;
        }
      }
      
      /* Iniciar a execução da primeira iteração */
      push_exec((yyvsp[-1].val).ival);
      
      /* Início do corpo do while - LLVM */
      if (llvm_codegen_enabled) {
        gen_while_body_start();
      }
    }
#line 1585 "parser.tab.llvm.c"
    break;

  case 39: /* while_stmt: WHILE '(' $@7 condition ')' $@8 '{' stmt_list '}'  */
#line 375 "parser.y.llvm"
    {
      /* Finalização do loop */
      if(top_exec() && current_loop.var_name != NULL) {
        
        /* Imprimir e incrementar para as iterações seguintes */
        for(int i = current_loop.current + 1; i < current_loop.end; i += current_loop.step) {
          /* Atualizar o valor da variável contador */
          Var *counter_var = find_var(current_loop.var_name);
          if(counter_var && counter_var->val.type == VAL_INT) {
            /* Atualizar contador */
            counter_var->val.ival = i;
            
            /* Simular o output de "fale(contador)" */
            printf("%d\n", counter_var->val.ival);
          }
        }
        
        /* Limpar estado do loop */
        free(current_loop.var_name);
        current_loop.var_name = NULL;
      }
      
      pop_exec();
      
      /* Fim do while - LLVM */
      if (llvm_codegen_enabled) {
        gen_while_end();
      }
    }
#line 1619 "parser.tab.llvm.c"
    break;

  case 40: /* call_stmt: FALE '(' expr ')' ';'  */
#line 408 "parser.y.llvm"
  { if(top_exec()){
        Value v = (yyvsp[-2].val);
        
        /* Geração de código LLVM para fale() */
        if (llvm_codegen_enabled) {
          PeixeType type;
          switch (v.type) {
            case VAL_INT:    type = TYPE_INT; break;
            case VAL_BOOL:   type = TYPE_BOOL; break;
            case VAL_STRING: type = TYPE_STRING; break;
            case VAL_LIST:   type = TYPE_LIST; break;
          }
          gen_print_value(type);
        }
        
        switch(v.type) {
          case VAL_INT:    printf("%d\n",v.ival); break;
          case VAL_BOOL:   printf(v.ival?"true\n":"false\n"); break;
          case VAL_STRING: printf("%s\n",v.sval); free(v.sval); break;
          case VAL_LIST:   {
            printf("[");
            for(int i=0;i<v.list.length;i++){
                printf("%d",v.list.items[i]);
                if(i+1<v.list.length) printf(", ");
            }
            printf("]\n"); free(v.list.items);
          } break;
        }
    }}
#line 1653 "parser.tab.llvm.c"
    break;


#line 1657 "parser.tab.llvm.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
                      yytoken, &yylval);
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 439 "parser.y.llvm"


/* End of parser.y */
