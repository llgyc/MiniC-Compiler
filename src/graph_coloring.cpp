#include <set>
#include <map>
#include <stack>
#include <cmath>
#include <vector>
#include <cassert>
#include <utility>
#include <algorithm>

#include "loop.hpp"
#include "eeyore.hpp"
#include "tigger.hpp"
#include "operator.hpp"
#include "liveness.hpp"

namespace graph_coloring {

static int global_num;
static int param_pos;
static int total_var;

static int numG;
static int numN;
static int numT;
static int numP;

static std::vector<int> weight;

#define CAST_P(POINTER, TYPE) \
dynamic_cast<eeyore::TYPE *>(POINTER.get())

#define TEST_TYPE(POINTER, TYPE) \
if (CAST_P(POINTER, TYPE) != nullptr)

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

const int kWeightMax = 1000000000;

bool weight_cmp(int x, int y) {
    return weight[x] > weight[y];
}

void addWeight(std::set<int> &s, int val) {
    int val_ = (int)std::pow(12, std::min(val, 8));
    for (auto x : s) {
        weight[x] += val_;
        weight[x] = std::min(weight[x], kWeightMax);
    }
}

void calculateWeight(eeyore::FuncPtr func) {
    // Initialization
    weight.clear();
    total_var = numN + numT + numP;
    for (int i = 0; i < total_var; ++i) weight.push_back(0);

    // Loop Level Detection
    std::vector<int> delta;
    for (int i = 0; i <= func->instNum(); ++i) delta.push_back(0);
    for (auto x : func->loop_pos_) {
        delta[x.first]++;
        delta[x.second + 1]--;
    }

    // Final Calculation
    liveness::setG(numG);
    liveness::setN(numN);
    liveness::setT(numT);
    liveness::setP(numP);
    int level = 0;
    for (int i = 0; i < func->instNum(); ++i) {
        level += delta[i];
        auto inst = func->insts_[i];
        std::set<int> temp;
        temp = liveness::def(func.get(), inst);
        addWeight(temp, level);
        temp = liveness::use(func.get(), inst);
        addWeight(temp, level);
    }
}

// Register Available
// 1-12 16-27
// 0 - zero register
// 13(t0) - left for RISC-V
// 14(t1) - left for partial result
// 15(t2) - left for partial result (?)
const int kRegAvail = 25;

// For array
// - in_reg = address in register
// - reg_pos = which register
// - stk_pos = where in stack
// For variable
// - in_reg = value in register
// - reg_pos = which register
// - stk_pos = where to store in stack (for spilling)
class VarInfo {
public:
    bool is_array;
    bool in_reg;
    int reg_pos, stk_pos;
};

static int now_pos;
std::map<int, VarInfo> var2reg;
std::map<int, int> reg2stk;
std::vector<std::set<int>> edges;
std::vector<int> degree;
std::vector<int> father;
std::vector<int> color;

int getfather(int x) {
    return (father[x] == x) ? x : (father[x] = getfather(father[x]));
}

void unify(int x, int y) {
    if (x == -1 || y == -1) return;
    int u = getfather(x), v = getfather(y);
    if (u == v) return;
    father[u] = v;
}

std::set<int> ret_use;
void dye(int var) {
    bool used[28] = {};
    for (auto x : edges[var]) {
        if (var2reg[x].in_reg) {
            used[var2reg[x].reg_pos] = 1;
        }
    }

    auto anc = getfather(var);
    auto tmp = color[anc];

    // Save parameters in their original place
    if (var >= numN + numT) {
        auto id = 20 + var - numN - numT;
        if (!used[id]) {
            var2reg[var].in_reg = true;
            var2reg[var].reg_pos = id;
            if (tmp == -1) color[anc] = id;
            return;
        }
    }
    /*
    if (tmp != -1 && !used[tmp]) {
        var2reg[var].in_reg = true;
        var2reg[var].reg_pos = tmp;
        return;
    }
    */

    // Save return value in a0
    if (ret_use.find(var) != ret_use.end()) {
        if (!used[20]) {
            var2reg[var].in_reg = true;
            var2reg[var].reg_pos = 20;
            if (tmp == -1) color[anc] = 20;
            return;
        }
    }
    for (int i = 1; i <= 27; ++i) {
        if (i == 13) continue;
        if (i == 14) continue;
        // if (i == 15) continue;
        if (used[i]) continue;
        var2reg[var].in_reg = true;
        var2reg[var].reg_pos = i;
        if (tmp == -1) color[anc] = i;
        return;
    }
    assert(false);
}

void registerAllocation(eeyore::FuncPtr func) {
    // Initialization 
    std::set<int> empty_set; 
    edges.clear(); degree.clear();
    for (int i = 0; i < total_var; ++i) {
        edges.push_back(empty_set);
        degree.push_back(0);
    }
    ret_use.clear();
    for (auto &inst : func->insts()) {
        TEST_TYPE(inst, ReturnInst) {
            auto ptr = CAST_P(inst, ReturnInst);
            if (ptr->ret_ != nullptr) {
                auto id = getID(ptr->ret_);
                if (id != -1) ret_use.insert(id);
            }
        }
    }
    bool leaf_node = true;
    for (auto &inst : func->insts()) {
        TEST_TYPE(inst, AssignCallInst) {
            leaf_node = false;
            break;
        }
    }
    father.clear(); color.clear();
    for (int i = 0; i < total_var; ++i) {
        father.push_back(i);
        color.push_back(-1);
    }
    for (auto &inst : func->insts()) {
        TEST_TYPE(inst, AssignInst) {
            auto ptr = CAST_P(inst, AssignInst);
            unify(getID(ptr->lhs_), getID(ptr->rhs_));
        }
    }

    // Variable preprocessing
    // Allocate stack position for array but not variables
    int true_var = 0;
    now_pos = 0; var2reg.clear();
    if (func->name() != "$global$") {
        for (auto &var : func->native()) {
            auto ptr = CAST_P(var, NativeVar);
            VarInfo tmp; 
            tmp.in_reg = false;
            tmp.reg_pos = tmp.stk_pos = -1;
            if (ptr->widths_.size() == 0) {
                tmp.is_array = false;
                ++true_var;
            } else {
                tmp.is_array = true;
                tmp.stk_pos = now_pos;
                now_pos += ptr->widths_[0];
            }
            var2reg[getID(var)] = tmp;
        }
    }
    for (auto &var : func->temp()) {
        VarInfo tmp;
        tmp.is_array = tmp.in_reg = false;
        tmp.reg_pos = tmp.stk_pos = -1;
        ++true_var;
        var2reg[getID(var)] = tmp;
    }
    for (auto &var : func->params()) {
        VarInfo tmp;
        tmp.is_array = tmp.in_reg = false;
        tmp.reg_pos = tmp.stk_pos = -1;
        ++true_var;
        var2reg[getID(var)] = tmp;
    }

    // Build Edges
    for (int i = 0; i < func->instNum(); ++i) {
        auto inst = func->insts_[i];
        std::vector<int> tmp;
        for (auto x : inst->data_out_) tmp.push_back(x);
        for (unsigned j = 0; j < tmp.size(); ++j) {
            if (var2reg[tmp[j]].is_array) continue;
            for (unsigned k = j + 1; k < tmp.size(); ++k) {
                if (var2reg[tmp[k]].is_array) continue;
                edges[tmp[j]].insert(tmp[k]);
                edges[tmp[k]].insert(tmp[j]);
            }
        }
    }

    // Coloring
    // First allocate register for variables
    // If there are abundant registers, allocate for arrays
    std::stack<int> stk;
    std::vector<bool> used;
    for (int i = 0; i < total_var; ++i) used.push_back(false);
    int cnt = true_var;
    while (cnt--) {
        int now = -1;
        for (int i = 0; i < total_var; ++i) {
            if (var2reg[i].is_array) continue;
            if (used[i]) continue;
            if (edges[i].size() < kRegAvail) {
                now = i;
                break;
            }
        }
        // Every node has degree >= kRegAvail
        if (now == -1) {
            int min_weight = kWeightMax + 1, min_var = -1;
            for (int i = 0; i < total_var; ++i) {
                if (var2reg[i].is_array) continue;
                if (used[i]) continue;
                if (weight[i] < min_weight) {
                    min_weight = weight[i];
                    min_var = i;
                }
            }
            assert(min_weight != kWeightMax + 1);
            assert(min_var != -1);
            // Spill this variable
            used[min_var] = true;
            var2reg[min_var].stk_pos = now_pos;
            ++now_pos;
            continue;
        }

        // Delete a node
        used[now] = true;
        stk.push(now);
        for (auto x : edges[now]) {
            edges[x].erase(now);
        }
    }

    // Pop from stack
    while (!stk.empty()) {
        int now = stk.top(); stk.pop();
        dye(now);
    }

    // Allocate remaining used register for arrays
    func->used_register_.clear();
    std::vector<int> remains;
    for (int i = 0; i < total_var; ++i) {
        if (var2reg[i].is_array) remains.push_back(i);
        if (var2reg[i].in_reg) func->used_register_.insert(var2reg[i].reg_pos);
    }
    sort(remains.begin(), remains.end(), weight_cmp);
    std::vector<int> remain_regs;
    for (int i = 1; i <= 27; ++i) {
        if (i == 13) continue;
        if (i == 14) continue;
        // if (i == 15) continue;
        if (func->used_register_.find(i) != func->used_register_.end())
            continue;
        remain_regs.push_back(i);
    }
    for (unsigned i = 0; i < std::min(remains.size(), remain_regs.size()); ++i) {
        var2reg[remains[i]].in_reg = true;
        var2reg[remains[i]].reg_pos = remain_regs[i];
        func->used_register_.insert(remain_regs[i]);
    }

    // Place for saving registers
    reg2stk.clear();
    if (!leaf_node) {
        for (auto x : func->used_register_) {
            reg2stk[x] = now_pos;
            ++now_pos;
        }
    }

    // Fully Calculated
}

int isPow2(int val) {
    int ret = 0;
    if (val <= 0) return -1;
    while (val != 1) {
        ++ret;
        if (val & 1) return -1;
        val >>= 1;
    }
    return ret;
}

void generateTiggerCode(eeyore::FuncPtr func, tigger::Program &dst) {
    func->used_register_union_ = func->used_register_;
    for (int i = 0; i < func->paramNum(); ++i) {
        func->used_register_union_.insert(20 + i);
    }
    for (auto &inst : func->insts()) {
        TEST_TYPE(inst, AssignCallInst) {
            auto ptr = CAST_P(inst, AssignCallInst);
            data_flow::mUnion(func->used_register_union_,
                ptr->func_->used_register_union_);
        }
    }

    auto t_func = std::make_shared<tigger::FunctionDef>
        (func->name(), func->paramNum(), now_pos);
    dst.pushFunction(t_func);

    #define PUSH_INST(INST) \
    t_func->pushInst(std::move(INST))

    // Save & Move Parameters
    for (int i = 0; i < func->paramNum(); ++i) {
        auto var = func->params_[i];
        auto id = getID(var);
        auto info = var2reg[id];
        if (info.in_reg) {
            // Not in the same register
            if (info.reg_pos != 20 + i) {
                auto inst = std::make_shared<tigger::StackStoreInst>
                    (20 + i, reg2stk[info.reg_pos]);
                PUSH_INST(inst);
            }
        } else {
            auto inst = std::make_shared<tigger::StackStoreInst>
                (20 + i, info.stk_pos);
            PUSH_INST(inst);
        }
    }
    for (int i = 0; i < func->paramNum(); ++i) {
        auto var = func->params_[i];
        auto id = getID(var);
        auto info = var2reg[id];
        if (info.in_reg && info.reg_pos != 20 + i) {
            auto inst = std::make_shared<tigger::StackLoadInst>
                (reg2stk[info.reg_pos], info.reg_pos);
            PUSH_INST(inst);
        }
    }

    // Load Local Array Address
    for (int i = 0; i < total_var; ++i) {
        auto info = var2reg[i];
        if (!info.is_array) continue;
        if (!info.in_reg) continue;
        auto inst = std::make_shared<tigger::StackAddrLoadInst>
            (info.stk_pos, info.reg_pos);
        PUSH_INST(inst);
    }

    #define LOAD_SYMBOL(SYMBOL, REG) \
    { \
        auto _id = getID(SYMBOL); \
        if (_id == -1) { \
            TEST_TYPE(SYMBOL, NativeVar) { \
                auto _ptr = CAST_P(SYMBOL, NativeVar); \
                if (_ptr->widths_.size() == 0) { \
                    auto _inst = std::make_shared<tigger::VarLoadInst> \
                        (_ptr->id_, REG); \
                    PUSH_INST(_inst); \
                } else { \
                    auto _inst = std::make_shared<tigger::VarAddrLoadInst> \
                        (_ptr->id_, REG); \
                    PUSH_INST(_inst); \
                } \
                macro_result = REG; \
            } else TEST_TYPE(SYMBOL, IntValue) { \
                auto _ptr = CAST_P(SYMBOL, IntValue); \
                if (replace_flag &&_ptr->val_ == 0) { \
                    macro_result = 0; \
                } else { \
                    auto _inst = std::make_shared<tigger::AssignIInst> \
                        (REG, _ptr->val_); \
                    PUSH_INST(_inst); \
                    macro_result = REG; \
                } \
            } else assert(false); \
        } else { \
            auto _info = var2reg[_id]; \
            if (!_info.in_reg) { \
                if (_info.is_array) { \
                    auto _inst = std::make_shared<tigger::StackAddrLoadInst> \
                        (_info.stk_pos, REG); \
                    PUSH_INST(_inst); \
                } else { \
                    auto _inst = std::make_shared<tigger::StackLoadInst> \
                        (_info.stk_pos, REG); \
                    PUSH_INST(_inst); \
                } \
                macro_result = REG; \
            } else { \
                macro_result = _info.reg_pos; \
            } \
        } \
    }

    #define STORE_SYMBOL(SYMBOL, REG, REG2) \
    { \
        auto _id = getID(SYMBOL); \
        if (_id == -1) { \
            TEST_TYPE(SYMBOL, NativeVar) { \
                auto _ptr = CAST_P(SYMBOL, NativeVar); \
                if (_ptr->widths_.size() == 0) { \
                    auto _inst = std::make_shared<tigger::VarAddrLoadInst> \
                        (_ptr->id_, REG2); \
                    PUSH_INST(_inst); \
                    auto _inst2 = std::make_shared<tigger::MemStoreInst> \
                        (REG2, 0, REG); \
                    PUSH_INST(_inst2); \
                } else assert(false); \
            } else assert(false); \
        } else { \
            auto _info = var2reg[_id]; \
            if (!_info.in_reg) { \
                if (!_info.is_array) { \
                    auto _inst = std::make_shared<tigger::StackStoreInst> \
                        (REG, _info.stk_pos); \
                    PUSH_INST(_inst); \
                } else assert(false); \
            } else { \
                if (_info.reg_pos != REG) { \
                    auto _inst = std::make_shared<tigger::AssignRInst> \
                        (_info.reg_pos, REG); \
                    PUSH_INST(_inst); \
                } \
            } \
        } \
    }
    
    // Generate Instructions
    std::set<int> already_saved;
    t_func->setLabelNum(func->labelNum());
    for (int i = 0; i < func->instNum(); ++i) {
        auto inst = func->insts_[i];
        // Label Generation
        for (int j = 0; j < func->labelNum(); ++j) {
            if (i == func->label_pos()[j]) {
                t_func->setLabel(j, t_func->instNum());
            }
        }

        // Normal Instruction
        int macro_result;
        bool replace_flag = true;
        TEST_TYPE(inst, AssignInst) {
            // SYMBOL = RightValue
            auto ptr = CAST_P(inst, AssignInst);
            TEST_TYPE(ptr->rhs_, IntValue) {
                auto ptr2 = CAST_P(ptr->rhs_, IntValue);
                auto id = getID(ptr->lhs_);
                if (id != -1 && var2reg[id].in_reg) {
                    auto inst = std::make_shared<tigger::AssignIInst>
                        (var2reg[id].reg_pos, ptr2->val_);
                    PUSH_INST(inst);
                } else {
                    LOAD_SYMBOL(ptr->rhs_, 14);
                    STORE_SYMBOL(ptr->lhs_, macro_result, 13);
                }
            } else {
                auto id = getID(ptr->lhs_);
                if (id != -1 && var2reg[id].in_reg) {
                    auto id2 = getID(ptr->rhs_);
                    if (id2 != -1 && var2reg[id2].in_reg) {
                        if (var2reg[id].reg_pos == var2reg[id2].reg_pos) {
                            // At the Right Place
                            // Do nothing!
                        } else {
                            auto inst = std::make_shared<tigger::AssignRInst>
                                (var2reg[id].reg_pos, var2reg[id2].reg_pos);
                            PUSH_INST(inst);
                        }
                    } else {
                        LOAD_SYMBOL(ptr->rhs_, var2reg[id].reg_pos);
                        assert(macro_result == var2reg[id].reg_pos);
                    }
                } else {
                    LOAD_SYMBOL(ptr->rhs_, 14);
                    STORE_SYMBOL(ptr->lhs_, macro_result, 13);
                }
            }
        } else TEST_TYPE(inst, ArrayAssignInst) {
            // SYMBOL [ RightValue ] = RightValue
            auto ptr = CAST_P(inst, ArrayAssignInst);

            LOAD_SYMBOL(ptr->lhs_var_, 14);
            int reg1 = macro_result;
            TEST_TYPE(ptr->lhs_index_, IntValue) {
                auto ptr2 = CAST_P(ptr->lhs_index_, IntValue);
                LOAD_SYMBOL(ptr->rhs_, 13);
                int reg2 = macro_result;
                auto inst = std::make_shared<tigger::MemStoreInst>
                    (reg1, ptr2->val_, reg2);
                PUSH_INST(inst);
            } else {
                LOAD_SYMBOL(ptr->lhs_index_, 13);
                int reg2 = macro_result;
                auto inst = std::make_shared<tigger::BinaryRInst>
                    (Operator::kAdd, 14, reg1, reg2);
                PUSH_INST(inst);
                LOAD_SYMBOL(ptr->rhs_, 13);
                int reg3 = macro_result;
                auto inst2 = std::make_shared<tigger::MemStoreInst>
                    (14, 0, reg3);
                PUSH_INST(inst2);
            }
        } else TEST_TYPE(inst, AssignArrayInst) {
            // SYMBOL = SYMBOL [ RightValue ]
            auto ptr = CAST_P(inst, AssignArrayInst);
            LOAD_SYMBOL(ptr->rhs_var_, 14);
            int reg1 = macro_result;
            TEST_TYPE(ptr->rhs_index_, IntValue) {
                auto ptr2 = CAST_P(ptr->rhs_index_, IntValue);
                auto id = getID(ptr->lhs_);
                if (id != -1 && var2reg[id].in_reg) {
                    auto inst = std::make_shared<tigger::MemLoadInst>
                        (var2reg[id].reg_pos, reg1, ptr2->val_);
                    PUSH_INST(inst);
                } else {
                    auto inst = std::make_shared<tigger::MemLoadInst>
                        (14, reg1, ptr2->val_);
                    PUSH_INST(inst);
                    STORE_SYMBOL(ptr->lhs_, 14, 13);
                }
            } else {
                LOAD_SYMBOL(ptr->rhs_index_, 13);
                int reg2 = macro_result;
                auto inst = std::make_shared<tigger::BinaryRInst>
                    (Operator::kAdd, 14, reg1, reg2);
                PUSH_INST(inst);
                auto id = getID(ptr->lhs_);
                if (id != -1 && var2reg[id].in_reg) {
                    auto inst2 = std::make_shared<tigger::MemLoadInst>
                        (var2reg[id].reg_pos, 14, 0);
                    PUSH_INST(inst2);
                } else {
                    auto inst2 = std::make_shared<tigger::MemLoadInst>
                        (14, 14, 0);
                    PUSH_INST(inst2);
                    STORE_SYMBOL(ptr->lhs_, 14, 13);
                }
            }
        } else TEST_TYPE(inst, BinaryInst) {
            // SYMBOL = RightValue BinOp RightValue
            auto ptr = CAST_P(inst, BinaryInst);
            TEST_TYPE(ptr->rhs_1_, IntValue) {
                if (ptr->op_ == Operator::kMul) {
                    auto ptr2 = CAST_P(ptr->rhs_1_, IntValue);
                    auto expo = isPow2(ptr2->val_);
                    /* Add version */
                    /*if (expo == 1 || expo == 2) {
                        LOAD_SYMBOL(ptr->rhs_2_, 14);
                        int reg1 = macro_result;
                        if (expo == 2) {
                            auto inst = std::make_shared<tigger::BinaryRInst>
                                (Operator::kAdd, 14, reg1, reg1);
                            PUSH_INST(inst);
                            reg1 = 14;
                        }
                        auto id = getID(ptr->lhs_);
                        if (id != -1 && var2reg[id].in_reg) {
                            auto inst = std::make_shared<tigger::BinaryRInst>
                                (Operator::kAdd, var2reg[id].reg_pos, reg1, reg1);
                            PUSH_INST(inst);
                        } else {
                            auto inst = std::make_shared<tigger::BinaryRInst>
                                (Operator::kAdd, 14, reg1, reg1);
                            PUSH_INST(inst);
                            STORE_SYMBOL(ptr->lhs_, 14, 13);
                        }
                        continue;
                    }*/
                    if (expo != -1) {
                        LOAD_SYMBOL(ptr->rhs_2_, 14);
                        int reg1 = macro_result;
                        auto id = getID(ptr->lhs_);
                        if (id != -1 && var2reg[id].in_reg) {
                            auto inst = std::make_shared<tigger::BinaryIInst>
                                (Operator::kShl, var2reg[id].reg_pos, reg1, expo);
                            PUSH_INST(inst);
                        } else {
                            auto inst = std::make_shared<tigger::BinaryIInst>
                                (Operator::kShl, 14, reg1, expo);
                            PUSH_INST(inst);
                            STORE_SYMBOL(ptr->lhs_, 14, 13);
                        }
                        continue;
                    }
                }
                if (ptr->op_ == Operator::kAdd || ptr->op_ == Operator::kMul) {
                    auto ptr2 = CAST_P(ptr->rhs_1_, IntValue);
                    LOAD_SYMBOL(ptr->rhs_2_, 14);
                    int reg1 = macro_result;
                    auto id = getID(ptr->lhs_);
                    if (id != -1 && var2reg[id].in_reg) {
                        auto inst = std::make_shared<tigger::BinaryIInst>
                            (ptr->op_, var2reg[id].reg_pos, reg1, ptr2->val_);
                        PUSH_INST(inst);
                    } else {
                        auto inst = std::make_shared<tigger::BinaryIInst>
                            (ptr->op_, 14, reg1, ptr2->val_);
                        PUSH_INST(inst);
                        STORE_SYMBOL(ptr->lhs_, 14, 13);
                    }
                    continue;
                } 
            }
            TEST_TYPE(ptr->rhs_2_, IntValue) {
                if (ptr->op_ == Operator::kMul) {
                    auto ptr2 = CAST_P(ptr->rhs_2_, IntValue);
                    auto expo = isPow2(ptr2->val_);
                    /* Add version */
                    /*
                    if (expo == 1 || expo == 2) {
                        LOAD_SYMBOL(ptr->rhs_1_, 14);
                        int reg1 = macro_result;
                        if (expo == 2) {
                            auto inst = std::make_shared<tigger::BinaryRInst>
                                (Operator::kAdd, 14, reg1, reg1);
                            PUSH_INST(inst);
                            reg1 = 14;
                        }
                        auto id = getID(ptr->lhs_);
                        if (id != -1 && var2reg[id].in_reg) {
                            auto inst = std::make_shared<tigger::BinaryRInst>
                                (Operator::kAdd, var2reg[id].reg_pos, reg1, reg1);
                            PUSH_INST(inst);
                        } else {
                            auto inst = std::make_shared<tigger::BinaryRInst>
                                (Operator::kAdd, 14, reg1, reg1);
                            PUSH_INST(inst);
                            STORE_SYMBOL(ptr->lhs_, 14, 13);
                        }
                        continue;
                    }
                    */
                    if (expo != -1) {
                        LOAD_SYMBOL(ptr->rhs_1_, 14);
                        int reg1 = macro_result;
                        auto id = getID(ptr->lhs_);
                        if (id != -1 && var2reg[id].in_reg) {
                            auto inst = std::make_shared<tigger::BinaryIInst>
                                (Operator::kShl, var2reg[id].reg_pos, reg1, expo);
                            PUSH_INST(inst);
                        } else {
                            auto inst = std::make_shared<tigger::BinaryIInst>
                                (Operator::kShl, 14, reg1, expo);
                            PUSH_INST(inst);
                            STORE_SYMBOL(ptr->lhs_, 14, 13);
                        }
                        continue;
                    }
                }
                if (ptr->op_ == Operator::kDiv) {
                    auto ptr2 = CAST_P(ptr->rhs_2_, IntValue);
                    auto expo = isPow2(ptr2->val_);
                    if (expo != -1) {
                        LOAD_SYMBOL(ptr->rhs_1_, 14);
                        int reg1 = macro_result;
                        auto id = getID(ptr->lhs_);
                        if (id != -1 && var2reg[id].in_reg) {
                            auto inst = std::make_shared<tigger::BinaryIInst>
                                (Operator::kShr, var2reg[id].reg_pos, reg1, expo);
                            PUSH_INST(inst);
                        } else {
                            auto inst = std::make_shared<tigger::BinaryIInst>
                                (Operator::kShr, 14, reg1, expo);
                            PUSH_INST(inst);
                            STORE_SYMBOL(ptr->lhs_, 14, 13);
                        }
                        continue;
                    }
                }
                if (ptr->op_ == Operator::kMod) {
                    auto ptr2 = CAST_P(ptr->rhs_2_, IntValue);
                    auto expo = isPow2(ptr2->val_);
                    if (expo != -1) {
                        LOAD_SYMBOL(ptr->rhs_1_, 14);
                        int reg1 = macro_result;
                        auto id = getID(ptr->lhs_);
                        if (id != -1 && var2reg[id].in_reg) {
                            auto inst = std::make_shared<tigger::BinaryIInst>
                                (Operator::kAnd, var2reg[id].reg_pos, reg1, ptr2->val_ - 1);
                            PUSH_INST(inst);
                        } else {
                            auto inst = std::make_shared<tigger::BinaryIInst>
                                (Operator::kAnd, 14, reg1, ptr2->val_ - 1);
                            PUSH_INST(inst);
                            STORE_SYMBOL(ptr->lhs_, 14, 13);
                        }
                        continue;
                    }
                }
                auto ptr2 = CAST_P(ptr->rhs_2_, IntValue);
                LOAD_SYMBOL(ptr->rhs_1_, 14);
                int reg1 = macro_result;
                auto id = getID(ptr->lhs_);
                if (id != -1 && var2reg[id].in_reg) {
                    auto inst = std::make_shared<tigger::BinaryIInst>
                        (ptr->op_, var2reg[id].reg_pos, reg1, ptr2->val_);
                    PUSH_INST(inst);
                } else {
                    auto inst = std::make_shared<tigger::BinaryIInst>
                        (ptr->op_, 14, reg1, ptr2->val_);
                    PUSH_INST(inst);
                    STORE_SYMBOL(ptr->lhs_, 14, 13);
                }
            } else {
                LOAD_SYMBOL(ptr->rhs_1_, 14);
                int reg1 = macro_result;
                LOAD_SYMBOL(ptr->rhs_2_, 13);
                int reg2 = macro_result;
                auto id = getID(ptr->lhs_);
                if (id != -1 && var2reg[id].in_reg) {
                    auto inst = std::make_shared<tigger::BinaryRInst>
                        (ptr->op_, var2reg[id].reg_pos, reg1, reg2);
                    PUSH_INST(inst);
                } else {
                    auto inst = std::make_shared<tigger::BinaryRInst>
                        (ptr->op_, 14, reg1, reg2);
                    PUSH_INST(inst);
                    STORE_SYMBOL(ptr->lhs_, 14, 13);
                }
            }
        } else TEST_TYPE(inst, UnaryInst) {
            // SYMBOL = OP RightValue
            auto ptr = CAST_P(inst, UnaryInst);
            LOAD_SYMBOL(ptr->rhs_, 14);
            auto id = getID(ptr->lhs_);
            if (id != -1 && var2reg[id].in_reg) {
                auto inst = std::make_shared<tigger::UnaryInst>
                    (ptr->op_, var2reg[id].reg_pos, macro_result);
                PUSH_INST(inst);
            } else {
                auto inst = std::make_shared<tigger::UnaryInst>
                    (ptr->op_, 14, macro_result);
                PUSH_INST(inst);
                STORE_SYMBOL(ptr->lhs_, 14, 13);
            }
        } else TEST_TYPE(inst, CondInst) {
            // if RightValue LOGICOP RightValue goto LABEL
            auto ptr = CAST_P(inst, CondInst);
            LOAD_SYMBOL(ptr->cond_1_, 14);
            int reg1 = macro_result;
            LOAD_SYMBOL(ptr->cond_2_, 13);
            int reg2 = macro_result;
            auto inst = std::make_shared<tigger::CondInst>
                (ptr->op_, reg1, reg2, ptr->label_id_);
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

            // Save Register
            if (func->used_register_.find(20 + param_pos) != 
                func->used_register_.end()) {
                bool flag = false;
                for (auto x : inst->data_out_) {
                    if (!var2reg[x].in_reg) continue;
                    if (var2reg[x].reg_pos != 20 + param_pos) continue;
                    flag = true;
                    break;
                }
                if (flag) {
                    auto inst = std::make_shared<tigger::StackStoreInst>
                        (20 + param_pos, reg2stk[20 + param_pos]);
                    PUSH_INST(inst);
                    already_saved.insert(20 + param_pos);
                }
            }

            // Load Parameter into register 
            auto id = getID(ptr->param_);
            if (id != -1 && var2reg[id].in_reg) {
                // In register
                auto target = var2reg[id].reg_pos;
                if (target == 20 + param_pos) {
                    // Do nothing!
                } else if (already_saved.find(target) != already_saved.end()) {
                    // Already saved
                    auto inst = std::make_shared<tigger::StackLoadInst>
                        (reg2stk[target], 20 + param_pos);
                    PUSH_INST(inst);
                } else {
                    // In other registers
                    auto inst = std::make_shared<tigger::AssignRInst>
                        (20 + param_pos, target);
                    PUSH_INST(inst);
                }
            } else {
                // In memory
                replace_flag = false;
                LOAD_SYMBOL(ptr->param_, 20 + param_pos);
                assert(macro_result == 20 + param_pos);
                replace_flag = true;
            }
            ++param_pos;
        } else TEST_TYPE(inst, AssignCallInst) {
            // [SYMBOL =] call FUNCTION
            auto ptr = CAST_P(inst, AssignCallInst);

            // Save Registers
            std::set<int> need_save = func->used_register_;
            std::set<int> temp_set = ptr->func_->used_register_union_;
            assert(param_pos == ptr->func_->paramNum());
            data_flow::mIntersect(need_save, temp_set);
            for (auto x : need_save) {
                if (already_saved.find(x) != already_saved.end()) continue;
                bool flag = false;
                for (auto var : inst->data_in_) {
                    if (!var2reg[var].in_reg) continue;
                    if (var2reg[var].reg_pos != x) continue;
                    flag = true;
                    break;
                }
                if (flag) {
                    auto inst = std::make_shared<tigger::StackStoreInst>
                        (x, reg2stk[x]);
                    PUSH_INST(inst);
                    already_saved.insert(x);
                }
            }

            // Function Call
            auto inst = std::make_shared<tigger::CallInst>
                (ptr->func_->name());
            PUSH_INST(inst);

            if (ptr->store_ != nullptr) {
                auto inst = std::make_shared<tigger::AssignRInst>
                    (14, 20);
                PUSH_INST(inst);
            }

            // Restore Registers
            for (auto x : already_saved) {
                assert(x != 13);
                assert(x != 14);
                // assert(x != 15);
                auto inst = std::make_shared<tigger::StackLoadInst>
                    (reg2stk[x], x);
                PUSH_INST(inst);
            }

            if (ptr->store_ != nullptr) {
                auto id = getID(ptr->store_);
                if (id != -1 && var2reg[id].in_reg) {
                    auto inst = std::make_shared<tigger::AssignRInst>
                        (var2reg[id].reg_pos, 14);
                    PUSH_INST(inst);
                } else {
                    STORE_SYMBOL(ptr->store_, 14, 13);
                }
            }

            param_pos = 0;
            already_saved.clear();
        } else TEST_TYPE(inst, ReturnInst) {
            // return [RightValue]
            auto ptr = CAST_P(inst, ReturnInst);
            if (ptr->ret_ != nullptr) {
                auto id = getID(ptr->ret_);
                if (id != -1 && var2reg[id].in_reg) {
                    if (var2reg[id].reg_pos == 20) {
                        // Do nothing!
                    } else {
                        auto inst = std::make_shared<tigger::AssignRInst>
                            (20, var2reg[id].reg_pos);
                        PUSH_INST(inst);
                    }
                } else {
                    replace_flag = false;
                    LOAD_SYMBOL(ptr->ret_, 20);
                    assert(macro_result == 20);
                    replace_flag = true;
                }
            }
            auto inst2 = std::make_shared<tigger::ReturnInst>();
            PUSH_INST(inst2);
        } else assert(false);

    }

    #undef STORE_SYMBOL
    #undef LOAD_SYMBOL

    #undef PUSH_INST
}

void translate_E2T(eeyore::Program &src, tigger::Program &dst) {
    global_num = param_pos = 0;
    liveness::optimize(src, -1);
    liveness::calculate(src);

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
    numG = src.funcs().back()->nativeNum();
    assert(numG == global_num);

    // Dump Functions
    for (auto &func : src.funcs()) {
        numN = func->nativeNum();
        if (func->name() == "$global$") numN = 0;
        numT = func->tempNum();
        numP = func->paramNum();
        calculateLoop(func);
        calculateWeight(func);
        registerAllocation(func);
        generateTiggerCode(func, dst);
    }
}

#undef TEST_TYPE
#undef CAST_P
    
} // namespace graph_coloring

