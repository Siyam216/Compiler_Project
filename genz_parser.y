%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
extern int yylineno;

void yyerror(const char *s);
%}

%union {
    int ival;
    float fval;
    char cval;
    char *sval;
}

%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <sval> IDENTIFIER STRING_CONTENT HEX_LITERAL BIN_LITERAL
%token <cval> CHAR_LITERAL CHAR_ESCAPE

%token KEYWORD_BET KEYWORD_FR KEYWORD_ERA KEYWORD_VIBECHECK KEYWORD_AINT KEYWORD_MEH
%token KEYWORD_LOOPIN KEYWORD_YEET KEYWORD_SPILL_TEA KEYWORD_NOCAP KEYWORD_CAP
%token KEYWORD_SQUAD KEYWORD_GHOST KEYWORD_SKIP KEYWORD_SLIDEIN KEYWORD_CLOUT
%token KEYWORD_LEVELUP KEYWORD_NPC KEYWORD_TEST KEYWORD_CAUGHT_IN_4K KEYWORD_SLAYY
%token KEYWORD_LOWKEY KEYWORD_HIGHKEY KEYWORD_OG KEYWORD_CHECKIT KEYWORD_SUS
%token KEYWORD_MAIN_CHARACTER KEYWORD_MAINCHARACTER KEYWORD_GLOWUP KEYWORD_FALLOFF
%token KEYWORD_RATIO KEYWORD_VIBE KEYWORD_BUSSIN KEYWORD_MID KEYWORD_ATE KEYWORD_RIZZ

%token TYPE_INTY TYPE_STRINGY TYPE_BOOLY TYPE_FLOATY TYPE_LISTY TYPE_MAPPY
%token VAL_TRUE VAL_FALSE

%token OPERATOR_EQ OPERATOR_NEQ OPERATOR_LTE OPERATOR_GTE OPERATOR_AND OPERATOR_OR
%token OPERATOR_NOT OPERATOR_INCR OPERATOR_DECR OPERATOR_ADD_ASSIGN OPERATOR_SUB_ASSIGN
%token OPERATOR_MUL_ASSIGN OPERATOR_DIV_ASSIGN OPERATOR_MOD_ASSIGN OPERATOR_PLUS
%token OPERATOR_MINUS OPERATOR_MUL OPERATOR_DIV OPERATOR_MOD OPERATOR_ASSIGN
%token OPERATOR_LT OPERATOR_GT

%token SYMBOL_SPREAD SYMBOL_ARROW SYMBOL_FOR_SEP SYMBOL_DOT SYMBOL_COMMA SYMBOL_COLON
%token SYMBOL_SEMI SYMBOL_LPAREN SYMBOL_RPAREN SYMBOL_LBRACE SYMBOL_RBRACE
%token SYMBOL_LBRACKET SYMBOL_RBRACKET

%left OPERATOR_OR
%left OPERATOR_AND
%left OPERATOR_EQ OPERATOR_NEQ
%left OPERATOR_LT OPERATOR_LTE OPERATOR_GT OPERATOR_GTE
%left OPERATOR_PLUS OPERATOR_MINUS
%left OPERATOR_MUL OPERATOR_DIV OPERATOR_MOD KEYWORD_RATIO
%right OPERATOR_NOT
%nonassoc LOWER_THAN_SEMI
%nonassoc ';' SYMBOL_SEMI

%%

program
    : statement_list
    ;

statement_list
    :
    | statement_list statement
    ;

statement
    : var_decl optional_semi
    | assignment optional_semi
    | print_stmt optional_semi
    | input_stmt optional_semi
    | return_stmt optional_semi
    | if_stmt
    | while_stmt
    | loopin_stmt
    | block
    ;

optional_semi
    : /* empty */ %prec LOWER_THAN_SEMI
    | ';'
    | SYMBOL_SEMI
    ;

block
    : '{' statement_list '}'
    ;

var_decl
    : decl_kw IDENTIFIER
    | decl_kw IDENTIFIER '=' expr
    | decl_kw IDENTIFIER OPERATOR_ASSIGN expr
    ;

decl_kw
    : KEYWORD_BET
    | KEYWORD_FR
    | TYPE_INTY
    | TYPE_STRINGY
    | TYPE_BOOLY
    | TYPE_FLOATY
    ;

assignment
    : IDENTIFIER '=' expr
    | IDENTIFIER OPERATOR_ASSIGN expr
    ;

if_stmt
    : KEYWORD_VIBECHECK '(' expr ')' block if_tail
    ;

if_tail
    :
    | KEYWORD_AINT '(' expr ')' block if_tail
    | KEYWORD_MEH block
    ;

while_stmt
    : KEYWORD_VIBE '(' expr ')' block
    ;

loopin_stmt
    : KEYWORD_LOOPIN for_init '#' expr '#' for_update block
    ;

for_init
    :
    | var_decl
    | assignment
    ;

for_update
    :
    | assignment
    | KEYWORD_GLOWUP IDENTIFIER
    | KEYWORD_FALLOFF IDENTIFIER
    ;

print_stmt
    : KEYWORD_SPILL_TEA '(' expr ')'
    ;

input_stmt
    : KEYWORD_CHECKIT '(' IDENTIFIER ')'
    ;

return_stmt
    : KEYWORD_YEET expr
    ;

expr
    : expr OPERATOR_OR expr
    | expr OPERATOR_AND expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_NEQ expr
    | expr OPERATOR_LT expr
    | expr OPERATOR_LTE expr
    | expr OPERATOR_GT expr
    | expr OPERATOR_GTE expr
    | expr OPERATOR_PLUS expr
    | expr OPERATOR_MINUS expr
    | expr OPERATOR_MUL expr
    | expr OPERATOR_DIV expr
    | expr OPERATOR_MOD expr
    | expr KEYWORD_RATIO expr
    | OPERATOR_NOT expr
    | '(' expr ')'
    | literal
    | IDENTIFIER
    ;

literal
    : INT_LITERAL
    | FLOAT_LITERAL
    | STRING_CONTENT
    | HEX_LITERAL
    | BIN_LITERAL
    | CHAR_LITERAL
    | CHAR_ESCAPE
    | KEYWORD_NOCAP
    | KEYWORD_CAP
    | VAL_TRUE
    | VAL_FALSE
    | KEYWORD_NPC
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}

int main(void) {
    if (yyparse() == 0) {
        printf("Parsing completed successfully.\n");
        return 0;
    }
    return 1;
}
