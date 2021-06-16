#ifndef __MINIC_EEYORE_HPP__
#define __MINIC_EEYORE_HPP__

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <ostream>
#include <optional>
#include <unordered_map>

#include "operator.hpp"

namespace data_flow {
    class Description;
} // namespace data_flow

namespace eeyore {

class FunctionDef;
using FuncPtr = std::shared_ptr<FunctionDef>;
using FuncList = std::vector<FuncPtr>;

class InstBase {
public:
    virtual ~InstBase() = default;
    virtual void updateGoto(int id) {
        assert(false);
    }
    virtual bool linkEdges(FunctionDef *func, int pos) {
        return true;
    }
    virtual void dumpCode(std::ostream &os, int label_init_id) const = 0;
    
    int def_id_;
    std::set<int> succ_;
    std::set<int> pred_;
    std::set<int> data_in_;
    std::set<int> data_out_;
};

using InstPtr = std::shared_ptr<InstBase>;
using InstPtrList = std::vector<InstPtr>;

class VarBase {
public:
    virtual ~VarBase() = default;
    virtual void dumpCode(std::ostream &os) const = 0;
    virtual void dumpVariableDeclaration(std::ostream &os) const {
        assert(false);
    }
};

using VarPtr = std::shared_ptr<VarBase>;
using VarPtrList = std::vector<VarPtr>;

// InstBase Subclass
class AssignInst : public InstBase {
public:
    AssignInst(VarPtr lhs, VarPtr rhs) : 
        lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    VarPtr lhs_;
    VarPtr rhs_;
};

class ArrayAssignInst : public InstBase {
public:
    ArrayAssignInst(VarPtr lhs_var, VarPtr lhs_index, VarPtr rhs) :
        lhs_var_(std::move(lhs_var)), 
        lhs_index_(std::move(lhs_index)),
        rhs_(std::move(rhs)) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    VarPtr lhs_var_;
    VarPtr lhs_index_;
    VarPtr rhs_;
};

class AssignArrayInst : public InstBase {
public:
    AssignArrayInst(VarPtr lhs, VarPtr rhs_var, VarPtr rhs_index) :
        lhs_(std::move(lhs)),
        rhs_var_(std::move(rhs_var)),
        rhs_index_(std::move(rhs_index)) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    VarPtr lhs_;
    VarPtr rhs_var_;
    VarPtr rhs_index_;
};

class BinaryInst : public InstBase {
public:
    BinaryInst(Operator op, VarPtr lhs, VarPtr rhs_1, VarPtr rhs_2) :
        op_(op),
        lhs_(std::move(lhs)),
        rhs_1_(std::move(rhs_1)),
        rhs_2_(std::move(rhs_2)) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    Operator op_;
    VarPtr lhs_;
    VarPtr rhs_1_;
    VarPtr rhs_2_;
};

class UnaryInst : public InstBase {
public:
    UnaryInst(Operator op, VarPtr lhs, VarPtr rhs) :
        op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    Operator op_;
    VarPtr lhs_;
    VarPtr rhs_;
};

class CondInst : public InstBase {
public:
    CondInst(Operator op, VarPtr cond_1, VarPtr cond_2) :
        op_(op), 
        cond_1_(std::move(cond_1)), 
        cond_2_(std::move(cond_2)), 
        label_id_(-1) {}
    void updateGoto(int id) override {
        label_id_ = id;
    }
    bool linkEdges(FunctionDef *func, int pos) override;
    void dumpCode(std::ostream &os, int label_init_id) const override;

    Operator op_;
    VarPtr cond_1_;
    VarPtr cond_2_;
    int label_id_;
};

class JumpInst : public InstBase {
public:
    JumpInst() : label_id_(-1) {}
    JumpInst(int id) : label_id_(id) {}
    void updateGoto(int id) override {
        label_id_ = id;
    }
    bool linkEdges(FunctionDef *func, int pos) override;
    void dumpCode(std::ostream &os, int label_init_id) const override;

    int label_id_; 
};

class ParamInst : public InstBase {
public:
    ParamInst(VarPtr param) : param_(std::move(param)) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    VarPtr param_;
};

class AssignCallInst : public InstBase {
public:
    AssignCallInst(FuncPtr func, VarPtr store = nullptr) :
        func_(std::move(func)), store_(std::move(store)) {}
    void dumpCode(std::ostream &os, int label_init_id) const override;

    FuncPtr func_;
    VarPtr store_;
};

class ReturnInst : public InstBase {
public:
    ReturnInst(VarPtr ret = nullptr) : ret_(std::move(ret)) {}
    bool linkEdges(FunctionDef *func, int pos) override;
    void dumpCode(std::ostream &os, int label_init_id) const override;

    VarPtr ret_;
};

// VarBase Subclass
class NativeVar : public VarBase {
public:
    NativeVar(int id) : id_(id), is_const_(false) { val_ = std::nullopt; }
    void putVal(int val) { val_ = val; }
    void putVal(int pos, int val) { pos2val_[pos] = val; }
    std::optional<int> getVal() { return val_; }
    std::optional<int> getVal(int pos) {
        if (pos2val_.find(pos) != pos2val_.end()) {
            return pos2val_[pos];
        }
        return std::nullopt;
    }
    void pushWidth(int w) { widths_.push_back(w); }
    const std::vector<int> &widths() const { return widths_; }
    void dumpCode(std::ostream &os) const override;
    void dumpVariableDeclaration(std::ostream &os) const override;

    int id_;
    bool is_const_;
    std::optional<int> val_;
    std::unordered_map<int, int> pos2val_;
    // info from SysY 
    std::vector<int> widths_;
};

using NativePtr = std::shared_ptr<NativeVar>;

class TempVar : public VarBase {
public:
    TempVar(int id) : id_(id) {}
    void dumpCode(std::ostream &os) const override;
    void dumpVariableDeclaration(std::ostream &os) const override;

    int id_;
};

using TempPtr = std::shared_ptr<TempVar>;

class ParamVar : public VarBase {
public:
    ParamVar(int id) : id_(id) {}
    void pushWidth(int w) { widths_.push_back(w); }
    const std::vector<int> &widths() const { return widths_; }
    void dumpCode(std::ostream &os) const override;

    int id_;
    // info from SysY
    std::vector<int> widths_;
};

using ParamPtr = std::shared_ptr<ParamVar>;

class IntValue : public VarBase {
public:
    IntValue(int val) : val_(val) {}
    void dumpCode(std::ostream &os) const override;

    int val_;
};


// TopLayer
class FunctionDef {
public:
    FunctionDef(const std::string &name, int native_init_id, bool has_return) : 
        has_return_(has_return),
        name_(name),
        native_init_id_(native_init_id),
        label_init_id_(0) {}
    void pushInst(InstPtr inst) {
        insts_.push_back(std::move(inst));
    }
    void setLabelInit(int init_id) {
        label_init_id_ = init_id;
    }
    bool need_return() const { return has_return_; }
    int instNum() const { return insts_.size(); }
    int nativeNum() const { return native_.size(); }
    int tempNum() const { return temp_.size(); }
    int paramNum() const { return params_.size(); }
    int labelNum() const { return label_pos_.size(); }
    NativePtr addNative() { 
        auto ptr = std::make_shared<NativeVar>(native_init_id_ + native_.size());
        native_.push_back(ptr);
        return ptr;
    }
    TempPtr addTemp() {
        auto ptr = std::make_shared<TempVar>(temp_.size());
        temp_.push_back(ptr);
        return ptr;
    }
    ParamPtr addParam() {
        auto ptr = std::make_shared<ParamVar>(params_.size());
        params_.push_back(ptr);
        return ptr;
    }
    VarPtr getLastTemp() { return temp_.back(); }
    int lookUpLabel(int inst_pos) {
        for (unsigned i = 0; i < label_pos_.size(); ++i) {
            if (label_pos_[i] == inst_pos)
                return i;
        }
        label_pos_.push_back(inst_pos);
        return label_pos_.size() - 1;
    }
    void backpatch(const std::vector<int> &list, int pos) {
        if (list.empty()) return;
        label_pos_.push_back(pos);
        for (auto id : list) {
            insts_[id]->updateGoto(label_pos_.size() - 1);
        }
    }
    void linkEdges() {
        for (int i = 0; i < instNum(); ++i) {
            insts_[i]->succ_.clear();
            insts_[i]->pred_.clear();
        }
        for (int i = 0; i < instNum(); ++i) {
            bool ret = insts_[i]->linkEdges(this, i);
            if (i + 1 == instNum()) break;
            if (!ret) continue;
            insts_[i]->succ_.insert(i+1);
            insts_[i+1]->pred_.insert(i);
        }

    }
    void eliminateDead();
    void forwardAccess(data_flow::Description &desc);
    void backwardAccess(data_flow::Description &desc);
    void dumpNativeDeclarations(std::ostream &os) const;
    void dumpTempDeclarations(std::ostream &os) const;
    void dumpInstructions(std::ostream &os) const;
    void dumpCode(std::ostream &os) const;
    const std::string &name() const { return name_; }
    const VarPtrList &native() const { return native_; }
    const VarPtrList &temp() const { return temp_; }
    const VarPtrList &params() const { return params_; }
    const InstPtrList &insts() const { return insts_; }
    const std::vector<int> &label_pos() const { return label_pos_; }
    
    bool has_return_;
    std::string name_; 
    int native_init_id_;
    int label_init_id_;
    InstPtrList insts_;
    VarPtrList native_;
    VarPtrList temp_;
    VarPtrList params_;
    std::vector<int> label_pos_;
    std::vector<std::pair<int, int>> loop_pos_;
    std::set<int> used_register_;
    std::set<int> used_register_union_;
};

// Whole Program
class Program {
public:
    void pushFunction(FuncPtr func) { funcs_.push_back(std::move(func)); }
    void dumpCode(std::ostream &os) const;
    const FuncList &funcs() const { return funcs_; }

private:
    FuncList funcs_;
};

} // namespace eeyore

#endif // __MINIC_EEYORE_HPP__

