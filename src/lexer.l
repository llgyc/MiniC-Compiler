%option noyywrap
%option noinput
%option nounput
%option yylineno

%{

#include <stdlib.h>

#include "context.hpp"
#include "parser.tab.hpp"

extern int yylex(void);
extern void yyerror(BaseASTNode **, const char *);

%}

IDENT               [_a-zA-Z][_a-zA-Z0-9]*
DEC_CONST           [1-9][0-9]*
OCT_CONST           0[0-7]*
HEX_CONST           0[xX][0-9a-fA-F]+
LINE_COMMENT        \/\/.*
WHITESPACE          [ \t\v\n\f]

%x C_COMMENT

%%

"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { }
<C_COMMENT>.    { }

{LINE_COMMENT}  { }
{WHITESPACE}    { }

"const"         { return CONST; }
"int"           { return INT; }
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"break"         { return BREAK; }
"continue"      { return CONTINUE; }
"return"        { return RETURN; }

{DEC_CONST}     { yylval.int_val = std::stoi(yytext, 0, 10); return INT_CONST; }
{OCT_CONST}     { yylval.int_val = std::stoi(yytext, 0, 8);  return INT_CONST; }
{HEX_CONST}     { yylval.int_val = std::stoi(yytext, 0, 16); return INT_CONST; }
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
