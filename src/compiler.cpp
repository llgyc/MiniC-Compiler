#include <iostream>
#include <string>
#include <cstring>
#include <cassert>

#include "ast.hpp"
#include "utils.hpp"

extern std::FILE *yyin;
extern int yyparse(BaseASTNode **);

static const std::string usage_prompt = 
    "Usage: compiler -S {-e input | -t input | input} -o output";

static void print_usage() {
    std::cerr << usage_prompt << std::endl;
}

ASTNodePtr ast_root;


void SysY_to_Eeyore(int argc, char *argv[]) {
    // SysY to Eeyore
    yyin = fopen(argv[3], "r");
    BaseASTNode *root_ptr = nullptr;
    yyparse(&root_ptr);
    assert(root_ptr != nullptr);
    ast_root.reset(root_ptr);
}

int main(int argc, char *argv[]) {
    
    if (argc < 4 || argc > 5 || !strcmp(argv[1], "-S")) {
        print_usage();
        return 0;
    }

    if (argc == 4) { 
        if (strcmp(argv[3], "-o")) {
            print_usage();
            return 0;
        }
        // TODO: Tiger to RISC-V

    }
    
    if (argc == 5) {
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
