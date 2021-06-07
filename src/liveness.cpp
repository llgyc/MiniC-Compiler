#include <memory>

#include "utils.hpp"
#include "eeyore.hpp"
#include "liveness.hpp"

namespace liveness {

#define CAST_P(POINTER, TYPE) \
dynamic_cast<eeyore::TYPE *>(POINTER.get())

#define TEST_TYPE(POINTER, TYPE) \
if (CAST_P(POINTER, TYPE) != nullptr)

static int numG;
static int numN;
static int numT;
static int numP;

void local_ident(eeyore::FunctionDef *func, std::set<int> &s) {
    s.clear();
}

void block_init(eeyore::FunctionDef *func, std::set<int> &s) {
    s.clear();
}

void global_init(eeyore::FunctionDef *func, std::set<int> &s) {
    s.clear();
}

int getID(eeyore::VarPtr var) {
    TEST_TYPE(var, NativeVar) {
        auto ptr = CAST_P(var, NativeVar);
        if (ptr->id_ < numG) return -1;
        return ptr->id_ - numG;
    } else TEST_TYPE(var, TempVar) {
        auto ptr = CAST_P(var, TempVar);
        return numN + ptr->id_;
    } else TEST_TYPE(var, ParamVar) {
        auto ptr = CAST_P(var, ParamVar);
        return numN + numT + ptr->id_;
    } else return -1;
}

#define COND_INS(DATA) \
if (getID(DATA) != -1) ret.insert(getID(DATA));

std::set<int> use(eeyore::FunctionDef *func, eeyore::InstPtr inst) {
    std::set<int> ret; 
    TEST_TYPE(inst, AssignInst) {
        auto ptr = CAST_P(inst, AssignInst);
        COND_INS(ptr->rhs_);
    } else TEST_TYPE(inst, ArrayAssignInst) {
        auto ptr = CAST_P(inst, ArrayAssignInst);
        COND_INS(ptr->lhs_var_);
        COND_INS(ptr->lhs_index_);
        COND_INS(ptr->rhs_);
    } else TEST_TYPE(inst, AssignArrayInst) {
        auto ptr = CAST_P(inst, AssignArrayInst);
        COND_INS(ptr->rhs_var_);
        COND_INS(ptr->rhs_index_);
    } else TEST_TYPE(inst, BinaryInst) {
        auto ptr = CAST_P(inst, BinaryInst);
        COND_INS(ptr->rhs_1_);
        COND_INS(ptr->rhs_2_);
    } else TEST_TYPE(inst, UnaryInst) {
        auto ptr = CAST_P(inst, UnaryInst);
        COND_INS(ptr->rhs_);
    } else TEST_TYPE(inst, CondInst) {
        auto ptr = CAST_P(inst, CondInst);
        COND_INS(ptr->cond_1_);
        COND_INS(ptr->cond_2_);
    } else TEST_TYPE(inst, JumpInst) {
        // Do nothing!
    } else TEST_TYPE(inst, ParamInst) {
        auto ptr = CAST_P(inst, ParamInst);
        COND_INS(ptr->param_);
    } else TEST_TYPE(inst, AssignCallInst) {
        // Do nothing!
    } else TEST_TYPE(inst, ReturnInst) {
        auto ptr = CAST_P(inst, ReturnInst);
        if (ptr->ret_ != nullptr) {
            COND_INS(ptr->ret_);
        }
    } else assert(false);
    return ret;
}

std::set<int> def(eeyore::FunctionDef *func, eeyore::InstPtr inst) {
    std::set<int> ret;
    TEST_TYPE(inst, AssignInst) {
        auto ptr = CAST_P(inst, AssignInst);
        COND_INS(ptr->lhs_);
    } else TEST_TYPE(inst, ArrayAssignInst) {
        // Do nothing!
    } else TEST_TYPE(inst, AssignArrayInst) {
        auto ptr = CAST_P(inst, AssignArrayInst);
        COND_INS(ptr->lhs_);
    } else TEST_TYPE(inst, BinaryInst) {
        auto ptr = CAST_P(inst, BinaryInst);
        COND_INS(ptr->lhs_);
    } else TEST_TYPE(inst, UnaryInst) {
        auto ptr = CAST_P(inst, UnaryInst);
        COND_INS(ptr->lhs_);
    } else TEST_TYPE(inst, CondInst) {
        // Do nothing!
    } else TEST_TYPE(inst, JumpInst) {
        // Do nothing!
    } else TEST_TYPE(inst, ParamInst) {
        // Do nothing!
    } else TEST_TYPE(inst, AssignCallInst) {
        auto ptr = CAST_P(inst, AssignCallInst);
        if (ptr->store_ != nullptr) {
            COND_INS(ptr->store_);
        }
    } else TEST_TYPE(inst, ReturnInst) {
        // Do nothing!
    } else assert(false);
    return ret;
}

#undef COND_INS

data_flow::Description desc {
    &use,
    &def,
    &local_ident,
    &block_init,
    &global_init,
    &data_flow::mUnion
};

void calculate(eeyore::Program &ir) {
    numG = ir.funcs().back()->nativeNum();
    for (auto &func : ir.funcs()) {
        numN = func->nativeNum();
        numT = func->tempNum();
        numP = func->paramNum();
        func->backwardAccess(desc);
    }
}

#define COND_INS(DATA) \
if (getID(DATA) != -1 && \
    inst->data_out_.find(getID(DATA)) == inst->data_out_.end()) \
    pos.push_back(i);

void reconstruct(eeyore::FuncPtr ptr) {
    std::vector<int> pos;
    for (int i = 0; i < ptr->instNum(); ++i) {
        auto inst = ptr->insts()[i];
        TEST_TYPE(inst, AssignInst) {
            auto ptr = CAST_P(inst, AssignInst);
            COND_INS(ptr->lhs_);
        } else TEST_TYPE(inst, ArrayAssignInst) {
            // Do nothing!
        } else TEST_TYPE(inst, AssignArrayInst) {
            auto ptr = CAST_P(inst, AssignArrayInst);
            COND_INS(ptr->lhs_);
        } else TEST_TYPE(inst, BinaryInst) {
            auto ptr = CAST_P(inst, BinaryInst);
            COND_INS(ptr->lhs_);
        } else TEST_TYPE(inst, UnaryInst) {
            auto ptr = CAST_P(inst, UnaryInst);
            COND_INS(ptr->lhs_);
        } else TEST_TYPE(inst, CondInst) {
            // Do nothing!
        } else TEST_TYPE(inst, JumpInst) {
            // Do nothing!
        } else TEST_TYPE(inst, ParamInst) {
            // Do nothing!
        } else TEST_TYPE(inst, AssignCallInst) {
            auto ptr = CAST_P(inst, AssignCallInst);
            if (ptr->store_ != nullptr) {
                COND_INS(ptr->store_);
            }
        } else TEST_TYPE(inst, ReturnInst) {
            // Do nothing!
        } else assert(false);

    }
    removeUnnecessary(ptr, pos);
}

#undef COND_INS

void optimize(eeyore::Program &ir) {
    numG = ir.funcs().back()->nativeNum();
    for (auto &func : ir.funcs()) {
        numN = func->nativeNum();
        numT = func->tempNum();
        numP = func->paramNum();
        int times = 5;
        while (--times) {
            func->backwardAccess(desc);
            reconstruct(func);
        }
    }
}

#undef TEST_TYPE
#undef CAST_P

} // namespace liveness
