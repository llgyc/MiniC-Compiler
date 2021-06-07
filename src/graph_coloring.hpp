#ifndef __MINIC_GRAPH_COLORING_HPP__
#define __MINIC_GRAPH_COLORING_HPP__

#include <cassert>
#include <unordered_map>

#include "eeyore.hpp"
#include "tigger.hpp"
#include "operator.hpp"

namespace graph_coloring {

static std::unordered_map<int, int> Tstart, tstart;
static int global_num;
static int param_pos;

#define CAST_P(POINTER, TYPE) \
dynamic_cast<eeyore::TYPE *>(POINTER.get())

#define TEST_TYPE(POINTER, TYPE) \
if (CAST_P(POINTER, TYPE) != nullptr)

void generateTiggerCode(eeyore::FuncPtr func, tigger::Program &dst) {
    tigger::FuncPtr t_func;
    int now_pos = func->paramNum();
    if (func->name() != "$global$") {
        for (auto &var : func->native()) {
            auto ptr = CAST_P(var, NativeVar);
            Tstart[ptr->id_] = now_pos;
            if (ptr->widths_.size() == 0) {
                ++now_pos;
            } else {
                now_pos += ptr->widths_[0];
            }
        }
    }
    for (auto &var : func->temp()) {
        auto ptr = CAST_P(var, TempVar);
        tstart[ptr->id_] = now_pos;
        ++now_pos;
    }
    
    t_func = std::make_shared<tigger::FunctionDef>
        (func->name(), func->paramNum(), now_pos);
    dst.pushFunction(t_func);

    #define PUSH_INST(INST) \
    t_func->pushInst(std::move(INST))

    #define LOAD_SYMBOL(SYMBOL, REG) \
    TEST_TYPE(SYMBOL, NativeVar) { \
        auto _ptr = CAST_P(SYMBOL, NativeVar); \
        if (_ptr->id_ < global_num) { \
            if (_ptr->widths_.size() == 0) { \
                auto _inst = std::make_shared<tigger::VarLoadInst> \
                    (_ptr->id_, REG); \
                PUSH_INST(_inst); \
            } else { \
                auto _inst = std::make_shared<tigger::VarAddrLoadInst> \
                    (_ptr->id_, REG); \
                PUSH_INST(_inst); \
            } \
        } else { \
            if (_ptr->widths_.size() == 0) { \
                auto _inst = std::make_shared<tigger::StackLoadInst> \
                    (Tstart[_ptr->id_], REG); \
                PUSH_INST(_inst); \
            } else { \
                auto _inst = std::make_shared<tigger::StackAddrLoadInst> \
                    (Tstart[_ptr->id_], REG); \
                PUSH_INST(_inst); \
            } \
        } \
    } else TEST_TYPE(SYMBOL, TempVar) { \
        auto _ptr = CAST_P(SYMBOL, TempVar); \
        auto _inst = std::make_shared<tigger::StackLoadInst> \
            (tstart[_ptr->id_], REG); \
        PUSH_INST(_inst); \
    } else TEST_TYPE(SYMBOL, ParamVar) { \
        auto _ptr = CAST_P(SYMBOL, ParamVar); \
        auto _inst = std::make_shared<tigger::StackLoadInst> \
            (_ptr->id_, REG); \
        PUSH_INST(_inst); \
    } else TEST_TYPE(SYMBOL, IntValue) { \
        auto _ptr = CAST_P(SYMBOL, IntValue); \
        auto _inst = std::make_shared<tigger::AssignIInst> \
            (REG, _ptr->val_); \
        PUSH_INST(_inst); \
    } else assert(false);

    #define STORE_SYMBOL(SYMBOL, REG) \
    TEST_TYPE(SYMBOL, NativeVar) { \
        auto _ptr = CAST_P(SYMBOL, NativeVar); \
        if (_ptr->id_ < global_num) { \
            if (_ptr->widths_.size() == 0) { \
                auto _inst = std::make_shared<tigger::VarAddrLoadInst> \
                    (_ptr->id_, 12); \
                PUSH_INST(_inst); \
                auto _inst2 = std::make_shared<tigger::MemStoreInst> \
                    (12, 0, REG); \
                PUSH_INST(_inst2); \
            } else assert(false); \
        } else { \
            if (_ptr->widths_.size() == 0) { \
                auto _inst = std::make_shared<tigger::StackStoreInst> \
                    (REG, Tstart[_ptr->id_]); \
                PUSH_INST(_inst); \
            } else assert(false); \
        } \
    } else TEST_TYPE(SYMBOL, TempVar) { \
        auto _ptr = CAST_P(SYMBOL, TempVar); \
        auto _inst = std::make_shared<tigger::StackStoreInst> \
            (REG, tstart[_ptr->id_]); \
        PUSH_INST(_inst); \
    } else TEST_TYPE(SYMBOL, ParamVar) { \
        auto _ptr = CAST_P(SYMBOL, ParamVar); \
        auto _inst = std::make_shared<tigger::StackStoreInst> \
            (REG, _ptr->id_); \
        PUSH_INST(_inst); \
    } else assert(false);


    // save parameter
    for (int i = 0; i < func->paramNum(); i++) {
        STORE_SYMBOL(func->params()[i], 20 + i);
    }

    // true instructions
    int cnt = 0;
    t_func->setLabelNum(func->labelNum());
    for (auto &inst : func->insts()) {
        for (int j = 0; j < func->labelNum(); ++j) {
            if (cnt == func->label_pos()[j]) {
                t_func->setLabel(j, t_func->instNum());
            }
        }
        ++cnt;
        TEST_TYPE(inst, AssignInst) {
            // SYMBOL = RightValue
            auto ptr = CAST_P(inst, AssignInst);
            LOAD_SYMBOL(ptr->rhs_, 1);
            STORE_SYMBOL(ptr->lhs_, 1);
        } else TEST_TYPE(inst, ArrayAssignInst) {
            // SYMBOL [ RightValue ] = RightValue
            auto ptr = CAST_P(inst, ArrayAssignInst);
            LOAD_SYMBOL(ptr->rhs_, 1);
            LOAD_SYMBOL(ptr->lhs_var_, 2);
            LOAD_SYMBOL(ptr->lhs_index_, 3);
            auto inst = std::make_shared<tigger::BinaryRInst>
                (Operator::kAdd, 2, 2, 3);
            PUSH_INST(inst);
            auto inst2 = std::make_shared<tigger::MemStoreInst>
                (2, 0, 1);
            PUSH_INST(inst2);
        } else TEST_TYPE(inst, AssignArrayInst) {
            // SYMBOL = SYMBOL [ RightValue ]
            auto ptr = CAST_P(inst, AssignArrayInst);
            LOAD_SYMBOL(ptr->rhs_var_, 1);
            LOAD_SYMBOL(ptr->rhs_index_, 2);
            auto inst = std::make_shared<tigger::BinaryRInst>
                (Operator::kAdd, 1, 1, 2);
            PUSH_INST(inst);
            auto inst2 = std::make_shared<tigger::MemLoadInst>
                (1, 1, 0);
            PUSH_INST(inst2);
            STORE_SYMBOL(ptr->lhs_, 1);
        } else TEST_TYPE(inst, BinaryInst) {
            // SYMBOL = RightValue BinOp RightValue
            auto ptr = CAST_P(inst, BinaryInst);
            LOAD_SYMBOL(ptr->rhs_1_, 1);
            LOAD_SYMBOL(ptr->rhs_2_, 2);
            auto inst = std::make_shared<tigger::BinaryRInst>
                (ptr->op_, 1, 1, 2);
            PUSH_INST(inst);
            STORE_SYMBOL(ptr->lhs_, 1);
        } else TEST_TYPE(inst, UnaryInst) {
            // SYMBOL = OP RightValue
            auto ptr = CAST_P(inst, UnaryInst);
            LOAD_SYMBOL(ptr->rhs_, 1);
            auto inst = std::make_shared<tigger::UnaryInst>
                (ptr->op_, 1, 1);
            PUSH_INST(inst);
            STORE_SYMBOL(ptr->lhs_, 1);
        } else TEST_TYPE(inst, CondInst) {
            // if RightValue LOGICOP RightValue goto LABEL
            auto ptr = CAST_P(inst, CondInst);
            LOAD_SYMBOL(ptr->cond_1_, 1);
            LOAD_SYMBOL(ptr->cond_2_, 2);
            auto inst = std::make_shared<tigger::CondInst>
                (ptr->op_, 1, 2, ptr->label_id_);
            PUSH_INST(inst);
        } else TEST_TYPE(inst, JumpInst) {
            // goto LABEL
            auto ptr = CAST_P(inst, JumpInst);
            auto inst = std::make_shared<tigger::JumpInst>
                (ptr->label_id_);
            PUSH_INST(inst);
        } else TEST_TYPE(inst, ParamInst) {
            // param RightValue
            auto ptr = CAST_P(inst, ParamInst);
            LOAD_SYMBOL(ptr->param_, 1);
            auto inst = std::make_shared<tigger::AssignRInst>
                (20 + param_pos++, 1);
            PUSH_INST(inst);
        } else TEST_TYPE(inst, AssignCallInst) {
            // [SYMBOL =] call FUNCTION
            auto ptr = CAST_P(inst, AssignCallInst);
            auto inst = std::make_shared<tigger::CallInst>
                (ptr->func_->name());
            PUSH_INST(inst);
            param_pos = 0;
            if (ptr->store_ != nullptr) {
                STORE_SYMBOL(ptr->store_, 20);
            }
        } else TEST_TYPE(inst, ReturnInst) {
            // return [RightValue]
            auto ptr = CAST_P(inst, ReturnInst);
            if (ptr->ret_ != nullptr) {
                LOAD_SYMBOL(ptr->ret_, 20);
            }
            auto inst = std::make_shared<tigger::ReturnInst>();
            PUSH_INST(inst);
        } else assert(false);

    }
    #undef STORE_SYMBOL
    #undef LOAD_SYMBOL
    #undef PUSH_INST
}

void translate_E2T(eeyore::Program &src, tigger::Program &dst) {
    Tstart.clear(); tstart.clear();
    global_num = param_pos = 0;
    // Calculate global_num
    assert(src.funcs().back()->name() == "$global$");
    for (auto &var : src.funcs().back()->native()) {
        auto ptr = dynamic_cast<eeyore::NativeVar *>(var.get());
        if (ptr->widths().size() == 0) {
            int val;
            if (ptr->getVal().has_value()) {
                val = ptr->getVal().value();
            } else {
                val = 0;
            }
            dst.pushGlobal(global_num++, false, val);
        } else {
            int size = ptr->widths()[0] * 4;
            dst.pushGlobal(global_num++, true, size);
        }
    }
    // Dump Functions
    for (auto &func : src.funcs()) {
        generateTiggerCode(func, dst);
    }
}

#undef TEST_TYPE
#undef CAST_P
    
} // namespace graph_coloring

#endif // __MINIC_GRAPH_COLORING_HPP__

