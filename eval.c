 #include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct {
    char *name;
    int value;
    char *svalue;
    int has_string;
    int is_const;
    int scope_depth;
    int in_use;
} Symbol;

#define MAX_SYMBOLS 256

static Symbol g_symbols[MAX_SYMBOLS];
static int g_semantic_error_count = 0;
static int g_scope_depth = 0;

int evaluate(ASTNode *node);
static int is_string_expr(ASTNode *node);
static char *evaluate_string_expr(ASTNode *node);

static void report_semantic_error(int line, const char *fmt, ...) {
    va_list args;

    g_semantic_error_count++;
    printf("Semantic error at line %d: ", line > 0 ? line : 0);

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

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
    int depth;
    int i;

    for (depth = g_scope_depth; depth >= 0; depth--) {
        for (i = 0; i < MAX_SYMBOLS; i++) {
            if (g_symbols[i].in_use && g_symbols[i].scope_depth == depth && strcmp(g_symbols[i].name, name) == 0) {
                return &g_symbols[i];
            }
        }
    }

    return NULL;
}

static Symbol *find_symbol_in_current_scope(const char *name) {
    int i;

    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (g_symbols[i].in_use && g_symbols[i].scope_depth == g_scope_depth && strcmp(g_symbols[i].name, name) == 0) {
            return &g_symbols[i];
        }
    }

    return NULL;
}

static Symbol *declare_symbol(const char *name) {
    int i;

    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (!g_symbols[i].in_use) {
            g_symbols[i].name = dup_string(name);
            if (g_symbols[i].name == NULL) {
                return NULL;
            }
            g_symbols[i].in_use = 1;
            g_symbols[i].value = 0;
            g_symbols[i].svalue = NULL;
            g_symbols[i].has_string = 0;
            g_symbols[i].is_const = 0;
            g_symbols[i].scope_depth = g_scope_depth;
            return &g_symbols[i];
        }
    }

    return NULL;
}

static void push_scope(void) {
    g_scope_depth++;
}

static void pop_scope(void) {
    int i;

    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (g_symbols[i].in_use && g_symbols[i].scope_depth == g_scope_depth) {
            free(g_symbols[i].name);
            g_symbols[i].name = NULL;
            free(g_symbols[i].svalue);
            g_symbols[i].svalue = NULL;
            g_symbols[i].in_use = 0;
            g_symbols[i].value = 0;
            g_symbols[i].has_string = 0;
            g_symbols[i].is_const = 0;
            g_symbols[i].scope_depth = 0;
        }
    }

    if (g_scope_depth > 0) {
        g_scope_depth--;
    }
}

static void print_string_literal(const char *s) {
    size_t len;

    if (s == NULL) {
        return;
    }

    len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        printf("%.*s", (int)(len - 2), s + 1);
        return;
    }

    printf("%s", s);
}

static char *dup_unquoted_string_literal(const char *s) {
    size_t len;
    size_t out_len;
    char *copy;

    if (s == NULL) {
        return dup_string("");
    }

    len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        out_len = len - 2;
        copy = (char *)malloc(out_len + 1);
        if (copy == NULL) {
            return NULL;
        }
        memcpy(copy, s + 1, out_len);
        copy[out_len] = '\0';
        return copy;
    }

    return dup_string(s);
}

static char *int_to_string(int value) {
    char buffer[32];
    size_t n;
    char *copy;

    snprintf(buffer, sizeof(buffer), "%d", value);
    n = strlen(buffer) + 1;
    copy = (char *)malloc(n);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, buffer, n);
    return copy;
}

static char *concat_strings(const char *left, const char *right) {
    size_t left_len;
    size_t right_len;
    char *out;

    left_len = (left != NULL) ? strlen(left) : 0;
    right_len = (right != NULL) ? strlen(right) : 0;
    out = (char *)malloc(left_len + right_len + 1);
    if (out == NULL) {
        return NULL;
    }

    if (left_len > 0) {
        memcpy(out, left, left_len);
    }
    if (right_len > 0) {
        memcpy(out + left_len, right, right_len);
    }
    out[left_len + right_len] = '\0';
    return out;
}

static int is_string_expr(ASTNode *node) {
    Symbol *sym;

    if (node == NULL) {
        return 0;
    }

    if (node->nodeType == NODE_STRING) {
        return 1;
    }

    if (node->nodeType == NODE_IDENTIFIER) {
        sym = find_symbol(node->data.sval);
        return (sym != NULL && sym->has_string) ? 1 : 0;
    }

    if (node->nodeType == NODE_BINARY && strcmp(node->data.binop.op, "+") == 0) {
        return is_string_expr(node->data.binop.left) || is_string_expr(node->data.binop.right);
    }

    return 0;
}

static char *evaluate_string_expr(ASTNode *node) {
    char *left;
    char *right;
    char *result;
    Symbol *sym;

    if (node == NULL) {
        return dup_string("");
    }

    if (node->nodeType == NODE_STRING) {
        return dup_unquoted_string_literal(node->data.sval);
    }

    if (node->nodeType == NODE_IDENTIFIER) {
        sym = find_symbol(node->data.sval);
        if (sym != NULL && sym->has_string && sym->svalue != NULL) {
            return dup_string(sym->svalue);
        }
        return int_to_string(evaluate(node));
    }

    if (node->nodeType == NODE_BINARY && strcmp(node->data.binop.op, "+") == 0 &&
        (is_string_expr(node->data.binop.left) || is_string_expr(node->data.binop.right))) {
        left = evaluate_string_expr(node->data.binop.left);
        right = evaluate_string_expr(node->data.binop.right);
        result = concat_strings(left, right);
        free(left);
        free(right);
        return result;
    }

    return int_to_string(evaluate(node));
}

int evaluate(ASTNode *node) {
    int left;
    int right;
    size_t i;
    Symbol *sym;
    char *new_text;

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

        case NODE_STRING:
            return 0;

        case NODE_IDENTIFIER:
            sym = find_symbol(node->data.sval);
            if (sym == NULL) {
                report_semantic_error(node->line, "undeclared variable '%s'", node->data.sval);
                return 0;
            }
            return (sym != NULL) ? sym->value : 0;

        case NODE_VAR_DECL:
            sym = find_symbol_in_current_scope(node->data.var_decl.var_name);
            if (sym != NULL) {
                report_semantic_error(node->line, "duplicate declaration of variable '%s'", node->data.var_decl.var_name);
                return sym->value;
            }

            sym = declare_symbol(node->data.var_decl.var_name);
            if (sym == NULL) {
                return 0;
            }
            sym->is_const = node->data.var_decl.is_const;
            sym->value = evaluate(node->data.var_decl.value_expr);
            if (is_string_expr(node->data.var_decl.value_expr)) {
                free(sym->svalue);
                sym->svalue = evaluate_string_expr(node->data.var_decl.value_expr);
                sym->has_string = 1;
            } else {
                free(sym->svalue);
                sym->svalue = NULL;
                sym->has_string = 0;
            }
            return sym->value;

        case NODE_ASSIGN:
            sym = find_symbol(node->data.assign.name);
            if (sym == NULL) {
                report_semantic_error(node->line, "assignment to undeclared variable '%s'", node->data.assign.name);
                return 0;
            }
            if (sym->is_const) {
                report_semantic_error(node->line, "cannot reassign const variable '%s'", node->data.assign.name);
                return 0;
            }
            sym->value = evaluate(node->data.assign.value);
            if (is_string_expr(node->data.assign.value)) {
                new_text = evaluate_string_expr(node->data.assign.value);
                free(sym->svalue);
                sym->svalue = new_text;
                sym->has_string = 1;
            } else {
                free(sym->svalue);
                sym->svalue = NULL;
                sym->has_string = 0;
            }
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
            if (is_string_expr(node->data.unary.expr)) {
                char *text = evaluate_string_expr(node->data.unary.expr);
                if (text != NULL) {
                    printf("%s", text);
                }
                free(text);
                printf("\n");
            } else {
                printf("%d\n", evaluate(node->data.unary.expr));
            }
            return 0;

        case NODE_PRINT_INLINE:
            if (is_string_expr(node->data.unary.expr)) {
                char *text = evaluate_string_expr(node->data.unary.expr);
                if (text != NULL) {
                    printf("%s", text);
                }
                free(text);
            } else {
                printf("%d", evaluate(node->data.unary.expr));
            }
            return 0;

        case NODE_IF:
            if (evaluate(node->data.if_stmt.condition)) {
                return evaluate(node->data.if_stmt.if_body);
            }
            return evaluate(node->data.if_stmt.else_body);

        case NODE_FOR:
            push_scope();
            for (evaluate(node->data.for_loop.init);
                 evaluate(node->data.for_loop.condition);
                 evaluate(node->data.for_loop.update)) {
                evaluate(node->data.for_loop.body);
            }
            pop_scope();
            return 0;

        case NODE_WHILE:
            while (evaluate(node->data.for_loop.condition)) {
                evaluate(node->data.for_loop.body);
            }
            return 0;

        case NODE_BLOCK:
            push_scope();
            for (i = 0; i < node->data.block.count; i++) {
                evaluate(node->data.block.items[i]);
            }
            pop_scope();
            return 0;

        case NODE_RETURN:
            return evaluate(node->data.unary.expr);

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
            free(g_symbols[i].svalue);
            g_symbols[i].svalue = NULL;
        }
        g_symbols[i].in_use = 0;
        g_symbols[i].value = 0;
        g_symbols[i].has_string = 0;
        g_symbols[i].is_const = 0;
        g_symbols[i].scope_depth = 0;
    }

    g_scope_depth = 0;
    g_semantic_error_count = 0;
}

int eval_get_error_count(void) {
    return g_semantic_error_count;
}
