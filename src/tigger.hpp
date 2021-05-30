#ifndef __MINIC_TIGGER_HPP__
#define __MINIC_TIGGER_HPP__

#include <map>
#include <memory>
#include <vector>
#include <cassert>
#include <ostream>

#include "operator.hpp"

namespace tigger {

class InstBase {
public:
    virtual ~InstBase() = default;
    virtual void dumpCode(std::ostream &os, int label_init_id) const = 0;
};

using InstPtr = std::shared_ptr<InstBase>;
using InstList = std::vector<InstPtr>;

class FunctionDef;
using FuncPtr = std::shared_ptr<FunctionDef>;
using FuncList = std::vector<FuncPtr>;

// Reg 0-27
// 0     x0
// 1-12  s0-s11
// 13-19 t0-t6
// 20-27 a0-a7

// InstBase Subclass
class BinaryRInst : public InstBase {
public:
    BinaryRInst(Operator op, int rd, int rs1, int rs2) :
        op_(op), rd_(rd), rs1_(rs1), rs2_(rs2) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    Operator op_;
    int rd_;
    int rs1_;
    int rs2_;
};

class BinaryIInst : public InstBase {
public:
    BinaryIInst(Operator op, int rd, int rs, int imm) :
        op_(op), rd_(rd), rs_(rs), imm_(imm) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    Operator op_;
    int rd_;
    int rs_;
    int imm_;
};

class UnaryInst : public InstBase {
public:
    UnaryInst(Operator op, int rd, int rs) :
        op_(op), rd_(rd), rs_(rs) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;
        
    Operator op_;
    int rd_;
    int rs_;
};

class AssignRInst : public InstBase {
public:
    AssignRInst(int rd, int rs) : 
        rd_(rd), rs_(rs) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int rd_;
    int rs_;
};

class AssignIInst : public InstBase {
public:
    AssignIInst(int rd, int imm) :
        rd_(rd), imm_(imm) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int rd_;
    int imm_;
};

class MemStoreInst : public InstBase {
public:
    MemStoreInst(int rd, int imm, int rs) :
        rd_(rd), imm_(imm), rs_(rs) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int rd_;
    int imm_;
    int rs_;
};

class MemLoadInst : public InstBase {
public:
    MemLoadInst(int rd, int rs, int imm) :
        rd_(rd), rs_(rs), imm_(imm) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int rd_;
    int rs_;
    int imm_;
};
    
class CondInst : public InstBase {
public:
    CondInst(Operator op, int rs1, int rs2, int label) :
        op_(op), rs1_(rs1), rs2_(rs2), label_(label) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    Operator op_;
    int rs1_;
    int rs2_;
    int label_;
};

class JumpInst : public InstBase {
public:
    JumpInst(int label) : label_(label) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int label_;
};

class CallInst : public InstBase {
public:
    CallInst(const std::string &func) : func_(func) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    std::string func_;
};

class ReturnInst : public InstBase {
public:
    ReturnInst() = default;
    void dumpCode(std::ostream &os, int label_init_id) const override;
};

class StackStoreInst : public InstBase {
public:
    StackStoreInst(int rs, int pos) : rs_(rs), pos_(pos) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int rs_;
    int pos_;
};

class StackLoadInst : public InstBase {
public:
    StackLoadInst(int pos, int rd) : pos_(pos), rd_(rd) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int pos_;
    int rd_;
};

class VarLoadInst : public InstBase {
public:
    VarLoadInst(int var, int rd) : 
        var_(var), rd_(rd) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int var_;
    int rd_;
};

class StackAddrLoadInst : public InstBase {
public:
    StackAddrLoadInst(int pos, int rd) : pos_(pos), rd_(rd) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int pos_;
    int rd_;
};

class VarAddrLoadInst : public InstBase {
public:
    VarAddrLoadInst(int var, int rd) : 
        var_(var), rd_(rd) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int var_;
    int rd_;
};

class FunctionDef {
public:
    FunctionDef(const std::string &name, int param_num, int stk_size) :
        name_(name), param_num_(param_num), stk_size_(stk_size) {}
    const std::string &name() const { return name_; }
    int param_num() const { return param_num_; }
    int stk_size() const { return stk_size_; }
    int instNum() const { return insts_.size(); }
    int labelNum() const { return label_pos_.size(); }
    void setLabel(int pos, int val) { label_pos_[pos] = val; }
    void setLabelNum(int size) { label_pos_.resize(size); }
    void pushInst(InstPtr inst) {
        insts_.push_back(std::move(inst));
    }
    void dumpCode(std::ostream &os, int label_init_id) const;
    const std::vector<int> &label_pos() const { return label_pos_; }
    const InstList &insts() const { return insts_; }

private:
    std::string name_;
    int param_num_;
    int stk_size_;
    std::vector<int> label_pos_;
    InstList insts_;
};

class Program {
public:
    void pushFunction(FuncPtr func) { funcs_.push_back(std::move(func)); }
    void pushGlobal(int id, bool is_array, int val) { 
        global_.insert(std::make_pair(id, std::make_pair(is_array, val)));
    }
    void dumpCode(std::ostream &os) const;
    const FuncList &funcs() const { return funcs_; }
    const std::map<int, std::pair<bool, int> > global() const { 
        return global_; 
    }

private:
    FuncList funcs_;
    std::map<int, std::pair<bool, int> > global_;

};

} // namespace tigger

#endif // __MINIC_TIGGER_HPP__

