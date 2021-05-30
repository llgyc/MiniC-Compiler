#include "tigger.hpp"

namespace tigger {
    
#define PRINT_REG(REG) \
if (REG == 0) { \
    os << "x0"; \
} else if (REG < 13) { \
    os << "s" << (REG - 1); \
} else if (REG < 20) { \
    os << "t" << (REG - 13); \
} else if (REG < 28) { \
    os << "a" << (REG - 20); \
} else assert(false);

void BinaryRInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    PRINT_REG(rd_);
    os << " = ";
    PRINT_REG(rs1_);
    os << " " << operators[static_cast<int>(op_)] << " ";
    PRINT_REG(rs2_);
    os << std::endl;
}

void BinaryIInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    PRINT_REG(rd_);
    os << " = ";
    PRINT_REG(rs_);
    os << " " << operators[static_cast<int>(op_)] << " ";
    os << imm_ << std::endl;
}

void UnaryInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    PRINT_REG(rd_);
    os << " = " << operators[static_cast<int>(op_)] << " ";
    PRINT_REG(rs_);
    os << std::endl;
}

void AssignRInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    PRINT_REG(rd_);
    os << " = ";
    PRINT_REG(rs_);
    os << std::endl;
}

void AssignIInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    PRINT_REG(rd_);
    os << " = " << imm_ << std::endl;
}

void MemStoreInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    PRINT_REG(rd_);
    os << "[" << imm_ << "] = ";
    PRINT_REG(rs_);
    os << std::endl;
}

void MemLoadInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  ";
    PRINT_REG(rd_);
    os << " = ";
    PRINT_REG(rs_);
    os << "[" << imm_ << "]" << std::endl;
}

void CondInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  if ";
    PRINT_REG(rs1_);
    os << " " << operators[static_cast<int>(op_)] << " ";
    PRINT_REG(rs2_);
    os << " goto l" << label_ + label_init_id << std::endl;
}

void JumpInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  goto l" << label_ + label_init_id << std::endl;
}

void CallInst::dumpCode(std::ostream &os, int label_init_id) const {
    if (func_ == "main")
        os << "  call f___main__" << std::endl;
    else 
        os << "  call f_" << func_ << std::endl;
}

void ReturnInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  return" << std::endl;
};

void StackStoreInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  store ";
    PRINT_REG(rs_);
    os << " " << pos_ << std::endl;
}

void StackLoadInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  load " << pos_ << " ";
    PRINT_REG(rd_);
    os << std::endl;
}

void VarLoadInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  load v" << var_ << " ";
    PRINT_REG(rd_);
    os << std::endl;
}

void StackAddrLoadInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  loadaddr " << pos_ << " ";
    PRINT_REG(rd_);
    os << std::endl;
}

void VarAddrLoadInst::dumpCode(std::ostream &os, int label_init_id) const {
    os << "  loadaddr v" << var_ << " ";
    PRINT_REG(rd_);
    os << std::endl;
}

void FunctionDef::dumpCode(std::ostream &os, int label_init_id) const {
    std::string name = name_;
    if (name_ == "$global$") {
        name = "main";
    }
    if (name_ == "main") {
        name = "__main__";
    }
    os << "f_" << name << " [" << param_num_ << "] [" << stk_size_ << "]";
    os << std::endl;
    for (int i = 0; i < instNum(); ++i) {
        for (int j = 0; j < labelNum(); ++j) {
            if (i == label_pos_[j]) {
                os << "l" << j + label_init_id << ":" << std::endl;
            }
        }
        insts_[i]->dumpCode(os, label_init_id);
    }
    os << "end f_" << name << std::endl;
}

void Program::dumpCode(std::ostream &os) const {
    // GlobalVarDecl
    for (auto x : global_) {
        os << "v" << x.first << " = ";
        if (x.second.first) {
            os << "malloc " << x.second.second << std::endl;
        } else {
            os << x.second.second << std::endl;
        }
    }
    // FunctionDef
    int total_label = 0;
    for (auto &func : funcs_) {
        func->dumpCode(os, total_label);
        total_label += func->labelNum();
    }
}

#undef PRINT_REG

} // namespace tigger
