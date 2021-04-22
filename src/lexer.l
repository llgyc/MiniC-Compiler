%option noyywrap
%option noinput
%option nounput
%option yylineno

%{

#include <stdlib.h>

#include "context.hpp"
#include "parser.tab.h"

extern int yylex(void);
extern void yyerror(BaseASTNode **, const char *);

%}

IDENT               [_a-zA-Z][_a-zA-Z0-9]*
DECIMAL_CONST       [1-9][0-9]*
OCTAL_CONST         0[0-7]*
HEXADECIMAL_CONST   0[xX][0-9a-fA-F]+
INT_CONST           {DECIMAL_CONST}|{OCTAL_CONST}|{HEXADECIMAL_CONST}
LINE_COMMENT        \/\/.*
WHITESPACE          [ \t\v\n\f]

%x C_COMMENT

%%

"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { }
<C_COMMENT>.    { }

{LINE_COMMENT}  { }
{WHITESPACE}   { }

"const"         { return CONST; }
"int"           { return INT; }
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"break"         { return BREAK; }
"continue"      { return CONTINUE; }
"return"        { return RETURN; }

{INT_CONST}     { yylval.int_val = atoi(yytext); return INT_CONST; }
{IDENT}         { yylval.str_val = strdup(yytext); return IDENT; }

"&&"            { return AND_OP; }
"||"            { return OR_OP; }
"<="            { return LE_OP; }
">="            { return GE_OP; }
"=="            { return EQ_OP; }
"!="            { return NE_OP; }
";"             { return ';'; }
"{"             { return '{'; }
"}"             { return '}'; }
","             { return ','; }
"="             { return '='; }
"("             { return '('; }
")"             { return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"!"             { return '!'; }
"-"             { return '-'; }
"+"             { return '+'; }
"*"             { return '*'; }
"/"             { return '/'; }
"%"             { return '%'; }
"<"             { return '<'; }
">"             { return '>'; }

.               { /* bad characters */ }

%%
