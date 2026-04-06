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
int eval_get_error_count(void);
void yyerror(const char *s);

ASTNode *root = NULL;

#define MARK_NODE_LINE(n) do { if ((n) != NULL) (n)->line = yylineno; } while (0)
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
    : statement { $$ = ast_make_block(); MARK_NODE_LINE($$); $$ = ast_block_append($$, $1); }
    | statement_list statement { $$ = ast_block_append($1, $2); MARK_NODE_LINE($$); }
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
    : '{' statement_list '}' { $$ = $2; MARK_NODE_LINE($$); }
    ;

var_decl
    : KEYWORD_BET IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_var_decl($2, 0, $4); MARK_NODE_LINE($$); }
    | KEYWORD_FR IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_var_decl($2, 1, $4); MARK_NODE_LINE($$); }
    | TYPE_INTY IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_var_decl($2, 0, $4); MARK_NODE_LINE($$); }
    | TYPE_STRINGY IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_var_decl($2, 0, $4); MARK_NODE_LINE($$); }
    ;

assignment
    : IDENTIFIER OPERATOR_ASSIGN expr { $$ = ast_make_assign($1, $3); MARK_NODE_LINE($$); }
    ;

if_stmt
    : KEYWORD_VIBECHECK '(' expr ')' block if_tail { $$ = ast_make_if($3, $5, $6); MARK_NODE_LINE($$); }
    ;

if_tail
    : { $$ = NULL; }
    | KEYWORD_AINT '(' expr ')' block if_tail { $$ = ast_make_if($3, $5, $6); MARK_NODE_LINE($$); }
    | KEYWORD_MEH block { $$ = $2; MARK_NODE_LINE($$); }
    ;

while_stmt
    : KEYWORD_VIBE '(' expr ')' block { $$ = ast_make_while($3, $5); MARK_NODE_LINE($$); }
    ;

loopin_stmt
    : KEYWORD_LOOPIN for_init '#' expr '#' for_update block { $$ = ast_make_for($2, $4, $6, $7); MARK_NODE_LINE($$); }
    ;

for_init
    : var_decl { $$ = $1; }
    | assignment { $$ = $1; }
    | { $$ = NULL; }
    ;

for_update
    : assignment { $$ = $1; MARK_NODE_LINE($$); }
    | KEYWORD_GLOWUP IDENTIFIER { $$ = ast_make_assign($2, ast_make_binary("+", ast_make_identifier($2), ast_make_int(1))); MARK_NODE_LINE($$); }
    | KEYWORD_FALLOFF IDENTIFIER { $$ = ast_make_assign($2, ast_make_binary("-", ast_make_identifier($2), ast_make_int(1))); MARK_NODE_LINE($$); }
    | { $$ = NULL; }
    ;

print_stmt
    : KEYWORD_SPILL_TEA '(' expr ')' { $$ = ast_make_print($3); MARK_NODE_LINE($$); }
    | KEYWORD_CHECKIT '(' expr ')' { $$ = ast_make_print_inline($3); MARK_NODE_LINE($$); }
    ;

return_stmt
    : KEYWORD_YEET expr { $$ = ast_make_return($2); MARK_NODE_LINE($$); }
    ;

expr
    : expr OPERATOR_OR expr { $$ = ast_make_binary("||", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_AND expr { $$ = ast_make_binary("&&", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_EQ expr { $$ = ast_make_binary("==", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_NEQ expr { $$ = ast_make_binary("!=", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_LT expr { $$ = ast_make_binary("<", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_LTE expr { $$ = ast_make_binary("<=", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_GT expr { $$ = ast_make_binary(">", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_GTE expr { $$ = ast_make_binary(">=", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_PLUS expr { $$ = ast_make_binary("+", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_MINUS expr { $$ = ast_make_binary("-", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_MUL expr { $$ = ast_make_binary("*", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_DIV expr { $$ = ast_make_binary("/", $1, $3); MARK_NODE_LINE($$); }
    | expr OPERATOR_MOD expr { $$ = ast_make_binary("%", $1, $3); MARK_NODE_LINE($$); }
    | expr KEYWORD_RATIO expr { $$ = ast_make_binary("%", $1, $3); MARK_NODE_LINE($$); }
    | OPERATOR_NOT expr { $$ = ast_make_binary("==", $2, ast_make_int(0)); MARK_NODE_LINE($$); }
    | '(' expr ')' { $$ = $2; MARK_NODE_LINE($$); }
    | literal { $$ = $1; MARK_NODE_LINE($$); }
    | IDENTIFIER { $$ = ast_make_identifier($1); MARK_NODE_LINE($$); }
    ;

literal
    : INT_LITERAL { $$ = ast_make_int($1); MARK_NODE_LINE($$); }
    | FLOAT_LITERAL { $$ = ast_make_float($1); MARK_NODE_LINE($$); }
    | STRING_CONTENT { $$ = ast_make_string($1); MARK_NODE_LINE($$); }
    | KEYWORD_NOCAP { $$ = ast_make_bool(1); MARK_NODE_LINE($$); }
    | KEYWORD_CAP { $$ = ast_make_bool(0); MARK_NODE_LINE($$); }
    | VAL_TRUE { $$ = ast_make_bool(1); MARK_NODE_LINE($$); }
    | VAL_FALSE { $$ = ast_make_bool(0); MARK_NODE_LINE($$); }
    | KEYWORD_NPC { $$ = ast_new(NODE_NULL); MARK_NODE_LINE($$); }
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
        printf("Semantic error count: %d\n", eval_get_error_count());
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
