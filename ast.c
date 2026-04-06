#include "ast.h"

#include <stdlib.h>
#include <string.h>

static char *ast_dup_string(const char *s) {
    size_t n;
    char *copy;

    if (s == NULL) {
        return NULL;
    }

    n = strlen(s) + 1;
    copy = (char *)malloc(n);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, s, n);
    return copy;
}

ASTNode *ast_new(NodeType type) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    node->nodeType = type;
    node->line = 0;
    return node;
}

ASTNode *ast_make_int(int value) {
    ASTNode *node = ast_new(NODE_INT);
    if (node != NULL) {
        node->data.ival = value;
    }
    return node;
}

ASTNode *ast_make_float(float value) {
    ASTNode *node = ast_new(NODE_FLOAT);
    if (node != NULL) {
        node->data.fval = value;
    }
    return node;
}

ASTNode *ast_make_string(char *value) {
    ASTNode *node = ast_new(NODE_STRING);
    if (node != NULL) {
        node->data.sval = value;
    }
    return node;
}

ASTNode *ast_make_identifier(char *name) {
    ASTNode *node = ast_new(NODE_IDENTIFIER);
    if (node != NULL) {
        node->data.sval = name;
    }
    return node;
}

ASTNode *ast_make_bool(int value) {
    ASTNode *node = ast_new(NODE_BOOL);
    if (node != NULL) {
        node->data.bval = value ? 1 : 0;
    }
    return node;
}

ASTNode *ast_make_binary(const char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = ast_new(NODE_BINARY);
    if (node == NULL) {
        return NULL;
    }

    strncpy(node->data.binop.op, op, sizeof(node->data.binop.op) - 1);
    node->data.binop.op[sizeof(node->data.binop.op) - 1] = '\0';
    node->data.binop.left = left;
    node->data.binop.right = right;
    return node;
}

ASTNode *ast_make_var_decl(char *name, int is_const, ASTNode *value_expr) {
    ASTNode *node = ast_new(NODE_VAR_DECL);
    if (node != NULL) {
        node->data.var_decl.var_name = name;
        node->data.var_decl.is_const = is_const;
        node->data.var_decl.value_expr = value_expr;
    }
    return node;
}

ASTNode *ast_make_assign(char *name, ASTNode *value) {
    ASTNode *node = ast_new(NODE_ASSIGN);
    if (node != NULL) {
        node->data.assign.name = name;
        node->data.assign.value = value;
    }
    return node;
}

ASTNode *ast_make_if(ASTNode *condition, ASTNode *if_body, ASTNode *else_body) {
    ASTNode *node = ast_new(NODE_IF);
    if (node != NULL) {
        node->data.if_stmt.condition = condition;
        node->data.if_stmt.if_body = if_body;
        node->data.if_stmt.else_body = else_body;
    }
    return node;
}

ASTNode *ast_make_for(ASTNode *init, ASTNode *condition, ASTNode *update, ASTNode *body) {
    ASTNode *node = ast_new(NODE_FOR);
    if (node != NULL) {
        node->data.for_loop.init = init;
        node->data.for_loop.condition = condition;
        node->data.for_loop.update = update;
        node->data.for_loop.body = body;
    }
    return node;
}

ASTNode *ast_make_while(ASTNode *condition, ASTNode *body) {
    ASTNode *node = ast_new(NODE_WHILE);
    if (node != NULL) {
        node->data.for_loop.condition = condition;
        node->data.for_loop.body = body;
    }
    return node;
}

ASTNode *ast_make_print(ASTNode *expr) {
    ASTNode *node = ast_new(NODE_PRINT);
    if (node != NULL) {
        node->data.unary.expr = expr;
    }
    return node;
}

ASTNode *ast_make_print_inline(ASTNode *expr) {
    ASTNode *node = ast_new(NODE_PRINT_INLINE);
    if (node != NULL) {
        node->data.unary.expr = expr;
    }
    return node;
}

ASTNode *ast_make_return(ASTNode *expr) {
    ASTNode *node = ast_new(NODE_RETURN);
    if (node != NULL) {
        node->data.unary.expr = expr;
    }
    return node;
}

ASTNode *ast_make_block(void) {
    return ast_new(NODE_BLOCK);
}

ASTNode *ast_block_append(ASTNode *block, ASTNode *item) {
    ASTNode **new_items;
    size_t new_count;

    if (block == NULL) {
        block = ast_make_block();
        if (block == NULL) {
            return NULL;
        }
    }

    if (block->nodeType != NODE_BLOCK) {
        return block;
    }

    new_count = block->data.block.count + 1;
    new_items = (ASTNode **)realloc(block->data.block.items, new_count * sizeof(ASTNode *));
    if (new_items == NULL) {
        return block;
    }

    block->data.block.items = new_items;
    block->data.block.items[block->data.block.count] = item;
    block->data.block.count = new_count;
    return block;
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
            ast_free(node->data.for_loop.condition);
            ast_free(node->data.for_loop.body);
            break;

        case NODE_PRINT:
        case NODE_PRINT_INLINE:
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
