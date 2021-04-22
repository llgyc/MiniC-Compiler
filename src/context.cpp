#include "context.hpp"

void Context::walkAndGetConst(const std::vector<int> &widths, int now_pos,
    const ASTPtrListPtr &ast, ArrayConstList &result, int level) {
    for (auto &elem : (*ast)) {
        auto base_ptr = elem.get();
        auto derived_ptr = dynamic_cast<ConstInitValListASTNode *>(base_ptr);
        if (derived_ptr == nullptr) {
            // Value
            result.push_back(
                std::make_pair(now_pos++, derived_ptr->eval(*this))
            );
        } else {
            // List
            int next_pos = now_pos;
            int next_level = level + 1;
            for (unsigned i = level; i < widths.size(); ++i) {
                if (now_pos % widths[i] == 0) {
                    next_pos = now_pos + widths[i];
                    next_level = i + 1;
                    break;
                }
            } 
            walkAndGetConst(widths, now_pos, derived_ptr->const_init_val_list(),
                            result, next_level);
            now_pos = next_pos;
        }
    }
}

void Context::walkAndGetVar(const std::vector<int> &widths, int now_pos,
    const ASTPtrListPtr &ast, ArrayVarList &result, int level) {
    for (auto &elem : (*ast)) {
        auto base_ptr = elem.get();
        auto derived_ptr = dynamic_cast<InitValListASTNode *>(base_ptr);
        if (derived_ptr == nullptr) {
            // Value
            
            result.push_back(
        } else {
            // List
        }
    }
}

void Context::generateEeyoreOn(CompUnitASTNode *ast, eeyore::Program &prog) {
    // Declaration first
    for (auto &comp_unit : (*ast->comp_units())) {
        auto ptr = comp_unit.get();
        if (dynamic_cast<FuncDefASTNode *>(ptr) != nullptr) continue;
        comp_unit->generateEeyoreCode(*this, prog);
    }
    // Function second
    for (auto &comp_unit : (*ast->comp_units())) {
        auto ptr = comp_unit.get();
        if (dynamic_cast<FuncDefASTNode *>(ptr) == nullptr) continue;
        comp_unit->generateEeyoreCode(*this, prog);
    }
}

void Context::generateEeyoreOn(ConstDefListASTNode *ast, eeyore::Program &prog) {
    for (auto &const_def : (*ast->const_defs())) {
        const_def->generateEeyoreCode(*this, prog);
    }
}

void Context::generateEeyoreOn(ConstDefASTNode *ast, eeyore::Program &prog) {
    auto name = ast->ident();
    auto index_ptr = ast->const_index_list().get();
    auto &index_list_ptr = 
        dynamic_cast<ConstIndexListASTNode *>(index_ptr)->const_exps();
    auto nvar = cur_func_->addNative(); 

    if (index_list_ptr->empty()) {
        // Variable Case
        std::optional<int> val = ast->const_init_val()->eval(*this);
        if (!val) LogError("[ERROR] Initial value of \"" 
                            + name + "\" is not a constant"); 
        dynamic_cast<eeyore::NativeVar *>(nvar.get())->putVal(val.value());

        // Add to symtab
        addVariable(name, nvar);

        // Generate toplayer related
        auto val_const = std::make_shared<eeyore::IntValue>(val.value());
        auto inst = std::make_shared<eeyore::AssignInst>
                        (nvar, std::move(val_const));
        cur_func_->pushInst(std::move(inst));
    } else {
        // Array Case
        auto val_ptr = ast->const_init_val().get();
        auto &val_list_ptr =
            dynamic_cast<ConstInitValListASTNode *>(val_ptr)
            ->const_init_val_list();
        std::vector<int> widths;
        for (auto &const_exp : (*index_list_ptr)) {
            auto ret = const_exp->eval(*this);
            if (ret)
                widths.push_back(ret.value());
            else
                LogError("[ERROR] Initial value of \"" 
                          + name + "\" is not a constant");
        }
        for (auto it = widths.rbegin() + 1; it != widths.rend(); ++it) {
            *it = (*it) * (*(it-1));
        }
        for (auto width : widths) {
            nvar->pushWidth(width);
        }
        ArrayConstList init_vals;
        walkAndGetConst(widths, 0, val_list_ptr, init_vals, 1);
         
        // Add to symtab
        addVariable(name, nvar);

        // Generate toplayer related
        for (auto init_val : init_vals) {
            if (!init_val.second)
                LogError("[ERROR] Initial value of \""
                          + name + "\" is not a constant");
            dynamic_cast<eeyore::NativeVar *>(nvar.get())->
                putVal(init_val.first, init_val.second.value());
            auto index_const = std::make_shared<eeyore::IntValue>
                (init_val.first);
            auto val_const = std::make_shared<eeyore::IntValue>
                (init_val.second.value());
            auto inst = std::make_shared<eeyore::ArrayAssignInst>
                (nvar, std::move(index_const), std::move(val_const));
            cur_func_ -> pushInst(std::move(inst));
        }
    }
}

void Context::generateEeyoreOn(ConstIndexListASTNode *ast, eeyore::Program &prog) {
    LogError("[ERROR] Unexpected Case: ConstIndexListASTNode");
}

void Context::generateEeyoreOn(ConstInitValListASTNode *ast, eeyore::Program &prog) {
    LogError("[ERROR] Unexpected Case: ConstInitValListASTNode");
}

void Context::generateEeyoreOn(VarDefListASTNode *ast, eeyore::Program &prog) {
    for (auto &var_def : (*ast->var_def_list())) {
        var_def->generateEeyoreCode(*this, prog);
    }
}

void Context::generateEeyoreOn(VarDefASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(InitValListASTNode *ast, eeyore::Program &prog) {
    LogError("[ERROR] Unexpected Case: InitValListASTNode");
}

void Context::generateEeyoreOn(FuncDefASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(FuncFParamsASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(FuncFParamASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(BlockItemsASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(AssignASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(IfASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(WhileASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(BreakASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(ContinueASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(ReturnASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(LValASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(IndexListASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(BinaryExpASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(FunCallASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(FuncRParamsASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(IntASTNode *ast, eeyore::Program &prog) {
}

void Context::generateEeyoreOn(UnaryOpASTNode *ast, eeyore::Program &prog) {
}


