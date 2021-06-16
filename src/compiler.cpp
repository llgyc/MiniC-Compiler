#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cassert>

#include "context.hpp"
#include "tigger.hpp"
#include "naive_alloc.hpp"
#include "graph_coloring.hpp"
#include "liveness.hpp"
#include "reaching.hpp"
#include "riscv_gen.hpp"

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

eeyore::Program eir1;

tigger::Program tir1;

void SysY_to_Eeyore(char *file, eeyore::Program &ir) {
    // SysY to Eeyore
    yyin = fopen(file, "r");
    yylineno = 1;
    BaseASTNode *root_ptr = nullptr;
    yyparse(&root_ptr);
    assert(root_ptr != nullptr);
    ast_root.reset(root_ptr);
    std::cerr << "[Success] Lexer and parser succeeded" << std::endl;
    ast_root->generateEeyoreCode(eeyore_generation_context, ir);
    std::cerr << "[Success] AST generation completed" << std::endl;
    // Optional
    reaching::optimize(ir);
    std::cerr << "[Success] Analyze reaching definition info" << std::endl;
}

void Eeyore_to_Tigger(eeyore::Program &ir1, tigger::Program &ir2) {
    // Various register allocation options:
    // naive_alloc::translate_E2T(ir1, ir2);
    graph_coloring::translate_E2T(ir1, ir2);
    std::cerr << "[Success] Tigger translation completed" << std::endl;
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
        SysY_to_Eeyore(argv[2], eir1);
        Eeyore_to_Tigger(eir1, tir1);
        std::ofstream ofs(argv[4]);
        riscv_gen::translate_T2R(tir1, ofs);
        std::cerr << "[Success] RISC-V code dumped" << std::endl;
    }
    
    if (argc == 6) {
        if (strcmp(argv[4], "-o")) {
            print_usage();
            return 0;
        }
        if (!strcmp(argv[2], "-e")) { 
            SysY_to_Eeyore(argv[3], eir1);
            std::ofstream ofs(argv[5]);
            eir1.dumpCode(ofs);
            std::cerr << "[Success] Eeyore code dumped" << std::endl;
        } else if (!strcmp(argv[2], "-t")) {
            SysY_to_Eeyore(argv[3], eir1);
            Eeyore_to_Tigger(eir1, tir1);
            std::ofstream ofs(argv[5]);
            tir1.dumpCode(ofs);
            std::cerr << "[Success] Tigger code dumped" << std::endl;
        }
    }

    return 0;
}
