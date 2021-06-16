#include <vector>

#include "utils.hpp"
#include "peephole_eeyore.hpp"

namespace eeyore {

#define CAST_P(POINTER, TYPE) \
dynamic_cast<TYPE *>(POINTER.get())

#define TEST_TYPE(POINTER, TYPE) \
if (CAST_P(POINTER, TYPE) != nullptr)

static FuncPtr global;
static int numG;

bool extractConst(FuncPtr func, VarPtr var, int &ret) {
    TEST_TYPE(var, NativeVar) {
        auto ptr = CAST_P(var, NativeVar);
        VarPtr target;
        if (ptr->id_ < numG) {
            target = global->native_[ptr->id_];
        } else {
            target = func->native_[ptr->id_-numG];
        }
        auto ptr2 = CAST_P(target, NativeVar);
        if (ptr2->is_const_ && ptr2->widths().size() != 0) {
            auto val = ptr2->getVal();
            if (val.has_value()) {
                ret = val.value();
                return true;
            } else return false;
        }
    }
    return false;
}

void precalculation(Program &ir) {
    for (auto &func : ir.funcs()) {
        std::vector<int> erased;
        for (int i = 0; i < func->instNum(); ++i) {
            auto inst = func->insts_[i];
            TEST_TYPE(inst, AssignInst) {
                auto ptr = CAST_P(inst, AssignInst);
                int val;
                if (extractConst(func, ptr->rhs_, val)) {
                    ptr->rhs_ = std::make_shared<IntValue>(val);
                }
            } else TEST_TYPE(inst, ArrayAssignInst) {
                auto ptr = CAST_P(inst, ArrayAssignInst);
                int val;
                if (extractConst(func, ptr->rhs_, val)) {
                    ptr->rhs_ = std::make_shared<IntValue>(val);
                }
            } else TEST_TYPE(inst, AssignArrayInst) {
                auto ptr = CAST_P(inst, AssignArrayInst);
                int val;
                if (extractConst(func, ptr->rhs_index_, val)) {
                    ptr->rhs_index_ = std::make_shared<IntValue>(val);
                }
            } else TEST_TYPE(inst, BinaryInst) {
                auto ptr = CAST_P(inst, BinaryInst);
                int val;
                if (extractConst(func, ptr->rhs_1_, val)) {
                    ptr->rhs_1_ = std::make_shared<IntValue>(val);
                }
                if (extractConst(func, ptr->rhs_2_, val)) {
                    ptr->rhs_2_ = std::make_shared<IntValue>(val);
                }
                TEST_TYPE(ptr->rhs_1_, IntValue) {
                    auto ptr1 = CAST_P(ptr->rhs_1_, IntValue);
                    TEST_TYPE(ptr->rhs_2_, IntValue) {
                        auto ptr2 = CAST_P(ptr->rhs_2_, IntValue);
                        if (ptr->op_ == Operator::kAdd) {
                            auto val = std::make_shared<IntValue>
                                (ptr1->val_ + ptr2->val_);
                            auto inst = std::make_shared<AssignInst>
                                (ptr->lhs_, val);
                            func->insts_[i] = inst;
                        } else if (ptr->op_ == Operator::kSub) {
                            auto val = std::make_shared<IntValue>
                                (ptr1->val_ - ptr2->val_);
                            auto inst = std::make_shared<AssignInst>
                                (ptr->lhs_, val);
                            func->insts_[i] = inst;
                        } else if (ptr->op_ == Operator::kMul) {
                            auto val = std::make_shared<IntValue>
                                (ptr1->val_ * ptr2->val_);
                            auto inst = std::make_shared<AssignInst>
                                (ptr->lhs_, val);
                            func->insts_[i] = inst;
                        } else if (ptr->op_ == Operator::kDiv) {
                            auto val = std::make_shared<IntValue>
                                (ptr1->val_ / ptr2->val_);
                            auto inst = std::make_shared<AssignInst>
                                (ptr->lhs_, val);
                            func->insts_[i] = inst;
                        } else if (ptr->op_ == Operator::kMod) {
                            auto val = std::make_shared<IntValue>
                                (ptr1->val_ % ptr2->val_);
                            auto inst = std::make_shared<AssignInst>
                                (ptr->lhs_, val);
                            func->insts_[i] = inst;
                        }
                    } else {
                        if (ptr->op_ == Operator::kAdd) {
                            if (ptr1->val_ == 0) {
                                auto inst = std::make_shared<AssignInst>
                                    (ptr->lhs_, ptr->rhs_2_);
                                func->insts_[i] = inst;
                            }
                        } else if (ptr->op_ == Operator::kSub) {
                            if (ptr1->val_ == 0) {
                                auto inst = std::make_shared<UnaryInst>
                                    (Operator::kSub, ptr->lhs_, ptr->rhs_2_);
                                func->insts_[i] = inst;
                            }
                        } else if (ptr->op_ == Operator::kMul) {
                            if (ptr1->val_ == 0) {
                                auto val = std::make_shared<IntValue>(0);
                                auto inst = std::make_shared<AssignInst>
                                    (ptr->lhs_, val);
                                func->insts_[i] = inst;
                            } else if (ptr1->val_ == 1) {
                                auto inst = std::make_shared<AssignInst>
                                    (ptr->lhs_, ptr->rhs_2_);
                                func->insts_[i] = inst;
                            }
                        }
                    }
                } else {
                    TEST_TYPE(ptr->rhs_2_, IntValue) {
                        auto ptr2 = CAST_P(ptr->rhs_2_, IntValue);
                        if (ptr->op_ == Operator::kAdd) {
                            if (ptr2->val_ == 0) {
                                auto inst = std::make_shared<AssignInst>
                                    (ptr->lhs_, ptr->rhs_1_);
                                func->insts_[i] = inst;
                            }
                        } else if (ptr->op_ == Operator::kSub) {
                            if (ptr2->val_ == 0) {
                                auto inst = std::make_shared<AssignInst>
                                    (ptr->lhs_, ptr->rhs_1_);
                                func->insts_[i] = inst;
                            }   
                        } else if (ptr->op_ == Operator::kMul) {
                            if (ptr2->val_ == 0) {
                                auto val = std::make_shared<IntValue>(0);
                                auto inst = std::make_shared<AssignInst>
                                    (ptr->lhs_, val);
                                func->insts_[i] = inst;
                            } else if (ptr2->val_ == 1) {
                                auto inst = std::make_shared<AssignInst>
                                    (ptr->lhs_, ptr->rhs_1_);
                                func->insts_[i] = inst;
                            }
                        } else if (ptr->op_ == Operator::kDiv) {
                            if (ptr2->val_ == 1) {
                                auto inst = std::make_shared<AssignInst>
                                    (ptr->lhs_, ptr->rhs_1_);
                                func->insts_[i] = inst;
                            }
                        }
                        
                    }
                }
            } else TEST_TYPE(inst, UnaryInst) {
                auto ptr = CAST_P(inst, UnaryInst);
                int val;
                if (extractConst(func, ptr->rhs_, val)) {
                    ptr->rhs_ = std::make_shared<IntValue>(val);
                }
                TEST_TYPE(ptr->rhs_, IntValue) {
                    auto rhs = CAST_P(ptr->rhs_, IntValue);
                    if (ptr->op_ == Operator::kSub) {
                        auto val = std::make_shared<IntValue>(-rhs->val_);
                        auto inst = std::make_shared<AssignInst>
                            (ptr->lhs_, val);
                        func->insts_[i] = inst;
                    }
                }
            } else TEST_TYPE(inst, CondInst) {
                auto ptr = CAST_P(inst, CondInst);
                int val;
                if (extractConst(func, ptr->cond_1_, val)) {
                    ptr->cond_1_ = std::make_shared<IntValue>(val);
                }
                if (extractConst(func, ptr->cond_2_, val)) {
                    ptr->cond_2_ = std::make_shared<IntValue>(val);
                }
                TEST_TYPE(ptr->cond_1_, IntValue) {
                    TEST_TYPE(ptr->cond_2_, IntValue) {
                        auto ptr1 = CAST_P(ptr->cond_1_, IntValue);
                        auto ptr2 = CAST_P(ptr->cond_2_, IntValue);
                        bool ans;
                        if (ptr->op_ == Operator::kEq) {
                            ans = ptr1->val_ == ptr2->val_;
                        } else if (ptr->op_ == Operator::kNeq) {
                            ans = ptr1->val_ != ptr2->val_;
                        } else if (ptr->op_ == Operator::kLess) {
                            ans = ptr1->val_ < ptr2->val_;
                        } else if (ptr->op_ == Operator::kLessEq) {
                            ans = ptr1->val_ <= ptr2->val_;
                        } else if (ptr->op_ == Operator::kGreater) {
                            ans = ptr1->val_ > ptr2->val_;
                        } else if (ptr->op_ == Operator::kGreaterEq) {
                            ans = ptr1->val_ >= ptr2->val_;
                        } else assert(false);
                        if (ans) {
                            auto inst = std::make_shared<JumpInst>
                                (ptr->label_id_);
                            func->insts_[i] = inst;
                        } else {
                            erased.push_back(i);
                        }
                    }
                }
            } else TEST_TYPE(inst, ParamInst) {
                auto ptr = CAST_P(inst, ParamInst);
                int val;
                if (extractConst(func, ptr->param_, val)) {
                    ptr->param_ = std::make_shared<IntValue>(val);
                }
            } else TEST_TYPE(inst, ReturnInst) {
                auto ptr = CAST_P(inst, ReturnInst);
                if (ptr->ret_ != nullptr) {
                    int val;
                    if (extractConst(func, ptr->ret_, val)) {
                        ptr->ret_ = std::make_shared<IntValue>(val);
                    }
                }
            }
        }
        removeUnnecessary(func, erased);
    }
}

bool sameVar(VarPtr var1, VarPtr var2) {
    TEST_TYPE(var1, NativeVar) {
        auto ptr1 = CAST_P(var1, NativeVar);
        TEST_TYPE(var2, NativeVar) {
            auto ptr2 = CAST_P(var2, NativeVar);
            if (ptr1->id_ == ptr2->id_)
                return true;
        }
    } else TEST_TYPE(var1, TempVar) {
        auto ptr1 = CAST_P(var1, TempVar);
        TEST_TYPE(var2, TempVar) {
            auto ptr2 = CAST_P(var2, TempVar);
            if (ptr1->id_ == ptr2->id_) 
                return true;
        }
    } else TEST_TYPE(var1, ParamVar) {
        auto ptr1 = CAST_P(var1, ParamVar);
        TEST_TYPE(var2, ParamVar) {
            auto ptr2 = CAST_P(var2, ParamVar);
            if (ptr1->id_ == ptr2->id_) 
                return true;
        }
    } else TEST_TYPE(var1, IntValue) {
        auto ptr1 = CAST_P(var1, IntValue);
        TEST_TYPE(var2, IntValue) {
            auto ptr2 = CAST_P(var2, IntValue);
            if (ptr1->val_ == ptr2->val_) 
                return true;
        }
    }; 
    return false;
}

void eliminateDeadCode(Program &ir) {
    for (auto &func : ir.funcs()) {
        func->eliminateDead();
    }
    for (auto &func : ir.funcs()) {
        std::vector<int> erased;
        for (int i = 0; i < func->instNum(); ++i) {
            auto inst = func->insts_[i];
            TEST_TYPE(inst, AssignInst) {
                auto ptr = CAST_P(inst, AssignInst);
                if (sameVar(ptr->lhs_, ptr->rhs_))
                    erased.push_back(i);
            }
        }
        removeUnnecessary(func, erased);
    }
}

void eliminateRedundantGoto(Program &ir) {
    for (auto &func : ir.funcs()) {
        std::vector<int> erased;
        for (int i = 0; i < func->instNum(); ++i) {
            auto inst = func->insts_[i];
            TEST_TYPE(inst, JumpInst) {
                auto ptr = CAST_P(inst, JumpInst);
                if (func->label_pos_[ptr->label_id_] == i + 1) {
                    erased.push_back(i);
                }
            }
        }
        removeUnnecessary(func, erased);
    }
    for (auto &func : ir.funcs()) {
        for (auto &inst : func->insts()) {
            TEST_TYPE(inst, CondInst) {
                auto ptr = CAST_P(inst, CondInst);
                auto id = func->label_pos_[ptr->label_id_];
                if (id < func->instNum()) {
                    auto nxt_inst = func->insts()[id];
                    TEST_TYPE(nxt_inst, JumpInst) {
                        auto ptr2 = CAST_P(nxt_inst, JumpInst);
                        ptr->label_id_ = ptr2->label_id_;
                    }
                }
            } TEST_TYPE(inst, JumpInst) {
                auto ptr = CAST_P(inst, JumpInst);
                auto id = func->label_pos_[ptr->label_id_];
                if (id < func->instNum()) {
                    auto nxt_inst = func->insts()[id];
                    TEST_TYPE(nxt_inst, JumpInst) {
                        auto ptr2 = CAST_P(nxt_inst, JumpInst);
                        ptr->label_id_ = ptr2->label_id_;
                    }
                }
            }
        }
    }
}

void peephole(Program &ir) {
    global = ir.funcs().back();
    numG = global->nativeNum();
    precalculation(ir);
    eliminateRedundantGoto(ir);
    eliminateDeadCode(ir);
}

#undef TEST_TYPE
#undef CAST_P

}
