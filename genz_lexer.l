%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line = 1;
int token_count = 0;
FILE *out = NULL;

typedef union {
    int ival;
    float fval;
    char cval;
    char *sval;
} YYSTYPE;

YYSTYPE yylval;

void write_token(char* type, char* text) {
    if (out) {
        fprintf(out, "Line %-4d: %-25s -> %s\n", line, type, text);
    }
    printf("Line %-4d: %-25s -> %s\n", line, type, text);
    token_count++;
}

void write_error(char* msg, char* text) {
    if (out) {
        fprintf(out, "Line %-4d: %-25s -> %s: %s\n", line, "ERROR", msg, text);
    }
    printf("Line %-4d: %-25s -> %s: %s\n", line, "ERROR", msg, text);
}
%}

%option noyywrap
%option yylineno

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          [a-zA-Z_][a-zA-Z0-9_]*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+\.{DIGIT}+
HEX         0[xX][0-9a-fA-F]+
BINARY      0[bB][01]+

%x COMMENT
%x STRING

%%

[ \t\r]+    ;  /* Ignore whitespace */
\n          { line++; }

 /* ----- KEYWORDS ----- */
"bet"           { write_token("KEYWORD_BET", yytext); }
"fr"            { write_token("KEYWORD_FR", yytext); }
"era"           { write_token("KEYWORD_ERA", yytext); }
"vibeCheck"     { write_token("KEYWORD_VIBECHECK", yytext); }
"aint"          { write_token("KEYWORD_AINT", yytext); }
"meh"           { write_token("KEYWORD_MEH", yytext); }
"loopin"        { write_token("KEYWORD_LOOPIN", yytext); }
"yeet"          { write_token("KEYWORD_YEET", yytext); }
"spill_tea"     { write_token("KEYWORD_SPILL_TEA", yytext); }
"noCap"         { write_token("KEYWORD_NOCAP", yytext); }
"cap"           { write_token("KEYWORD_CAP", yytext); }
"squad"         { write_token("KEYWORD_SQUAD", yytext); }
"ghost"         { write_token("KEYWORD_GHOST", yytext); }
"skip"          { write_token("KEYWORD_SKIP", yytext); }
"slidein"       { write_token("KEYWORD_SLIDEIN", yytext); }
"clout"         { write_token("KEYWORD_CLOUT", yytext); }
"levelUp"       { write_token("KEYWORD_LEVELUP", yytext); }
"npc"           { write_token("KEYWORD_NPC", yytext); }
"test"          { write_token("KEYWORD_TEST", yytext); }
"caughtIn4k"    { write_token("KEYWORD_CAUGHT_IN_4K", yytext); }
"slayy"         { write_token("KEYWORD_SLAYY", yytext); }
"lowkey"        { write_token("KEYWORD_LOWKEY", yytext); }
"highkey"       { write_token("KEYWORD_HIGHKEY", yytext); }
"OG"            { write_token("KEYWORD_OG", yytext); }
"checkIt"       { write_token("KEYWORD_CHECKIT", yytext); }

"sus"           { write_token("KEYWORD_SUS", yytext); }
"mainCharacter" { write_token("KEYWORD_MAIN_CHARACTER", yytext); }
"glowUp"        { write_token("KEYWORD_GLOWUP", yytext); }
"fallOff"       { write_token("KEYWORD_FALLOFF", yytext); }
"ratio"         { write_token("KEYWORD_RATIO", yytext); }
"vibe"          { write_token("KEYWORD_VIBE", yytext); }
"bussin"        { write_token("KEYWORD_BUSSIN", yytext); }
"mid"           { write_token("KEYWORD_MID", yytext); }
"ate"           { write_token("KEYWORD_ATE", yytext); }
"rizz"          { write_token("KEYWORD_RIZZ", yytext); }

"inty"          { write_token("TYPE_INTY", yytext); }
"stringy"       { write_token("TYPE_STRINGY", yytext); }
"booly"         { write_token("TYPE_BOOLY", yytext); }
"floaty"        { write_token("TYPE_FLOATY", yytext); }
"listy"         { write_token("TYPE_LISTY", yytext); }
"mappy"         { write_token("TYPE_MAPPY", yytext); }

 /* ----- LITERALS ----- */
{HEX}           { write_token("HEX_LITERAL", yytext); }
{BINARY}        { write_token("BIN_LITERAL", yytext); }
{INTEGER}       { 
                  write_token("INT_LITERAL", yytext); 
                  yylval.ival = atoi(yytext);
                }
{FLOAT}         { 
                  write_token("FLOAT_LITERAL", yytext); 
                  yylval.fval = atof(yytext);
                }

\"              { 
                  BEGIN(STRING); 
                  write_token("STRING_START", "\""); 
                }
<STRING>{
  \"            { 
                  BEGIN(INITIAL); 
                  write_token("STRING_END", "\""); 
                }
  \\[\\'"nrt]   { write_token("ESCAPE_SEQ", yytext); }
  [^"\\\n]+     { write_token("STRING_CONTENT", yytext); }
  \n            { 
                  write_error("Unterminated string", ""); 
                  BEGIN(INITIAL); 
                  line++;
                }
}

\'[^'\n]\'      { 
                  write_token("CHAR_LITERAL", yytext); 
                  yylval.cval = yytext[1];
                }
\'\\[\\'"nrt]\' { 
                  write_token("CHAR_ESCAPE", yytext); 
                  switch(yytext[2]) {
                    case 'n': yylval.cval = '\n'; break;
                    case 't': yylval.cval = '\t'; break;
                    case 'r': yylval.cval = '\r'; break;
                    case '\\': yylval.cval = '\\'; break;
                    case '\'': yylval.cval = '\''; break;
                    case '"': yylval.cval = '"'; break;
                  }
                }

 /* ----- IDENTIFIERS ----- */
{ID}            { 
                  write_token("IDENTIFIER", yytext); 
                  yylval.sval = strdup(yytext);
                }

 /* ----- OPERATORS ----- */
"=="            { write_token("OPERATOR_EQ", yytext); }
"!="            { write_token("OPERATOR_NEQ", yytext); }
"<="            { write_token("OPERATOR_LTE", yytext); }
">="            { write_token("OPERATOR_GTE", yytext); }
"&&"            { write_token("OPERATOR_AND", yytext); }
"||"            { write_token("OPERATOR_OR", yytext); }
"!"             { write_token("OPERATOR_NOT", yytext); }
"++"            { write_token("OPERATOR_INCR", yytext); }
"--"            { write_token("OPERATOR_DECR", yytext); }
"+="            { write_token("OPERATOR_ADD_ASSIGN", yytext); }
"-="            { write_token("OPERATOR_SUB_ASSIGN", yytext); }
"*="            { write_token("OPERATOR_MUL_ASSIGN", yytext); }
"/="            { write_token("OPERATOR_DIV_ASSIGN", yytext); }
"%="            { write_token("OPERATOR_MOD_ASSIGN", yytext); }
"+"             { write_token("OPERATOR_PLUS", yytext); }
"-"             { write_token("OPERATOR_MINUS", yytext); }
"*"             { write_token("OPERATOR_MUL", yytext); }
"/"             { write_token("OPERATOR_DIV", yytext); }
"%"             { write_token("OPERATOR_MOD", yytext); }
"="             { write_token("OPERATOR_ASSIGN", yytext); }
"<"             { write_token("OPERATOR_LT", yytext); }
">"             { write_token("OPERATOR_GT", yytext); }

 /* ----- SPECIAL SYMBOLS ----- */
"..."           { write_token("SYMBOL_SPREAD", yytext); }
"=>"            { write_token("SYMBOL_ARROW", yytext); }
"#"             { write_token("SYMBOL_FOR_SEP", yytext); }
"."             { write_token("SYMBOL_DOT", yytext); }
","             { write_token("SYMBOL_COMMA", yytext); }
":"             { write_token("SYMBOL_COLON", yytext); }
";"             { write_token("SYMBOL_SEMI", yytext); }
"("             { write_token("SYMBOL_LPAREN", yytext); }
")"             { write_token("SYMBOL_RPAREN", yytext); }
"{"             { write_token("SYMBOL_LBRACE", yytext); }
"}"             { write_token("SYMBOL_RBRACE", yytext); }
"["             { write_token("SYMBOL_LBRACKET", yytext); }
"]"             { write_token("SYMBOL_RBRACKET", yytext); }

 /* ----- COMMENTS ----- */
"//".*          { write_token("COMMENT_SINGLE", yytext); }

"/*"            { 
                  write_token("COMMENT_ML_START", "/*"); 
                  BEGIN(COMMENT); 
                }
<COMMENT>"*/"   { 
                  write_token("COMMENT_ML_END", "*/"); 
                  BEGIN(INITIAL); 
                }
<COMMENT>\n     { line++; }
<COMMENT>.      ;

 /* ----- ERROR ----- */
.               { write_error("Unexpected character", yytext); }

%%

int main() {
    // Open output file
    out = fopen("output.txt", "w");
    if (!out) {
        printf("Error: Cannot open output.txt\n");
        return 1;
    }
    
    // Write header
    fprintf(out, "========================================\n");
    fprintf(out, "       GEN Z COMPILER - TOKENS         \n");
    fprintf(out, "========================================\n\n");
    
    // Check if input.txt exists
    FILE *in = fopen("input.txt", "r");
    if (!in) {
        printf("Creating sample input.txt...\n");
        in = fopen("input.txt", "w");
        if (!in) {
            printf("Error: Cannot create input.txt\n");
            fclose(out);
            return 1;
        }
        
        // Create sample GenZ code
        fprintf(in, "// Sample GenZ Program\n");
        fprintf(in, "slidein \"stdio.genz\"\n\n");
        fprintf(in, "OG main() {\n");
        fprintf(in, "    // Variable declarations\n");
        fprintf(in, "    bet age = 25\n");
        fprintf(in, "    fr PI = 3.14159\n");
        fprintf(in, "    stringy name = \"GenZ\"\n\n");
        fprintf(in, "    // Conditional statement\n");
        fprintf(in, "    vibeCheck age >= 18 {\n");
        fprintf(in, "        spill_tea(name + \" is adult!\")\n");
        fprintf(in, "    }\n");
        fprintf(in, "    aint age >= 13 {\n");
        fprintf(in, "        spill_tea(\"Teenager\")\n");
        fprintf(in, "    }\n");
        fprintf(in, "    meh {\n");
        fprintf(in, "        spill_tea(\"Child\")\n");
        fprintf(in, "    }\n\n");
        fprintf(in, "    // Loop example\n");
        fprintf(in, "    loopin bet i = 0 # i < 5 # glowUp i {\n");
        fprintf(in, "        spill_tea(\"Count: \" + i)\n");
        fprintf(in, "    }\n\n");
        fprintf(in, "    // Array example\n");
        fprintf(in, "    squad scores = [95, 87, 92, 88]\n");
        fprintf(in, "    \n");
        fprintf(in, "    yeet 0\n");
        fprintf(in, "}\n");
        fclose(in);
        in = fopen("input.txt", "r");
    }
    
    if (!in) {
        printf("Error: Cannot open input.txt\n");
        fclose(out);
        return 1;
    }
    
    // Set input stream
    yyin = in;
    
    printf("Starting GenZ Compiler...\n");
    printf("Input: input.txt\n");
    printf("Output: output.txt\n");
    printf("========================================\n\n");
    
    // Start lexing
    yylex();
    
    // Write summary
    fprintf(out, "\n========================================\n");
    fprintf(out, "SUMMARY:\n");
    fprintf(out, "Total lines: %d\n", line);
    fprintf(out, "Total tokens: %d\n", token_count);
    fprintf(out, "========================================\n");
    
    // Close files
    fclose(in);
    fclose(out);
    
    printf("\nTokenization complete!\n");
    printf("Total tokens generated: %d\n", token_count);
    printf("Check 'output.txt' for detailed token list.\n\n");
    
    // Show sample of output
    printf("Sample of output.txt:\n");
    printf("----------------------------------------\n");
    system("head -20 output.txt 2>/dev/null || echo 'Could not display output'");
    
    return 0;
}