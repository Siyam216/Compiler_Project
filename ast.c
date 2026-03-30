#include "ast.h"

#include <stdlib.h>

ASTNode *ast_new(NodeType type) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    node->nodeType = type;
    return node;
}

static void ast_free_children(ASTNode *node) {
    size_t i;

    if (node == NULL) {
        return;
    }

    switch (node->nodeType) {
        case NODE_BINARY:
            ast_free(node->data.binop.left);
            ast_free(node->data.binop.right);
            break;

        case NODE_IF:
            ast_free(node->data.if_stmt.condition);
            ast_free(node->data.if_stmt.if_body);
            ast_free(node->data.if_stmt.else_body);
            break;

        case NODE_FOR:
            ast_free(node->data.for_loop.init);
            ast_free(node->data.for_loop.condition);
            ast_free(node->data.for_loop.update);
            ast_free(node->data.for_loop.body);
            break;

        case NODE_WHILE:
        case NODE_PRINT:
        case NODE_INPUT:
        case NODE_RETURN:
            ast_free(node->data.unary.expr);
            break;

        case NODE_VAR_DECL:
            free(node->data.var_decl.var_name);
            ast_free(node->data.var_decl.value_expr);
            break;

        case NODE_ASSIGN:
            free(node->data.assign.name);
            ast_free(node->data.assign.value);
            break;

        case NODE_BLOCK:
            for (i = 0; i < node->data.block.count; i++) {
                ast_free(node->data.block.items[i]);
            }
            free(node->data.block.items);
            break;

        case NODE_STRING:
        case NODE_IDENTIFIER:
            free(node->data.sval);
            break;

        default:
            break;
    }
}

void ast_free(ASTNode *node) {
    if (node == NULL) {
        return;
    }
    ast_free_children(node);
    free(node);
}
