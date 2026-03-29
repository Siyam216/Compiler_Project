# Gen Z Compiler & Interpreter: Detailed Implementation Plan

This outline translates your complete Gen Z lexical syntax (from your `compiler.docx`, `genz_lexer.l` and `input.txt`) into a complete compiler pipeline (Lexer -> Parser -> AST -> Interpreter).

## 1. Project Organization
Structure your source code the same way standard compiler pipelines are built:
1.  **Lexical Analysis:** (`genz_lexer.l`) Tokenizes the `"vibeCheck"`, `"spill_tea"`, `"glowUp"`, etc.
2.  **Syntax Analysis:** (`genz_parser.y`) Defines the grammar rules of the Gen Z language using Bison.
3.  **Abstract Syntax Tree (AST):** (`ast.h`, `ast.c`) The runtime hierarchy map of the syntax.
4.  **Interpreter/Evaluator:** (`eval.c`) Code that walks the AST to execute conditions, loops, and math.

---

## 2. Phase 1: Refining the Lexer (`genz_lexer.l`)
You already have a fantastic lexer ready! Right now, it just prints out string matches using `write_token()`. 

**What needs to change:**
When integrating with Bison, your lexer shouldn't just *print* the token; it needs to *return* the token ID to Bison so the parser can process it.

*Example changes to make in `genz_lexer.l`:*
```c
"bet"           { return KEYWORD_BET; }        // int/float/char var
"fr"            { return KEYWORD_FR; }         // const var
"era"           { return KEYWORD_ERA; }        // void func()
"vibeCheck"     { return KEYWORD_VIBECHECK; }  // if
"aint"          { return KEYWORD_AINT; }       // else if
"meh"           { return KEYWORD_MEH; }        // else
"loopin"        { return KEYWORD_LOOPIN; }     // for loop
"vibe"          { return KEYWORD_VIBE; }       // while loop
"yeet"          { return KEYWORD_YEET; }       // return
"spill_tea"     { return KEYWORD_SPILL_TEA; }  // printf
"checkIt"       { return KEYWORD_CHECKIT; }    // scanf
"OG"            { return KEYWORD_OG; }         // main function

/* Operators & Modifiers */
"glowUp"        { return KEYWORD_GLOWUP; }     // ++
"fallOff"       { return KEYWORD_FALLOFF; }    // --
"ratio"         { return KEYWORD_RATIO; }      // %

/* Logic */
"sus"           { return KEYWORD_SUS; }        // assert()
"mainCharacter" { return KEYWORD_MAINCHARACTER;} // this/self
"npc"           { return KEYWORD_NPC; }        // NULL

/* Data Types */
"inty"          { return TYPE_INTY; }
"stringy"       { return TYPE_STRINGY; }
"noCap"         { return VAL_TRUE; }
"cap"           { return VAL_FALSE; }

{INTEGER}       { yylval.ival = atoi(yytext); return INT_LITERAL; }
{ID}            { yylval.sval = strdup(yytext); return IDENTIFIER; }
```

---

## 3. Phase 2: Building the Bison Parser (`genz_parser.y`)
This is where you define the grammar to make sense of the tokens. 

1. **Define the Tokens:** Ensure all keywords from the lexer are declared using `%token`.
2. **Handle Special Syntax:** Notice that `loopin` uses `#` as a separator instead of `;` (e.g., `loopin i=0#i<5#glowUp i`). You'll need to define `<statement> '#' <condition> '#' <statement>` in Bison.
3. **Write the Grammar Rules (BNF):**

*Example `genz_parser.y` snippet for your unique language:*
```bison
%{
#include "ast.h"
extern int yylex();
extern void yyerror(const char *s);
ASTNode* root; 
%}

%union {
    int ival;
    char* sval;
    struct ASTNode* node;
}

// Token Definitions
%token <ival> INT_LITERAL
%token <sval> IDENTIFIER STRING_CONTENT KEYWORD_BET KEYWORD_FR
%token KEYWORD_VIBECHECK KEYWORD_AINT KEYWORD_MEH KEYWORD_LOOPIN KEYWORD_VIBE
%token KEYWORD_SPILL_TEA KEYWORD_YEET KEYWORD_OG VAL_TRUE VAL_FALSE KEYWORD_GLOWUP

%type <node> program statement expr var_decl if_stmt loopin_stmt print_stmt

%%
program:
    statements { root = $1; }
    ;

statement:
    var_decl
    | if_stmt
    | loopin_stmt
    | print_stmt
    | expr
    ;

var_decl:
    // bet age = 25
    KEYWORD_BET IDENTIFIER '=' expr { $$ = create_var_decl($2, $4, 0); }
    // fr PI = 3.14
    | KEYWORD_FR IDENTIFIER '=' expr { $$ = create_var_decl($2, $4, 1); } 
    ;

if_stmt:
    // vibeCheck (condition) { statements }
    KEYWORD_VIBECHECK expr '{' statements '}' { $$ = create_if_node($2, $4); }
    ;

loopin_stmt:
    // loopin bet i = 0 # i < 5 # glowUp i { statements }
    KEYWORD_LOOPIN var_decl '#' expr '#' KEYWORD_GLOWUP IDENTIFIER '{' statements '}' { 
        $$ = create_for_node($2, $4, create_increment_node($7), $9); 
    }
    ;

print_stmt:
    // spill_tea(name + " is adult!")
    KEYWORD_SPILL_TEA '(' expr ')' { $$ = create_print_node($3); }
    ;
%%
```

---

## 4. Phase 3: Constructing the Abstract Syntax Tree (AST)
The AST is a tree data structure in C that captures the semantic meaning of the parsed rules.

**Steps:**
1.  **Create `ast.h`:** Define an `enum` of Node Types (e.g., `NODE_VAR_DECL`, `NODE_IF`, `NODE_FOR`, `NODE_PRINT`).
2.  **Create the Struct:** Build a recursive node structure covering the Gen Z types.
    ```c
    typedef struct ASTNode {
        int nodeType;
        union {
            int ival;            // For inty / booly (noCap/cap)
            char* sval;          // For Identifiers / stringy
            struct {             // Binary operations (+, -, ==)
                struct ASTNode* left;
                struct ASTNode* right;
            } binop;
            struct {             // vibeCheck / aint / meh
                struct ASTNode* condition;
                struct ASTNode* if_body;
                struct ASTNode* else_body;
            } if_stmt;
            struct {             // loopin
                struct ASTNode* init;
                struct ASTNode* condition;
                struct ASTNode* update;
                struct ASTNode* body;
            } for_loop;
            struct {             // bet / fr
                char* var_name;
                int is_const;    // 1 if 'fr', 0 if 'bet'
                struct ASTNode* value_expr;
            } var_decl;
        } data;
    } ASTNode;
    ```

---

## 5. Phase 4: Implementation of the Interpreter/Evaluator
Once parsing is complete, Bison gives you the `root` `ASTNode*`. You evaluate the program by recursively walking down this tree.

**Steps (`eval.c`):**
1.  **Create a Symbol Table:** A map storing `{ variable_name, value, is_const }`.
2.  **Write the Evaluation Function:**
    ```c
    int evaluate(ASTNode* node) {
        if (!node) return 0;
        
        switch (node->nodeType) {
            case NODE_INT: return node->data.ival;
                
            case NODE_PRINT: // spill_tea
                // Add logic based on string/int combination
                return 0;
                
            case NODE_ADD:
                return evaluate(node->data.binop.left) + evaluate(node->data.binop.right);
                
            case NODE_IF: // vibeCheck
                if (evaluate(node->data.if_stmt.condition)) {
                    evaluate(node->data.if_stmt.if_body);
                } else if (node->data.if_stmt.else_body) {
                    evaluate(node->data.if_stmt.else_body); // Handle aint/meh
                }
                return 0;
                
            case NODE_FOR: // loopin
                for (evaluate(node->data.for_loop.init);
                     evaluate(node->data.for_loop.condition);
                     evaluate(node->data.for_loop.update)) {
                     evaluate(node->data.for_loop.body);
                }
                return 0;
        }
    }
    ```

---

## 6. Commands to Compile & Build
To build all the pieces into your final compiler:

```bash
# 1. Generate the Parser 
bison -d genz_parser.y

# 2. Generate the Lexer 
flex genz_lexer.l

# 3. Compile the whole project
gcc genz_parser.tab.c lex.yy.c ast.c eval.c -o genz_compiler -lfl

# 4. Run the Compiler against your Gen Z file
./genz_compiler input.txt
```