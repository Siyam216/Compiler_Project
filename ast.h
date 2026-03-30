#ifndef AST_H
#define AST_H

#include <stddef.h>

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
    NODE_INPUT,
    NODE_RETURN,
    NODE_BLOCK
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeType nodeType;
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
void ast_free(ASTNode *node);

#endif
