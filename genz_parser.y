%{
#include <stdio.h>
#include <stdlib.h>
#include <io.h>

#include "ast.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;

int evaluate(ASTNode *node);
void eval_reset_symbols(void);
void yyerror(const char *s);

ASTNode *root = NULL;
%}

%code requires {
#include "ast.h"
}

%union {
    int ival;
    float fval;
    char *sval;
    ASTNode *node;
}

%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <sval> IDENTIFIER STRING_CONTENT

%token KEYWORD_BET KEYWORD_FR KEYWORD_ERA KEYWORD_VIBECHECK KEYWORD_AINT KEYWORD_MEH
%token KEYWORD_LOOPIN KEYWORD_VIBE KEYWORD_YEET KEYWORD_SPILL_TEA KEYWORD_CHECKIT
%token KEYWORD_OG KEYWORD_GLOWUP KEYWORD_FALLOFF KEYWORD_RATIO KEYWORD_SUS
%token KEYWORD_MAINCHARACTER KEYWORD_NPC TYPE_INTY TYPE_STRINGY VAL_TRUE VAL_FALSE
%token KEYWORD_NOCAP KEYWORD_CAP

%token OPERATOR_EQ OPERATOR_NEQ OPERATOR_LTE OPERATOR_GTE OPERATOR_AND OPERATOR_OR
%token OPERATOR_NOT OPERATOR_PLUS OPERATOR_MINUS OPERATOR_MUL OPERATOR_DIV OPERATOR_MOD
%token OPERATOR_ASSIGN OPERATOR_LT OPERATOR_GT

%type <node> program statement_list statement var_decl assignment if_stmt if_tail while_stmt
%type <node> loopin_stmt for_init for_update print_stmt return_stmt block expr literal

%left OPERATOR_OR
%left OPERATOR_AND
%left OPERATOR_EQ OPERATOR_NEQ
%left OPERATOR_LT OPERATOR_LTE OPERATOR_GT OPERATOR_GTE
%left OPERATOR_PLUS OPERATOR_MINUS
%left OPERATOR_MUL OPERATOR_DIV OPERATOR_MOD KEYWORD_RATIO
%right OPERATOR_NOT

%%

program
    : statement_list { root = $1; }
    ;

statement_list
    : statement { $$ = ast_make_block(); $$ = ast_block_append($$, $1); }
    | statement_list statement { $$ = ast_block_append($1, $2); }
    ;

statement
    : var_decl ';' { $$ = $1; }
    | assignment ';' { $$ = $1; }
    | print_stmt ';' { $$ = $1; }
    | return_stmt ';' { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | loopin_stmt { $$ = $1; }
    | block { $$ = $1; }
    ;

block
    : '{' statement_list '}' { $$ = $2; }
    ;

var_decl
    : KEYWORD_BET IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_var_decl($2, 0, $4); }
    | KEYWORD_FR IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_var_decl($2, 1, $4); }
    | TYPE_INTY IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_var_decl($2, 0, $4); }
    | TYPE_STRINGY IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_var_decl($2, 0, $4); }
    ;

assignment
    : IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_assign($1, $3); }
    ;

if_stmt
    : KEYWORD_VIBECHECK '(' expr ')' block if_tail { $$ = ast_make_if($3, $5, $6); }
    ;

if_tail
    : { $$ = NULL; }
    | KEYWORD_AINT '(' expr ')' block if_tail { $$ = ast_make_if($3, $5, $6); }
    | KEYWORD_MEH block { $$ = $2; }
    ;

while_stmt
    : KEYWORD_VIBE '(' expr ')' block { $$ = ast_make_while($3, $5); }
    ;

loopin_stmt
    : KEYWORD_LOOPIN for_init '#' expr '#' for_update block { $$ = ast_make_for($2, $4, $6, $7); }
    ;

for_init
    : var_decl { $$ = $1; }
    | assignment { $$ = $1; }
    | { $$ = NULL; }
    ;

for_update
    : assignment { $$ = $1; }
    | KEYWORD_GLOWUP IDENTIFIER { $$ = ast_make_assign($2, ast_make_binary("+", ast_make_identifier($2), ast_make_int(1))); }
    | KEYWORD_FALLOFF IDENTIFIER { $$ = ast_make_assign($2, ast_make_binary("-", ast_make_identifier($2), ast_make_int(1))); }
    | { $$ = NULL; }
    ;

print_stmt
    : KEYWORD_SPILL_TEA '(' expr ')' { $$ = ast_make_print($3); }
    ;

return_stmt
    : KEYWORD_YEET expr { $$ = ast_make_return($2); }
    ;

expr
    : expr OPERATOR_OR expr { $$ = ast_make_binary("||", $1, $3); }
    | expr OPERATOR_AND expr { $$ = ast_make_binary("&&", $1, $3); }
    | expr OPERATOR_EQ expr { $$ = ast_make_binary("==", $1, $3); }
    | expr OPERATOR_NEQ expr { $$ = ast_make_binary("!=", $1, $3); }
    | expr OPERATOR_LT expr { $$ = ast_make_binary("<", $1, $3); }
    | expr OPERATOR_LTE expr { $$ = ast_make_binary("<=", $1, $3); }
    | expr OPERATOR_GT expr { $$ = ast_make_binary(">", $1, $3); }
    | expr OPERATOR_GTE expr { $$ = ast_make_binary(">=", $1, $3); }
    | expr OPERATOR_PLUS expr { $$ = ast_make_binary("+", $1, $3); }
    | expr OPERATOR_MINUS expr { $$ = ast_make_binary("-", $1, $3); }
    | expr OPERATOR_MUL expr { $$ = ast_make_binary("*", $1, $3); }
    | expr OPERATOR_DIV expr { $$ = ast_make_binary("/", $1, $3); }
    | expr OPERATOR_MOD expr { $$ = ast_make_binary("%", $1, $3); }
    | expr KEYWORD_RATIO expr { $$ = ast_make_binary("%", $1, $3); }
    | OPERATOR_NOT expr { $$ = ast_make_binary("==", $2, ast_make_int(0)); }
    | '(' expr ')' { $$ = $2; }
    | literal { $$ = $1; }
    | IDENTIFIER { $$ = ast_make_identifier($1); }
    ;

literal
    : INT_LITERAL { $$ = ast_make_int($1); }
    | FLOAT_LITERAL { $$ = ast_make_float($1); }
    | STRING_CONTENT { $$ = ast_make_string($1); }
    | KEYWORD_NOCAP { $$ = ast_make_bool(1); }
    | KEYWORD_CAP { $$ = ast_make_bool(0); }
    | VAL_TRUE { $$ = ast_make_bool(1); }
    | VAL_FALSE { $$ = ast_make_bool(0); }
    | KEYWORD_NPC { $$ = ast_new(NODE_NULL); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}

int main(void) {
    FILE *in = fopen("input.txt", "r");
    FILE *out = fopen("output.txt", "w");

    if (in == NULL) {
        fprintf(stderr, "Error: cannot open input.txt\n");
        return 1;
    }
    if (out == NULL) {
        fclose(in);
        fprintf(stderr, "Error: cannot open output.txt\n");
        return 1;
    }

    yyin = in;
    if (yyparse() != 0) {
        fclose(in);
        fclose(out);
        return 1;
    }

    /* Route evaluator print output to output.txt. */
    fflush(stdout);
    {
        int stdout_fd = _dup(_fileno(stdout));
        _dup2(_fileno(out), _fileno(stdout));
        evaluate(root);
        fflush(stdout);
        _dup2(stdout_fd, _fileno(stdout));
        _close(stdout_fd);
    }

    fclose(in);
    fclose(out);
    eval_reset_symbols();
    ast_free(root);
    return 0;
}
