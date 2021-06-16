#include <set>
#include <queue>

#include "eeyore.hpp"
#include "data_flow.hpp"

namespace eeyore {

void AssignInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    lhs_->dumpCode(os);
    os << " = ";
    rhs_->dumpCode(os);
    os << std::endl;
}

void ArrayAssignInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    lhs_var_->dumpCode(os);
    os << "[";
    lhs_index_->dumpCode(os);
    os << "] = ";
    rhs_->dumpCode(os);
    os << std::endl;
}

void AssignArrayInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    lhs_->dumpCode(os);
    os << " = ";
    rhs_var_->dumpCode(os);
    os << "[";
    rhs_index_->dumpCode(os);
    os << "]" << std::endl;
}

void BinaryInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    lhs_->dumpCode(os);
    os << " = ";
    rhs_1_->dumpCode(os);
    os << " " << operators[static_cast<int>(op_)] << " ";
    rhs_2_->dumpCode(os);
    os << std::endl;
}

void UnaryInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    lhs_->dumpCode(os);
    os << " = " << operators[static_cast<int>(op_)] << " ";
    rhs_->dumpCode(os);
    os << std::endl;
}

bool CondInst::linkEdges(FunctionDef *func, int pos) {
    succ_.insert(func->label_pos()[label_id_]);
    func->insts()[func->label_pos()[label_id_]]->pred_.insert(pos);
    return true;
}

void CondInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    os << "if ";
    cond_1_->dumpCode(os);
    os << " " << operators[static_cast<int>(op_)] << " ";
    cond_2_->dumpCode(os);
    os << " goto l" << label_init_id + label_id_;
    os << std::endl;
}

bool JumpInst::linkEdges(FunctionDef *func, int pos) {
    succ_.insert(func->label_pos()[label_id_]);
    func->insts()[func->label_pos()[label_id_]]->pred_.insert(pos);
    return false;
}

void JumpInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    os << "goto l" << label_init_id + label_id_;
    os << std::endl;
}

void ParamInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    os << "param ";
    param_->dumpCode(os);
    os << std::endl;
}

void AssignCallInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    if (store_ == nullptr) {
        os << "call f_";
        if (func_->name() == "main")
            os << "__main__";
        else 
            os << func_->name();
    } else {
        store_->dumpCode(os);
        os << " = call f_";
        if (func_->name() == "main")
            os << "__main__";
        else
            os << func_->name();
    }
    os << std::endl;
}

bool ReturnInst::linkEdges(FunctionDef *func, int pos) {
    succ_.insert(func->instNum());
    return false;
}

void ReturnInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  return";
    if (ret_ != nullptr) {
        os << " "; 
        ret_->dumpCode(os);
    }
    os << std::endl;
}

void NativeVar::dumpCode(std::ostream &os) const {
    os << "T" << id_;
}

void TempVar::dumpCode(std::ostream &os) const {
    os << "t" << id_;
}

void ParamVar::dumpCode(std::ostream &os) const {
    os << "p" << id_;
    return;
}

void IntValue::dumpCode(std::ostream &os) const {
    os << val_;
}

void NativeVar::dumpVariableDeclaration(std::ostream &os) const {
    if (widths_.empty()) {
        os << "  var T" << id_ << std::endl;
    } else {
        os << "  var " << widths_[0] * 4 << " T" << id_ << std::endl;
    }
}

void TempVar::dumpVariableDeclaration(std::ostream &os) const {
    os << "  var t" << id_ << std::endl;
}

void FunctionDef::dumpNativeDeclarations(std::ostream &os) const {
    for (auto &nvar : native_) {
        nvar->dumpVariableDeclaration(os);
    }
}

void FunctionDef::dumpTempDeclarations(std::ostream &os) const {
    for (auto &tvar : temp_) {
        tvar->dumpVariableDeclaration(os);
    }
}

void FunctionDef::dumpInstructions(std::ostream &os) const {
    for (int i = 0; i < instNum(); ++i) {
        for (unsigned j = 0; j < label_pos_.size(); ++j) {
            if (i == label_pos_[j]) {
                os << "l" << label_init_id_ + j << ":" << std::endl;
            }
        }
        insts_[i]->dumpCode(os, label_init_id_);
    }
}

void FunctionDef::dumpCode(std::ostream &os) const {
    // Real Functions
    std::string name = name_;
    if (name_ == "$global$") {
        name = "main";
    } 
    if (name_ == "main") {
        name = "__main__";
    }
    os << "f_" << name << " [" << paramNum() << "]" << std::endl;
    if (name_ != "$global$") {
        dumpNativeDeclarations(os);
    }
    dumpTempDeclarations(os);
    dumpInstructions(os);
    os << "end f_" << name << std::endl;
}

void Program::dumpCode(std::ostream &os) const {
    int label_num = 0;
    assert(funcs_.back()->name() == "$global$");
    // Global Variable
    funcs_.back()->dumpNativeDeclarations(os);
    for (auto &func : funcs_) {
        func->setLabelInit(label_num);
        func->dumpCode(os);
        label_num += func->labelNum();
    }
}

void FunctionDef::forwardAccess(data_flow::Description &desc) {
    linkEdges();
    for (auto &inst : insts_) {
        desc.block_init_(this, inst->data_out_);
    }
    std::set<int> entry_out;
    desc.global_init_(this, entry_out);
    std::set<int> used;
    std::queue<int> q;
    for (int i = 0; i < instNum(); ++i) {
        used.insert(i);
        q.push(i);
    }
    while (!q.empty()) {
        int now = q.front(); q.pop();
        used.erase(now);

        desc.local_ident_(this, insts_[now]->data_in_);
        for (auto nxt : insts_[now]->pred_) {
            desc.meet_(insts_[now]->data_in_, insts_[nxt]->data_out_);
        }
        if (now == 1)
            desc.meet_(insts_[now]->data_in_, entry_out);

        unsigned last_size = insts_[now]->data_out_.size();
        insts_[now]->data_out_ = insts_[now]->data_in_;
        data_flow::mDifference(insts_[now]->data_out_,
            desc.kill_(this, insts_[now]));
        data_flow::mUnion(insts_[now]->data_out_,
            desc.gen_(this, insts_[now]));

        if (insts_[now]->data_out_.size() != last_size) {
            for (auto nxt : insts_[now]->succ_) {
                if (nxt == instNum()) continue;
                if (used.find(nxt) == used.end()) {
                    used.insert(nxt);
                    q.push(nxt);
                }
            }
        }

    }
}

void FunctionDef::backwardAccess(data_flow::Description &desc) {
    linkEdges();
    for (auto &inst : insts_) {
        desc.block_init_(this, inst->data_in_);
    }
    std::set<int> exit_in;
    desc.global_init_(this, exit_in);
    std::set<int> used;
    std::queue<int> q;
    for (int i = 0; i < instNum(); ++i) {
       used.insert(i);
       q.push(i);
    }
    while (!q.empty()) {
        int now = q.front(); q.pop();
        used.erase(now);

        desc.local_ident_(this, insts_[now]->data_out_);
        for (auto nxt : insts_[now]->succ_) {
            if (nxt == instNum())
                desc.meet_(insts_[now]->data_out_, exit_in);
            else
                desc.meet_(insts_[now]->data_out_, insts_[nxt]->data_in_);
        }

        unsigned last_size = insts_[now]->data_in_.size();
        insts_[now]->data_in_ = insts_[now]->data_out_;
        data_flow::mDifference(insts_[now]->data_in_,
            desc.kill_(this, insts_[now]));
        data_flow::mUnion(insts_[now]->data_in_,
            desc.gen_(this, insts_[now]));

        if (insts_[now]->data_in_.size() != last_size) {
            for (auto nxt : insts_[now]->pred_) {
                if (used.find(nxt) == used.end()) {
                    used.insert(nxt);
                    q.push(nxt);
                }
            }
        }
    }
}

} // namespace eeyore
