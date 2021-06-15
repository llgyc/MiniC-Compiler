#include "ast.hpp"
#include "utils.hpp"
#include "context.hpp"

void CompUnitASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void ConstDefListASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void ConstDefASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void ConstIndexListASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void ConstInitValListASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void VarDefListASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void VarDefASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void InitValListASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void FuncDefASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void FuncFParamsASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void FuncFParamASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void BlockItemsASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void AssignASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void IfASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void WhileASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void BreakASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void ContinueASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void ReturnASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void LValASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void IndexListASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void BinaryExpASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void FunCallASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void FuncRParamsASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void IntASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

void UnaryOpASTNode::generateEeyoreCode(Context &ctx, eeyore::Program &prog) {
    return ctx.generateEeyoreOn(this, prog);
}

std::optional<int> LValASTNode::eval(Context &ctx) const {
    auto base_ptr = ctx.lookUpVariable(ident_).get();
    auto derived_ptr = dynamic_cast<eeyore::NativeVar *>(base_ptr);
    if (derived_ptr == nullptr)
        return std::nullopt;
    if (!derived_ptr->is_const_) return std::nullopt;
    auto &widths = derived_ptr->widths();
    // Variable Case
    if (widths.size() == 0)
        return derived_ptr->getVal();
    // Array Case
    std::vector<int> index_list;
    auto index_ptr = dynamic_cast<IndexListASTNode *>(index_list_.get());
    for (auto &index : (*index_ptr->index_list())) {
        auto val = index->eval(ctx);
        if (!val) return std::nullopt;
        index_list.push_back(val.value());
    }
    auto id = getIdFromIndex(widths, index_list); 
    if (id) return derived_ptr->getVal(id.value());
    return std::nullopt;
}

std::optional<int> BinaryExpASTNode::eval(Context &ctx) const {
    auto lhs = lhs_->eval(ctx);
    auto rhs = rhs_->eval(ctx);
    if (lhs && rhs) {
        switch (op_) {
            case Operator::kAdd:
                return lhs.value() + rhs.value();
            case Operator::kSub:
                return lhs.value() - rhs.value();
            case Operator::kMul:
                return lhs.value() * rhs.value();
            case Operator::kDiv:
                return lhs.value() / rhs.value();
            case Operator::kMod:
                return lhs.value() % rhs.value();
            default:
                return std::nullopt;
        }
    } 
    return std::nullopt;
}

std::optional<int> IntASTNode::eval(Context &ctx) const {
    return val_;
}

std::optional<int> UnaryOpASTNode::eval(Context &ctx) const {
    auto operand = operand_->eval(ctx);
    if (operand) {
        switch (op_) {
            case Operator::kAdd:
                return operand;
            case Operator::kSub:
                return -operand.value();
            default:
                return std::nullopt;
        }
    }
    return std::nullopt;
}
