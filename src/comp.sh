rm lex.yy.cpp parser.tab.cpp parser.tab.hpp compiler parser.output 
echo "[Flex] Processing ..."
flex -o lex.yy.cpp lexer.l
echo "[Bison] Processing ..."
bison -d -o parser.tab.cpp parser.y -v -t
echo "[GCC] Compiling ..."
g++-8 -Wno-register -O0 -lm -std=c++17 *.cpp -o compiler -g -Wall
echo "[Success] Finished"
