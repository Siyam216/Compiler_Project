
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

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 19 "genz_parser.y"

#include "ast.h"



/* Line 1676 of yacc.c  */
#line 46 "genz_parser.tab.h"

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
     KEYWORD_BET = 262,
     KEYWORD_FR = 263,
     KEYWORD_ERA = 264,
     KEYWORD_VIBECHECK = 265,
     KEYWORD_AINT = 266,
     KEYWORD_MEH = 267,
     KEYWORD_LOOPIN = 268,
     KEYWORD_VIBE = 269,
     KEYWORD_YEET = 270,
     KEYWORD_SPILL_TEA = 271,
     KEYWORD_CHECKIT = 272,
     KEYWORD_OG = 273,
     KEYWORD_GLOWUP = 274,
     KEYWORD_FALLOFF = 275,
     KEYWORD_RATIO = 276,
     KEYWORD_SUS = 277,
     KEYWORD_MAINCHARACTER = 278,
     KEYWORD_NPC = 279,
     TYPE_INTY = 280,
     TYPE_STRINGY = 281,
     VAL_TRUE = 282,
     VAL_FALSE = 283,
     KEYWORD_NOCAP = 284,
     KEYWORD_CAP = 285,
     OPERATOR_EQ = 286,
     OPERATOR_NEQ = 287,
     OPERATOR_LTE = 288,
     OPERATOR_GTE = 289,
     OPERATOR_AND = 290,
     OPERATOR_OR = 291,
     OPERATOR_NOT = 292,
     OPERATOR_PLUS = 293,
     OPERATOR_MINUS = 294,
     OPERATOR_MUL = 295,
     OPERATOR_DIV = 296,
     OPERATOR_MOD = 297,
     OPERATOR_ASSIGN = 298,
     OPERATOR_LT = 299,
     OPERATOR_GT = 300
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 23 "genz_parser.y"

    int ival;
    float fval;
    char *sval;
    ASTNode *node;



/* Line 1676 of yacc.c  */
#line 117 "genz_parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


