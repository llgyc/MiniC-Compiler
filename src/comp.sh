flex -o lex.yy.c lexer.l
bison -d -o parser.tab.c parser.y -v
g++-8 -Wno-register -O2 -lm -std=c++17 lex.yy.c parser.tab.c compiler.cpp -o compiler
