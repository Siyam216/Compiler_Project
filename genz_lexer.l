%{
#include <stdlib.h>
#include <string.h>

#include "genz_parser.tab.h"
%}

%option noyywrap
%option yylineno

DIGIT       [0-9]
ID          [a-zA-Z_][a-zA-Z0-9_]*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+\.{DIGIT}+
STRING      \"([^\"\\\n]|\\.)*\"

%x COMMENT

%%

[ \t\r\n]+      ;
"//"[^\n]*      ;
"/*"             { BEGIN(COMMENT); }
<COMMENT>"*/"    { BEGIN(INITIAL); }
<COMMENT>\n      ;
<COMMENT>.       ;

"bet"            { return KEYWORD_BET; }
"fr"             { return KEYWORD_FR; }
"era"            { return KEYWORD_ERA; }
"vibeCheck"      { return KEYWORD_VIBECHECK; }
"aint"           { return KEYWORD_AINT; }
"meh"            { return KEYWORD_MEH; }
"loopin"         { return KEYWORD_LOOPIN; }
"vibe"           { return KEYWORD_VIBE; }
"yeet"           { return KEYWORD_YEET; }
"spill_tea"      { return KEYWORD_SPILL_TEA; }
"checkIt"        { return KEYWORD_CHECKIT; }
"OG"             { return KEYWORD_OG; }

"glowUp"         { return KEYWORD_GLOWUP; }
"fallOff"        { return KEYWORD_FALLOFF; }
"ratio"          { return KEYWORD_RATIO; }

"sus"            { return KEYWORD_SUS; }
"mainCharacter"  { return KEYWORD_MAINCHARACTER; }
"npc"            { return KEYWORD_NPC; }

"inty"           { return TYPE_INTY; }
"stringy"        { return TYPE_STRINGY; }
"noCap"          { return VAL_TRUE; }
"cap"            { return VAL_FALSE; }

{FLOAT}           { yylval.fval = (float)atof(yytext); return FLOAT_LITERAL; }
{INTEGER}         { yylval.ival = atoi(yytext); return INT_LITERAL; }
{STRING}          { yylval.sval = strdup(yytext); return STRING_CONTENT; }
{ID}              { yylval.sval = strdup(yytext); return IDENTIFIER; }

"=="             { return OPERATOR_EQ; }
"!="             { return OPERATOR_NEQ; }
"<="             { return OPERATOR_LTE; }
">="             { return OPERATOR_GTE; }
"&&"             { return OPERATOR_AND; }
"||"             { return OPERATOR_OR; }
"!"              { return OPERATOR_NOT; }
"+"              { return OPERATOR_PLUS; }
"-"              { return OPERATOR_MINUS; }
"*"              { return OPERATOR_MUL; }
"/"              { return OPERATOR_DIV; }
"%"              { return OPERATOR_MOD; }
"="              { return OPERATOR_ASSIGN; }
"<"              { return OPERATOR_LT; }
">"              { return OPERATOR_GT; }

"#"              { return '#'; }
"("              { return '('; }
")"              { return ')'; }
"{"              { return '{'; }
"}"              { return '}'; }
";"              { return ';'; }
","              { return ','; }

.                { return yytext[0]; }

%%
