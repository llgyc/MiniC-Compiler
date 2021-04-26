#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cassert>

#include "context.hpp"

extern int yylineno;
extern std::FILE *yyin;
extern int yyparse(BaseASTNode **);

static const std::string usage_prompt = 
    "Usage: compiler -S {-e input | -t input | input} -o output";

static void print_usage() {
    std::cerr << usage_prompt << std::endl;
}

ASTNodePtr ast_root;

Context eeyore_generation_context;

eeyore::Program ir1;

void SysY_to_Eeyore(int argc, char *argv[]) {
    // SysY to Eeyore
    yyin = fopen(argv[3], "r");
    yylineno = 1;
    BaseASTNode *root_ptr = nullptr;
    yyparse(&root_ptr);
    assert(root_ptr != nullptr);
    ast_root.reset(root_ptr);
    std::cerr << "[Success] Lexer and parser succeeded" << std::endl;
    ast_root->generateEeyoreCode(eeyore_generation_context, ir1);
    std::cerr << "[Success] AST generation completed" << std::endl;
    std::ofstream ofs(argv[5]);
    ir1.dumpCode(ofs);
    std::cerr << "[Success] Eeyore Code dumped" << std::endl;
}

int main(int argc, char *argv[]) {
    
    if (argc < 5 || argc > 6 || strcmp(argv[1], "-S")) {
        print_usage();
        return 0;
    }

    if (argc == 5) { 
        if (strcmp(argv[3], "-o")) {
            print_usage();
            return 0;
        }
        // TODO: Tiger to RISC-V

    }
    
    if (argc == 6) {
        if (strcmp(argv[4], "-o")) {
            print_usage();
            return 0;
        }
        if (!strcmp(argv[2], "-e")) { 
            SysY_to_Eeyore(argc, argv);
        } else if (!strcmp(argv[2], "-t")) {
            // TODO: Eeyore to Tiger
        }
    }

    return 0;
}
