#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int value;
    int is_const;
    int in_use;
} Symbol;

#define MAX_SYMBOLS 256

static Symbol g_symbols[MAX_SYMBOLS];

static char *dup_string(const char *s) {
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

static Symbol *find_symbol(const char *name) {
    int i;

    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (g_symbols[i].in_use && strcmp(g_symbols[i].name, name) == 0) {
            return &g_symbols[i];
        }
    }

    return NULL;
}

static Symbol *upsert_symbol(const char *name) {
    int i;
    Symbol *sym = find_symbol(name);

    if (sym != NULL) {
        return sym;
    }

    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (!g_symbols[i].in_use) {
            g_symbols[i].name = dup_string(name);
            if (g_symbols[i].name == NULL) {
                return NULL;
            }
            g_symbols[i].in_use = 1;
            g_symbols[i].value = 0;
            g_symbols[i].is_const = 0;
            return &g_symbols[i];
        }
    }

    return NULL;
}

int evaluate(ASTNode *node) {
    int left;
    int right;
    size_t i;
    Symbol *sym;

    if (node == NULL) {
        return 0;
    }

    switch (node->nodeType) {
        case NODE_INT:
            return node->data.ival;

        case NODE_FLOAT:
            return (int)node->data.fval;

        case NODE_BOOL:
            return node->data.bval ? 1 : 0;

        case NODE_NULL:
            return 0;

        case NODE_IDENTIFIER:
            sym = find_symbol(node->data.sval);
            return (sym != NULL) ? sym->value : 0;

        case NODE_VAR_DECL:
            sym = upsert_symbol(node->data.var_decl.var_name);
            if (sym == NULL) {
                return 0;
            }
            sym->is_const = node->data.var_decl.is_const;
            sym->value = evaluate(node->data.var_decl.value_expr);
            return sym->value;

        case NODE_ASSIGN:
            sym = upsert_symbol(node->data.assign.name);
            if (sym == NULL || sym->is_const) {
                return 0;
            }
            sym->value = evaluate(node->data.assign.value);
            return sym->value;

        case NODE_BINARY:
            left = evaluate(node->data.binop.left);
            right = evaluate(node->data.binop.right);

            if (strcmp(node->data.binop.op, "+") == 0) return left + right;
            if (strcmp(node->data.binop.op, "-") == 0) return left - right;
            if (strcmp(node->data.binop.op, "*") == 0) return left * right;
            if (strcmp(node->data.binop.op, "/") == 0) return (right != 0) ? (left / right) : 0;
            if (strcmp(node->data.binop.op, "%") == 0) return (right != 0) ? (left % right) : 0;
            if (strcmp(node->data.binop.op, "==") == 0) return left == right;
            if (strcmp(node->data.binop.op, "!=") == 0) return left != right;
            if (strcmp(node->data.binop.op, "<") == 0) return left < right;
            if (strcmp(node->data.binop.op, "<=") == 0) return left <= right;
            if (strcmp(node->data.binop.op, ">") == 0) return left > right;
            if (strcmp(node->data.binop.op, ">=") == 0) return left >= right;
            if (strcmp(node->data.binop.op, "&&") == 0) return (left && right) ? 1 : 0;
            if (strcmp(node->data.binop.op, "||") == 0) return (left || right) ? 1 : 0;
            return 0;

        case NODE_PRINT:
            if (node->data.unary.expr != NULL && node->data.unary.expr->nodeType == NODE_STRING) {
                printf("%s\n", node->data.unary.expr->data.sval);
                return 0;
            }
            printf("%d\n", evaluate(node->data.unary.expr));
            return 0;

        case NODE_IF:
            if (evaluate(node->data.if_stmt.condition)) {
                return evaluate(node->data.if_stmt.if_body);
            }
            return evaluate(node->data.if_stmt.else_body);

        case NODE_FOR:
            for (evaluate(node->data.for_loop.init);
                 evaluate(node->data.for_loop.condition);
                 evaluate(node->data.for_loop.update)) {
                evaluate(node->data.for_loop.body);
            }
            return 0;

        case NODE_WHILE:
            while (evaluate(node->data.for_loop.condition)) {
                evaluate(node->data.for_loop.body);
            }
            return 0;

        case NODE_BLOCK:
            for (i = 0; i < node->data.block.count; i++) {
                evaluate(node->data.block.items[i]);
            }
            return 0;

        case NODE_RETURN:
            return evaluate(node->data.unary.expr);

        case NODE_STRING:
        case NODE_INPUT:
        default:
            return 0;
    }
}

void eval_reset_symbols(void) {
    int i;

    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (g_symbols[i].in_use) {
            free(g_symbols[i].name);
            g_symbols[i].name = NULL;
        }
        g_symbols[i].in_use = 0;
        g_symbols[i].value = 0;
        g_symbols[i].is_const = 0;
    }
}
