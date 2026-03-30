
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT_LITERAL = 258,
     FLOAT_LITERAL = 259,
     IDENTIFIER = 260,
     STRING_CONTENT = 261,
     HEX_LITERAL = 262,
     BIN_LITERAL = 263,
     CHAR_LITERAL = 264,
     CHAR_ESCAPE = 265,
     KEYWORD_BET = 266,
     KEYWORD_FR = 267,
     KEYWORD_ERA = 268,
     KEYWORD_VIBECHECK = 269,
     KEYWORD_AINT = 270,
     KEYWORD_MEH = 271,
     KEYWORD_LOOPIN = 272,
     KEYWORD_YEET = 273,
     KEYWORD_SPILL_TEA = 274,
     KEYWORD_NOCAP = 275,
     KEYWORD_CAP = 276,
     KEYWORD_SQUAD = 277,
     KEYWORD_GHOST = 278,
     KEYWORD_SKIP = 279,
     KEYWORD_SLIDEIN = 280,
     KEYWORD_CLOUT = 281,
     KEYWORD_LEVELUP = 282,
     KEYWORD_NPC = 283,
     KEYWORD_TEST = 284,
     KEYWORD_CAUGHT_IN_4K = 285,
     KEYWORD_SLAYY = 286,
     KEYWORD_LOWKEY = 287,
     KEYWORD_HIGHKEY = 288,
     KEYWORD_OG = 289,
     KEYWORD_CHECKIT = 290,
     KEYWORD_SUS = 291,
     KEYWORD_MAIN_CHARACTER = 292,
     KEYWORD_MAINCHARACTER = 293,
     KEYWORD_GLOWUP = 294,
     KEYWORD_FALLOFF = 295,
     KEYWORD_RATIO = 296,
     KEYWORD_VIBE = 297,
     KEYWORD_BUSSIN = 298,
     KEYWORD_MID = 299,
     KEYWORD_ATE = 300,
     KEYWORD_RIZZ = 301,
     TYPE_INTY = 302,
     TYPE_STRINGY = 303,
     TYPE_BOOLY = 304,
     TYPE_FLOATY = 305,
     TYPE_LISTY = 306,
     TYPE_MAPPY = 307,
     VAL_TRUE = 308,
     VAL_FALSE = 309,
     OPERATOR_EQ = 310,
     OPERATOR_NEQ = 311,
     OPERATOR_LTE = 312,
     OPERATOR_GTE = 313,
     OPERATOR_AND = 314,
     OPERATOR_OR = 315,
     OPERATOR_NOT = 316,
     OPERATOR_INCR = 317,
     OPERATOR_DECR = 318,
     OPERATOR_ADD_ASSIGN = 319,
     OPERATOR_SUB_ASSIGN = 320,
     OPERATOR_MUL_ASSIGN = 321,
     OPERATOR_DIV_ASSIGN = 322,
     OPERATOR_MOD_ASSIGN = 323,
     OPERATOR_PLUS = 324,
     OPERATOR_MINUS = 325,
     OPERATOR_MUL = 326,
     OPERATOR_DIV = 327,
     OPERATOR_MOD = 328,
     OPERATOR_ASSIGN = 329,
     OPERATOR_LT = 330,
     OPERATOR_GT = 331,
     SYMBOL_SPREAD = 332,
     SYMBOL_ARROW = 333,
     SYMBOL_FOR_SEP = 334,
     SYMBOL_DOT = 335,
     SYMBOL_COMMA = 336,
     SYMBOL_COLON = 337,
     SYMBOL_SEMI = 338,
     SYMBOL_LPAREN = 339,
     SYMBOL_RPAREN = 340,
     SYMBOL_LBRACE = 341,
     SYMBOL_RBRACE = 342,
     SYMBOL_LBRACKET = 343,
     SYMBOL_RBRACKET = 344,
     LOWER_THAN_SEMI = 345
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 11 "genz_parser.y"

    int ival;
    float fval;
    char cval;
    char *sval;



/* Line 1676 of yacc.c  */
#line 151 "genz_parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


