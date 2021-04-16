#ifndef __MINIC_AST_HPP__
#define __MINIC_AST_HPP__

#include <string>
#include <vector>
#include <memory>
#include <cassert>

#include "operator.hpp"

class BaseASTNode;

using ASTNodePtr = std::unique_ptr<BaseASTNode>;
using ASTPtrList = std::vector<ASTNodePtr>;
using ASTPtrListPtr = std::unique_ptr<ASTPtrList>;

class BaseASTNode {
public:
    virtual ~BaseASTNode() = default;
};

class CompUnitASTNode : public BaseASTNode {
public:
    CompUnitASTNode(ASTNodePtr comp_unit) {
        comp_units_ = std::make_unique<ASTPtrList>();
        comp_units_ -> push_back(std::move(comp_unit));
    }
    CompUnitASTNode(ASTNodePtr comp_units, ASTNodePtr comp_unit) {
        auto base_ptr = comp_units.get();
        auto derived_ptr = static_cast<CompUnitASTNode *>(base_ptr);
        comp_units_ = std::move(derived_ptr -> comp_units_);
        comp_units_ -> push_back(std::move(comp_unit));
    }

private:
    ASTPtrListPtr comp_units_;
};

class ConstDefListASTNode : public BaseASTNode {
public:
    ConstDefListASTNode(ASTNodePtr const_def) {
        const_defs_ = std::make_unique<ASTPtrList>();
        const_defs_ -> push_back(std::move(const_def));
    }
    ConstDefListASTNode(ASTNodePtr const_defs, ASTNodePtr const_def) {
        auto base_ptr = const_defs.get();
        auto derived_ptr = static_cast<ConstDefListASTNode *>(base_ptr);
        const_defs_ = std::move(derived_ptr -> const_defs_);
        const_defs_ -> push_back(std::move(const_def));
    }

private:
    ASTPtrListPtr const_defs_;
};

class ConstDefASTNode : public BaseASTNode {
public:
    ConstDefASTNode(const std::string &ident, ASTNodePtr const_index_list, 
        ASTNodePtr const_init_val) :
        ident_(ident),
        const_index_list_(std::move(const_index_list)), 
        const_init_val_(std::move(const_init_val)) {}
private:
    std::string ident_;
    ASTNodePtr const_index_list_;
    ASTNodePtr const_init_val_;
};

class ConstIndexListASTNode : public BaseASTNode {
public:
    ConstIndexListASTNode() {
        const_exps_ = std::make_unique<ASTPtrList>();
    }
    ConstIndexListASTNode(ASTNodePtr const_exps, ASTNodePtr const_exp) {
        auto base_ptr = const_exps.get();
        auto derived_ptr = static_cast<ConstIndexListASTNode *>(base_ptr); 
        const_exps_ = std::move(derived_ptr -> const_exps_);
        const_exps_ -> push_back(std::move(const_exp));
    }

private:
    ASTPtrListPtr const_exps_;
};

class ConstInitValListASTNode : public BaseASTNode {
public:
    ConstInitValListASTNode() {
        const_init_val_list_ = std::make_unique<ASTPtrList>();
    }
    ConstInitValListASTNode(ASTNodePtr const_init_val) {
        const_init_val_list_ = std::make_unique<ASTPtrList>();
        const_init_val_list_ -> push_back(std::move(const_init_val));
    }
    ConstInitValListASTNode(ASTNodePtr const_init_val_list, 
        ASTNodePtr const_init_val) {
        auto base_ptr = const_init_val_list.get();
        auto derived_ptr = static_cast<ConstInitValListASTNode *>(base_ptr);
        const_init_val_list_ = std::move(derived_ptr -> const_init_val_list_);
        const_init_val_list_ -> push_back(std::move(const_init_val));
    }

private:
    ASTPtrListPtr const_init_val_list_;
};

class VarDefListASTNode : public BaseASTNode {
public:
    VarDefListASTNode(ASTNodePtr var_def) {
        var_def_list_ = std::make_unique<ASTPtrList>();
        var_def_list_ -> push_back(std::move(var_def));
    }
    VarDefListASTNode(ASTNodePtr var_def_list, ASTNodePtr var_def) {
        auto base_ptr = var_def_list.get();
        auto derived_ptr = static_cast<VarDefListASTNode *>(base_ptr);
        var_def_list_ = std::move(derived_ptr -> var_def_list_);
        var_def_list_ -> push_back(std::move(var_def));
    }

private:
    ASTPtrListPtr var_def_list_;
};

class VarDefASTNode : public BaseASTNode {
public:
    VarDefASTNode(const std::string &ident, ASTNodePtr const_index_list) :
        ident_(ident),
        const_index_list_(std::move(const_index_list)),
        init_val_(nullptr) {}
    VarDefASTNode(const std::string &ident, ASTNodePtr const_index_list,
        ASTNodePtr init_val) :
        ident_(ident),
        const_index_list_(std::move(const_index_list)),
        init_val_(std::move(init_val)) {}

private:
    std::string ident_;
    ASTNodePtr const_index_list_;
    ASTNodePtr init_val_;
};

class InitValListASTNode : public BaseASTNode {
public:
    InitValListASTNode() {
        init_val_list_ = std::make_unique<ASTPtrList>();
    }
    InitValListASTNode(ASTNodePtr init_val) {
        init_val_list_ = std::make_unique<ASTPtrList>();
        init_val_list_ -> push_back(std::move(init_val));
    }
    InitValListASTNode(ASTNodePtr init_val_list, ASTNodePtr init_val) {
        auto base_ptr = init_val_list.get();
        auto derived_ptr = static_cast<InitValListASTNode *>(base_ptr);
        init_val_list_ = std::move(derived_ptr -> init_val_list_);
        init_val_list_ -> push_back(std::move(init_val));
    }

private:
    ASTPtrListPtr init_val_list_;
};

enum class ReturnType {
    kVoidType,
    kIntType
};

class FuncDefASTNode : public BaseASTNode {
public:
    FuncDefASTNode(ReturnType return_type, const std::string &ident, 
        ASTNodePtr block) :
        return_type_(return_type), 
        ident_(ident), 
        func_f_params_(nullptr),                                
        block_(std::move(block)) {}
    FuncDefASTNode(ReturnType return_type, const std::string &ident, 
        ASTNodePtr func_f_params, ASTNodePtr block) :
        return_type_(return_type),
        ident_(ident), 
        func_f_params_(std::move(func_f_params)),
        block_(std::move(block)) {}
    ReturnType return_type() const { return return_type_; }

private:
    ReturnType return_type_;
    std::string ident_;
    ASTNodePtr func_f_params_;
    ASTNodePtr block_;
};

class FuncFParamsASTNode : public BaseASTNode {
public:
    FuncFParamsASTNode(ASTNodePtr func_f_param) {
        func_f_params_  = std::make_unique<ASTPtrList>();
        func_f_params_ -> push_back(std::move(func_f_param));
    }
    FuncFParamsASTNode(ASTNodePtr func_f_params, ASTNodePtr func_f_param) {
        auto base_ptr = func_f_params.get();
        auto derived_ptr = static_cast<FuncFParamsASTNode *>(base_ptr);
        func_f_params_ = std::move(derived_ptr->func_f_params_);
        func_f_params_ -> push_back(std::move(func_f_param));
    }

private:
    ASTPtrListPtr func_f_params_;
};

class FuncFParamASTNode : public BaseASTNode {
public:
    FuncFParamASTNode(const std::string &ident) :
        ident_(ident), const_index_list_(nullptr) {}
    FuncFParamASTNode(const std::string &ident, ASTNodePtr const_index_list) :
        ident_(ident), const_index_list_(std::move(const_index_list)) {}
        
private:
    std::string ident_;
    ASTNodePtr const_index_list_;
};

class BlockItemsASTNode : public BaseASTNode {
public:
    BlockItemsASTNode() {
        block_items_ = std::make_unique<ASTPtrList>();
    }
    BlockItemsASTNode(ASTNodePtr block_items, ASTNodePtr block_item) {
        auto base_ptr = block_items.get();
        auto derived_ptr = static_cast<BlockItemsASTNode *>(base_ptr);
        block_items_ = std::move(derived_ptr -> block_items_);
        if (block_items != nullptr)
            block_items_ -> push_back(std::move(block_item));
    }

private:
    ASTPtrListPtr block_items_;
};

class AssignASTNode : public BaseASTNode {
public:
    AssignASTNode(ASTNodePtr lval, ASTNodePtr exp) : 
        lval_(std::move(lval)), exp_(std::move(exp)) {}

private:
    ASTNodePtr lval_;
    ASTNodePtr exp_;
};

class IfASTNode : public BaseASTNode {
public:
    IfASTNode(ASTNodePtr cond, ASTNodePtr then):
        cond_(std::move(cond)),
        then_(std::move(then)),
        else_then_(nullptr) {}
    IfASTNode(ASTNodePtr cond, ASTNodePtr then, ASTNodePtr else_then) :
        cond_(std::move(cond)),
        then_(std::move(then)),
        else_then_(std::move(else_then)) {}

private:
    ASTNodePtr cond_;
    ASTNodePtr then_;
    ASTNodePtr else_then_;
};

class WhileASTNode : public BaseASTNode {
public:
    WhileASTNode(ASTNodePtr cond, ASTNodePtr stmt) :
        cond_(std::move(cond)),
        stmt_(std::move(stmt)) {}

private:
    ASTNodePtr cond_;
    ASTNodePtr stmt_;
};

class BreakASTNode : public BaseASTNode {
public:
    BreakASTNode() = default;
};

class ContinueASTNode : public BaseASTNode {
public:
    ContinueASTNode() = default;
};

class ReturnASTNode : public BaseASTNode {
public:
    ReturnASTNode() : exp_(nullptr) {}
    ReturnASTNode(ASTNodePtr exp) :
        exp_(std::move(exp)) {}

private:
    ASTNodePtr exp_;
};

class LValASTNode : public BaseASTNode {
public:
    LValASTNode(const std::string &ident, ASTNodePtr index_list) :
        ident_(ident), index_list_(std::move(index_list)) {}

private:
    std::string ident_;
    ASTNodePtr index_list_;
};

class IndexListASTNode : public BaseASTNode {
public:
    IndexListASTNode() {
        index_list_ = std::make_unique<ASTPtrList>();
    }
    IndexListASTNode(ASTNodePtr index_list, ASTNodePtr exp) {
        auto base_ptr = index_list.get();
        auto derived_ptr = static_cast<IndexListASTNode *>(base_ptr);
        index_list_ = std::move(derived_ptr -> index_list_);
        index_list_ -> push_back(std::move(exp));
    }

private:
    ASTPtrListPtr index_list_;
};
    
class BinaryExpASTNode : public BaseASTNode {
public:
    BinaryExpASTNode(Operator op, ASTNodePtr lhs, ASTNodePtr rhs) :
        op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    Operator op() const { return op_; }

private:
    Operator op_;
    ASTNodePtr lhs_;
    ASTNodePtr rhs_;
};

class FunCallASTNode : public BaseASTNode {
public:
    FunCallASTNode(const std::string &ident) :
        ident_(ident), func_r_params_(nullptr) {}
    FunCallASTNode(const std::string &ident, ASTNodePtr func_r_params) :
        ident_(ident), func_r_params_(std::move(func_r_params)) {}

private:
    std::string ident_;
    ASTNodePtr func_r_params_;
};

class FuncRParamsASTNode : public BaseASTNode {
public:
    FuncRParamsASTNode(ASTNodePtr exp) {
        func_r_params_ = std::make_unique<ASTPtrList>();
        func_r_params_ -> push_back(std::move(exp));
    }
    FuncRParamsASTNode(ASTNodePtr func_r_params, ASTNodePtr exp) {
        auto base_ptr = func_r_params.get();
        auto derived_ptr = static_cast<FuncRParamsASTNode *>(base_ptr);
        func_r_params_ = std::move(derived_ptr -> func_r_params_);
        func_r_params_ -> push_back(std::move(exp));
    }

private:
    ASTPtrListPtr func_r_params_;
};
    
class IntASTNode : public BaseASTNode {
public:
    IntASTNode(int val) : val_(val) {}
    int val() const { return val_; }

private:
    int val_;
};

class UnaryOpASTNode : public BaseASTNode {
public:
    UnaryOpASTNode(Operator op, ASTNodePtr operand) :
        op_(op), operand_(std::move(operand)) {}
    Operator op() const { return op_; }

private:
    Operator op_;
    ASTNodePtr operand_;
};

#endif // __MINIC_AST_HPP__
