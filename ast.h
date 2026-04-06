#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef struct ASTNode ASTNode;

/* Core node kinds used by the parser/interpreter pipeline. */
typedef enum {
    NODE_INT,
    NODE_FLOAT,
    NODE_STRING,
    NODE_IDENTIFIER,
    NODE_BOOL,
    NODE_NULL,
    NODE_BINARY,
    NODE_VAR_DECL,
    NODE_ASSIGN,
    NODE_IF,
    NODE_FOR,
    NODE_WHILE,
    NODE_PRINT,
    NODE_PRINT_INLINE,
    NODE_INPUT,
    NODE_RETURN,
    NODE_BLOCK
} NodeType;

struct ASTNode {
    NodeType nodeType;
    int line;
    union {
        int ival;
        float fval;
        char *sval;
        int bval;
        struct {
            char op[8];
            ASTNode *left;
            ASTNode *right;
        } binop;
        struct {
            ASTNode *condition;
            ASTNode *if_body;
            ASTNode *else_body;
        } if_stmt;
        struct {
            ASTNode *init;
            ASTNode *condition;
            ASTNode *update;
            ASTNode *body;
        } for_loop;
        struct {
            char *var_name;
            int is_const;
            ASTNode *value_expr;
        } var_decl;
        struct {
            char *name;
            ASTNode *value;
        } assign;
        struct {
            ASTNode **items;
            size_t count;
        } block;
        struct {
            ASTNode *expr;
        } unary;
    } data;
};

ASTNode *ast_new(NodeType type);
ASTNode *ast_make_int(int value);
ASTNode *ast_make_float(float value);
ASTNode *ast_make_string(char *value);
ASTNode *ast_make_identifier(char *name);
ASTNode *ast_make_bool(int value);
ASTNode *ast_make_binary(const char *op, ASTNode *left, ASTNode *right);
ASTNode *ast_make_var_decl(char *name, int is_const, ASTNode *value_expr);
ASTNode *ast_make_assign(char *name, ASTNode *value);
ASTNode *ast_make_if(ASTNode *condition, ASTNode *if_body, ASTNode *else_body);
ASTNode *ast_make_for(ASTNode *init, ASTNode *condition, ASTNode *update, ASTNode *body);
ASTNode *ast_make_while(ASTNode *condition, ASTNode *body);
ASTNode *ast_make_print(ASTNode *expr);
ASTNode *ast_make_print_inline(ASTNode *expr);
ASTNode *ast_make_return(ASTNode *expr);
ASTNode *ast_make_block(void);
ASTNode *ast_block_append(ASTNode *block, ASTNode *item);
void ast_free(ASTNode *node);

#endif
