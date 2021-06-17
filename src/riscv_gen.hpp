#ifndef __MINIC_RISCV_GEN_HPP__
#define __MINIC_RISCV_GEN_HPP__

#include <cassert>

#include "tigger.hpp"
#include "operator.hpp"

namespace riscv_gen {

#define PRINT_REG(REG) \
if (REG < 0) { \
    assert(false); \
} else if (REG == 0) { \
    os << "x0"; \
} else if (REG < 13) { \
    os << "s" << (REG - 1); \
} else if (REG < 20) { \
    os << "t" << (REG - 13); \
} else if (REG < 28) { \
    os << "a" << (REG - 20); \
} else if (REG == 28) { \
    os << "ra"; \
} else if (REG == 29) { \
    os << "sp"; \
} else assert(false);

#define PRINT_REG2(REG1, REG2) \
PRINT_REG(REG1); \
os << ", "; \
PRINT_REG(REG2);

#define PRINT_REG3(REG1, REG2, REG3) \
PRINT_REG2(REG1, REG2); \
os << ", "; \
PRINT_REG(REG3);

#define CAST_P(POINTER, TYPE) \
dynamic_cast<tigger::TYPE *>(POINTER.get())

#define TEST_TYPE(POINTER, TYPE) \
if (CAST_P(POINTER, TYPE) != nullptr)

inline static bool is_int12(int val) {
    return (val >= -2048 && val <= 2047);
}

void generateBinaryRInst(std::ostream &os, 
    Operator op, int r1, int r2, int r3) {
    if (op != Operator::kAnd) {
        os << "  " << instname[static_cast<int>(op)];
        PRINT_REG3(r1, r2, r3);
        os << std::endl;
    } 
    if (op == Operator::kLessEq || 
        op == Operator::kGreaterEq ||
        op == Operator::kEq) {
        os << "  seqz     ";
        PRINT_REG2(r1, r1);
        os << std::endl;
    } else if (op == Operator::kAnd) {
        os << "  snez     ";
        PRINT_REG2(r1, r2);
        os << std::endl;
        os << "  snez     ";
        PRINT_REG2(13, r3);
        os << std::endl;
        os << "  and      ";
        PRINT_REG3(r1, r1, 13);
        os << std::endl;
    } else if (op == Operator::kOr ||
               op == Operator::kNeq) {
        os << "  snez     ";
        PRINT_REG2(r1, r1);
        os << std::endl;
    }
}

void generateSW(std::ostream &os, int r1, int r2, int imm) {
    if (is_int12(imm)) {
        os << "  sw       ";
        PRINT_REG(r2);
        os << ", " << imm << "(";
        PRINT_REG(r1);
        os << ")" << std::endl;
    } else {
        os << "  li       t0, " << imm << std::endl;
        os << "  add      t0, ";
        PRINT_REG(r1);
        os << ", t0" << std::endl;
        os << "  sw       ";
        PRINT_REG(r2);
        os << ", 0(t0)" << std::endl;
    }
}

void generateLW(std::ostream &os, int r1, int r2, int imm) {
    if (is_int12(imm)) {
        os << "  lw       ";
        PRINT_REG(r1);
        os << ", " << imm << "(";
        PRINT_REG(r2);
        os << ")" << std::endl;
    } else {
        os << "  li       t0, " << imm << std::endl;
        os << "  add      t0, ";
        PRINT_REG(r2);
        os << ", t0" << std::endl;
        os << "  lw       ";
        PRINT_REG(r1);
        os << ", 0(t0)" << std::endl;
    }
}

void generateRISCVCode(tigger::FuncPtr func, std::ostream &os,
    int label_init_id) {
    std::string name = func->name();
    if (func->name() == "$global$") {
        name = "main";
    }
    if (func->name() == "main") {
        name = "__main__";
    }

    bool leaf_flag = true;
    for (auto &inst : func->insts()) {
        TEST_TYPE(inst, CallInst) {
            leaf_flag = false;
            break;
        }
    }

    // Header
    os << "  .text" << std::endl;
    os << "  .align   2" << std::endl;
    os << "  .global  " << name << std::endl;
    os << "  .type    " << name << ", @function" << std::endl;
    os << name << ":" << std::endl;
    int int2 = func->stk_size();
    int STK = leaf_flag ? (int2 + 3) / 4 * 16 : (int2 / 4 + 1) * 16;
    if (STK == 0) {
        // Do nothing!
    } else if (is_int12(-STK)) {
        os << "  addi     sp, sp, " << -STK << std::endl;
    } else {
        os << "  li       t0, " << -STK << std::endl;
        os << "  add      sp, sp, t0" << std::endl;
    }
    if (!leaf_flag)
        generateSW(os, 29, 28, STK-4);

    for (int i = 0; i < func->instNum(); ++i) {
        auto inst = func->insts()[i];
        for (int j = 0; j < func->labelNum(); ++j) {
            if (i == func->label_pos()[j]) {
                os << ".l" << j + label_init_id << ":" << std::endl;
            }
        }
        // Dump Instructions
        TEST_TYPE(inst, BinaryRInst) {
            auto ptr = CAST_P(inst, BinaryRInst);
            generateBinaryRInst(os, ptr->op_, ptr->rd_, ptr->rs1_, ptr->rs2_);
        } else TEST_TYPE(inst, BinaryIInst) {
            auto ptr = CAST_P(inst, BinaryIInst);
            if (is_int12(ptr->imm_)) { 
                if (ptr->op_ == Operator::kAdd) {
                    os << "  addi     ";
                    PRINT_REG2(ptr->rd_, ptr->rs_);
                    os << ", " << ptr->imm_ << std::endl;
                    continue;
                } else if (ptr->op_ == Operator::kLess) {
                    os << "  slti     ";
                    PRINT_REG2(ptr->rd_, ptr->rs_);
                    os << ", " << ptr->imm_ << std::endl;
                    continue;
                } else if (ptr->op_ == Operator::kShl) {
                    os << "  slli     ";
                    PRINT_REG2(ptr->rd_, ptr->rs_);
                    os << ", " << ptr->imm_ << std::endl;
                    continue;
                } else if (ptr->op_ == Operator::kShr) {
                    os << "  srai     ";
                    PRINT_REG2(ptr->rd_, ptr->rs_);
                    os << ", " << ptr->imm_ << std::endl;
                    continue;
                }
            }
            os << "  li       t0, " << ptr->imm_ << std::endl;
            generateBinaryRInst(os, ptr->op_, ptr->rd_, ptr->rs_, 13);
        } else TEST_TYPE(inst, UnaryInst) {
            auto ptr = CAST_P(inst, UnaryInst);
            if (ptr->op_ == Operator::kSub) {
                os << "  neg      ";
                PRINT_REG2(ptr->rd_, ptr->rs_);
                os << std::endl;
            } else if (ptr->op_ == Operator::kNot) {
                os << "  seqz     ";
                PRINT_REG2(ptr->rd_, ptr->rs_);
                os << std::endl;
            } else assert(false);
        } else TEST_TYPE(inst, AssignRInst) {
            auto ptr = CAST_P(inst, AssignRInst);
            os << "  mv       ";
            PRINT_REG2(ptr->rd_, ptr->rs_);
            os << std::endl;
        } else TEST_TYPE(inst, AssignIInst) {
            auto ptr = CAST_P(inst, AssignIInst);
            os << "  li       ";
            PRINT_REG(ptr->rd_);
            os << ", " << ptr->imm_ << std::endl;
        } else TEST_TYPE(inst, MemStoreInst) {
            auto ptr = CAST_P(inst, MemStoreInst);
            generateSW(os, ptr->rd_, ptr->rs_, ptr->imm_);
        } else TEST_TYPE(inst, MemLoadInst) {
            auto ptr = CAST_P(inst, MemLoadInst);
            generateLW(os, ptr->rd_, ptr->rs_, ptr->imm_);
        } else TEST_TYPE(inst, CondInst) {
            auto ptr = CAST_P(inst, CondInst);
            std::string hd = "  ";
            if (ptr->op_ == Operator::kLess) {
                hd += "blt      ";
            } else if (ptr->op_ == Operator::kGreater) {
                hd += "bgt      ";
            } else if (ptr->op_ == Operator::kLessEq) {
                hd += "ble      ";
            } else if (ptr->op_ == Operator::kGreaterEq) {
                hd += "bge      ";
            } else if (ptr->op_ == Operator::kNeq) {
                hd += "bne      ";
            } else if (ptr->op_ == Operator::kEq) {
                hd += "beq      ";
            }
            os << hd;
            PRINT_REG2(ptr->rs1_, ptr->rs2_);
            os << ", .l" << label_init_id + ptr->label_ << std::endl;
        } else TEST_TYPE(inst, JumpInst) {
            auto ptr = CAST_P(inst, JumpInst);
            os << "  j        .l" << label_init_id + ptr->label_ << std::endl;
        } else TEST_TYPE(inst, CallInst) {
            auto ptr = CAST_P(inst, CallInst);
            auto name = ptr->func_;
            if (name == "main") name = "__main__";
            os << "  call     " << name << std::endl;
        } else TEST_TYPE(inst, ReturnInst) {
            int int2 = func->stk_size();
            int STK = leaf_flag ? (int2 + 3) / 4 * 16 : (int2 / 4 + 1) * 16;
            if (!leaf_flag)
                generateLW(os, 28, 29, STK-4);
            if (STK == 0) {
                // Do nothing!
            } else if (is_int12(STK)) {
                os << "  addi     sp, sp, " << STK << std::endl;
            } else {
                os << "  li       t0, " << STK << std::endl;
                os << "  add      sp, sp, t0" << std::endl;
            }
            os << "  ret" << std::endl;
        } else TEST_TYPE(inst, StackStoreInst) {
            auto ptr = CAST_P(inst, StackStoreInst);
            generateSW(os, 29, ptr->rs_, ptr->pos_*4);
        } else TEST_TYPE(inst, StackLoadInst) {
            auto ptr = CAST_P(inst, StackLoadInst);
            generateLW(os, ptr->rd_, 29, ptr->pos_*4);
        } else TEST_TYPE(inst, VarLoadInst) {
            auto ptr = CAST_P(inst, VarLoadInst);
            os << "  lui      ";
            PRINT_REG(ptr->rd_);
            os << ", %hi(v" << ptr->var_ << ")" << std::endl;
            os << "  lw       ";
            PRINT_REG(ptr->rd_);
            os << ", %lo(v" << ptr->var_ << ")(";
            PRINT_REG(ptr->rd_);
            os << ")" << std::endl;
        } else TEST_TYPE(inst, StackAddrLoadInst) {
            auto ptr = CAST_P(inst, StackAddrLoadInst);
            if (is_int12(ptr->pos_ * 4)) {
                os << "  addi     ";
                PRINT_REG(ptr->rd_);
                os << ", sp, " << ptr->pos_ * 4 << std::endl;
            } else {
                os << "  li       t0, " << ptr->pos_ * 4 << std::endl;
                os << "  add      ";
                PRINT_REG(ptr->rd_);
                os << ", sp, t0" << std::endl;
            }
        } else TEST_TYPE(inst, VarAddrLoadInst) {
            auto ptr = CAST_P(inst, VarAddrLoadInst);
            os << "  la       ";
            PRINT_REG(ptr->rd_);
            os << ", v" << ptr->var_ << std::endl;
        } else assert(false);
    }

    // Tailer
    os << "  .size    " << name << ", .-";
    os << name << std::endl;
}

void translate_T2R(tigger::Program &src, std::ostream &os) {
    // GlobalVarDecl
    for (auto x : src.global()) {
        if (x.second.first) {
            // Global Array
            os << "  .comm     v" << x.first << ", ";
            os << x.second.second << ", 4" << std::endl;
        } else {
            // Global Variable
            std::string name = "v" + std::to_string(x.first);
            os << "  .global   " << name << std::endl;
            os << "  .section  .sdata" << std::endl;
            os << "  .align    2" << std::endl;
            os << "  .type     " << name << ", @object" << std::endl;
            os << "  .size     " << name << ", 4" << std::endl;
            os << name << ":" << std::endl;
            os << "  .word     " << x.second.second << std::endl;
        }
    }

    // FunctionDef
    int total_label = 0;
    for (auto &func : src.funcs()) {
        generateRISCVCode(func, os, total_label);
        total_label += func->labelNum();
    }
}

#undef TEST_TYPE
#undef CAST_P

#undef PRINT_REG3
#undef PRINT_REG2
#undef PRINT_REG
    
} // namespace riscv_gen

#endif // __MINIC_RISCV_GEN_HPP__

