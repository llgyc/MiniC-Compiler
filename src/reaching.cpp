#include <map>
#include <memory>
#include <iostream>

#include "utils.hpp"
#include "eeyore.hpp"
#include "liveness.hpp"
#include "data_flow.hpp"

namespace reaching {

static int cnt;
static int init_num;
static std::map<int, eeyore::InstPtr> id2inst;

#define CAST_P(POINTER, TYPE) \
dynamic_cast<eeyore::TYPE *>(POINTER.get())

#define TEST_TYPE(POINTER, TYPE) \
if (CAST_P(POINTER, TYPE) != nullptr)

void local_ident(eeyore::FunctionDef *func, std::set<int> &s) {
    s.clear();
}

void block_init(eeyore::FunctionDef *func, std::set<int> &s) {
    s.clear();
}

void global_init(eeyore::FunctionDef *func, std::set<int> &s) {
    s.clear();
    for (int i = 0; i < init_num; ++i) s.insert(i);
}

std::set<int> gen(eeyore::FunctionDef *func, eeyore::InstPtr inst) {
    std::set<int> ret; 
    TEST_TYPE(inst, AssignInst) {
        ret.insert(inst->def_id_);
    } else TEST_TYPE(inst, AssignArrayInst) {
        ret.insert(inst->def_id_);
    } else TEST_TYPE(inst, BinaryInst) {
        ret.insert(inst->def_id_);
    } else TEST_TYPE(inst, UnaryInst) {
        ret.insert(inst->def_id_);
    } else TEST_TYPE(inst, AssignCallInst) {
        auto ptr = CAST_P(inst, AssignCallInst);
        if (ptr->store_ != nullptr)
            ret.insert(inst->def_id_);
    }
    return ret;
}

bool sameVar(eeyore::VarPtr var1, eeyore::VarPtr var2) {
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

std::set<int> calcKill(eeyore::VarPtr now) {
    std::set<int> ret;
    for (int i = 0; i < cnt; ++i) {
        auto inst = id2inst[i];
        TEST_TYPE(inst, AssignInst) {
            auto ptr = CAST_P(inst, AssignInst);
            auto tmp = ptr->lhs_;
            if (sameVar(now, tmp)) ret.insert(i);
        } else TEST_TYPE(inst, AssignArrayInst) {
            auto ptr = CAST_P(inst, AssignArrayInst);
            auto tmp = ptr->lhs_;
            if (sameVar(now, tmp)) ret.insert(i);
        } else TEST_TYPE(inst, BinaryInst) {
            auto ptr = CAST_P(inst, BinaryInst);
            auto tmp = ptr->lhs_;
            if (sameVar(now, tmp)) ret.insert(i);
        } else TEST_TYPE(inst, UnaryInst) {
            auto ptr = CAST_P(inst, UnaryInst);
            auto tmp = ptr->lhs_;
            if (sameVar(now, tmp)) ret.insert(i);
        } else TEST_TYPE(inst, AssignCallInst) {
            auto ptr = CAST_P(inst, AssignCallInst);
            if (ptr->store_ != nullptr) {
                auto tmp = ptr->store_;
                if (sameVar(now, tmp)) ret.insert(i);
            }
        }
    }
    return ret;
}

std::set<int> kill(eeyore::FunctionDef *func, eeyore::InstPtr inst) {
    std::set<int> ret;
    TEST_TYPE(inst, AssignInst) {
        auto ptr = CAST_P(inst, AssignInst);
        ret = calcKill(ptr->lhs_);
    } else TEST_TYPE(inst, AssignArrayInst) {
        auto ptr = CAST_P(inst, AssignArrayInst);
        ret = calcKill(ptr->lhs_);
    } else TEST_TYPE(inst, BinaryInst) {
        auto ptr = CAST_P(inst, BinaryInst);
        ret = calcKill(ptr->lhs_);
    } else TEST_TYPE(inst, UnaryInst) {
        auto ptr = CAST_P(inst, UnaryInst);
        ret = calcKill(ptr->lhs_);
    } else TEST_TYPE(inst, AssignCallInst) {
        auto ptr = CAST_P(inst, AssignCallInst);
        if (ptr->store_ != nullptr) {
            ret = calcKill(ptr->store_);
        }
    }
    return ret;
}

data_flow::Description desc {
    &gen,
    &kill,
    &local_ident,
    &block_init,
    &global_init,
    &data_flow::mUnion
};

int usable(eeyore::VarPtr var, std::set<int> &s, eeyore::VarPtr &ret) {
    ret = nullptr;
    int type = 0;
    for (auto x : s) {
        auto inst = id2inst[x];
        TEST_TYPE(inst, AssignInst) {
            auto ptr = CAST_P(inst, AssignInst);
            if (sameVar(ptr->lhs_, var)) {
                if (type == 0) {
                    ret = ptr->rhs_;
                    type = 1;
                } else if (type != 1) {
                    return 0;
                } else if (sameVar(ptr->rhs_, ret)) {
                    // Do nothing!
                } else return 0;
            }
        } else TEST_TYPE(inst, AssignArrayInst) {
            auto ptr = CAST_P(inst, AssignArrayInst);
            if (sameVar(ptr->lhs_, var)) return 0;
        } else TEST_TYPE(inst, BinaryInst) {
            auto ptr = CAST_P(inst, BinaryInst);
            if (sameVar(ptr->lhs_, var)) return 0;
        } else TEST_TYPE(inst, UnaryInst) {
            auto ptr = CAST_P(inst, UnaryInst);
            if (sameVar(ptr->lhs_, var)) return 0;
        } else TEST_TYPE(inst, AssignCallInst) {
            auto ptr = CAST_P(inst, AssignCallInst);
            if (ptr->store_ != nullptr) {
                if (sameVar(ptr->store_, var)) return 0;
            }
        } else assert(false);
    }
    // Only has an "unknown" initial value
    if (type) {
        if (ret == nullptr) return 0;
        TEST_TYPE(ret, IntValue) {
            // OK
        } else return 0; 
    }
    return type;
}

void reconstruct(eeyore::FuncPtr func) {
    for (int i = 0; i < func->instNum(); ++i) {
        auto inst = func->insts_[i];
        eeyore::VarPtr res; 
        TEST_TYPE(inst, AssignInst) {
            auto ptr = CAST_P(inst, AssignInst);
            int flag = usable(ptr->rhs_, inst->data_in_, res);
            if (flag == 1) {
                ptr->rhs_ = res;
            }
        } else TEST_TYPE(inst, ArrayAssignInst) {
            auto ptr = CAST_P(inst, ArrayAssignInst);
            int flag = usable(ptr->rhs_, inst->data_in_, res);
            if (flag == 1) {
                ptr->rhs_ = res;
            }
        } else TEST_TYPE(inst, AssignArrayInst) {
            auto ptr = CAST_P(inst, AssignArrayInst);
            int flag1 = usable(ptr->rhs_var_, inst->data_in_, res);
            if (flag1 == 1) {
                ptr->rhs_var_ = res;
            }
            int flag2 = usable(ptr->rhs_index_, inst->data_in_, res);
            if (flag2 == 1) {
                ptr->rhs_index_ = res;
            }
        } else TEST_TYPE(inst, BinaryInst) {
            auto ptr = CAST_P(inst, BinaryInst);
            int flag1 = usable(ptr->rhs_1_, inst->data_in_, res);
            if (flag1 == 1) {
                ptr->rhs_1_ = res;
            }
            int flag2 = usable(ptr->rhs_2_, inst->data_in_, res);
            if (flag2 == 1) {
                ptr->rhs_2_ = res;
            }
        } else TEST_TYPE(inst, UnaryInst) {
            auto ptr = CAST_P(inst, UnaryInst);
            int flag = usable(ptr->rhs_, inst->data_in_, res);
            if (flag == 1) {
                ptr->rhs_ = res;
            }
        } else TEST_TYPE(inst, CondInst) {
            auto ptr = CAST_P(inst, CondInst);
            int flag1 = usable(ptr->cond_1_, inst->data_in_, res);
            if (flag1 == 1) {
                ptr->cond_1_ = res;
            }
            int flag2 = usable(ptr->cond_2_, inst->data_in_, res);
            if (flag2 == 1) {
                ptr->cond_2_ = res;
            }
        } else TEST_TYPE(inst, JumpInst) {
            // Do nothing!
        } else TEST_TYPE(inst, ParamInst) {
            auto ptr = CAST_P(inst, ParamInst);
            int flag = usable(ptr->param_, inst->data_in_, res);
            if (flag == 1) {
                ptr->param_ = res;
            }
        } else TEST_TYPE(inst, AssignCallInst) {
            // Do nothing!
        } else TEST_TYPE(inst, ReturnInst) {
            auto ptr = CAST_P(inst, ReturnInst);
            if (ptr->ret_ != nullptr) {
                int flag = usable(ptr->ret_, inst->data_in_, res);
                if (flag == 1) {
                    ptr->ret_ = res;
                }
            }
        } else assert(false);
    }
}

void optimize(eeyore::Program &ir) {
    assert(ir.funcs().back()->name() == "$global$");
    auto global = ir.funcs().back();
    int numG = global->nativeNum();
    std::vector<int> last_size;
    for (auto &func : ir.funcs()) last_size.push_back(func->instNum());

    while (true) {
        for (auto &func : ir.funcs()) {
            cnt = 0;
            // Global variable and parameter have initial "unknown" values
            for (int i = 0; i < numG; ++i) {
                auto inst = std::make_shared<eeyore::AssignInst>
                    (global->native_[i], nullptr);
                id2inst[cnt] = inst;
                inst->def_id_ = cnt++;
            }
            for (int i = 0; i < func->paramNum(); ++i) {
                auto inst = std::make_shared<eeyore::AssignInst>
                    (func->params_[i], nullptr);
                id2inst[cnt] = inst;
                inst->def_id_ = cnt++;
            }
            init_num = numG + func->paramNum();
            for (auto &inst : func->insts()) {
                TEST_TYPE(inst, AssignInst) {
                    id2inst[cnt] = inst;
                    inst->def_id_ = cnt++;
                } else TEST_TYPE(inst, AssignArrayInst) {
                    id2inst[cnt] = inst;
                    inst->def_id_ = cnt++;
                } else TEST_TYPE(inst, BinaryInst) {
                    id2inst[cnt] = inst;
                    inst->def_id_ = cnt++;
                } else TEST_TYPE(inst, UnaryInst) {
                    id2inst[cnt] = inst;
                    inst->def_id_ = cnt++;
                } else TEST_TYPE(inst, AssignCallInst) {
                    auto ptr = CAST_P(inst, AssignCallInst);
                    if (ptr->store_ != nullptr) {
                        id2inst[cnt] = inst;
                        inst->def_id_ = cnt++;
                    }
                }
            }
            func->forwardAccess(desc);
            reconstruct(func);
        }
        liveness::optimize(ir, -1);

        std::vector<int> now_size;
        for (auto &func : ir.funcs()) now_size.push_back(func->instNum());
        assert(last_size.size() == now_size.size());
        bool flag = false;
        for (unsigned i = 0; i < last_size.size(); ++i) {
            if (now_size[i] != last_size[i]) {
                flag = true;
                break;
            }
        }
        if (!flag) break;
        last_size = now_size;
    }
}

#undef TEST_TYPE
#undef CAST_P

} // namespace reaching
