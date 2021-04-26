#include "eeyore.hpp"

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

void CondInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    os << "if ";
    cond_1_->dumpCode(os);
    os << " " << operators[static_cast<int>(op_)] << " ";
    cond_2_->dumpCode(os);
    os << " goto l" << label_init_id + label_id_;
    os << std::endl;
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
        os << func_->name();
    } else {
        store_->dumpCode(os);
        os << " = call f_";
        os << func_->name();
    }
    os << std::endl;
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

void FunctionDef::dumpVariableDeclarations(std::ostream &os) const {
    for (auto &nvar : native_) {
        nvar->dumpVariableDeclaration(os);
    }
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
    if (name_ == "$global$") {
        // Global Definition
        dumpVariableDeclarations(os);
        dumpInstructions(os);
    } else {
        // Real Functions
        os << "f_" << name_ << " [" << paramNum() << "]" << std::endl;
        dumpVariableDeclarations(os);
        dumpInstructions(os);
        os << "end f_" << name_ << std::endl;
    }
}

void Program::dumpCode(std::ostream &os) const {
    int label_num = 0;
    for (auto &func : funcs_) {
        func->setLabelInit(label_num);
        func->dumpCode(os);
        label_num += func->labelNum();
    }
}

} // namespace eeyore
