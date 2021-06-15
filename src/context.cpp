#include "context.hpp"

void Context::walkAndGetConst(const std::vector<int> &widths, int now_pos,
    const ASTPtrListPtr &ast, ArrayConstList &result, int level) {
    for (auto &elem : (*ast)) {
        auto base_ptr = elem.get();
        auto derived_ptr = dynamic_cast<ConstInitValListASTNode *>(base_ptr);
        auto derived_ptr2 = dynamic_cast<InitValListASTNode *>(base_ptr);
        if (derived_ptr == nullptr && derived_ptr2 == nullptr) {
            // Value
            result.push_back(
                std::make_pair(now_pos++, base_ptr->eval(*this))
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
            if (derived_ptr != nullptr) {
                walkAndGetConst(widths, now_pos, 
                    derived_ptr->const_init_val_list(), result, next_level);
            } else {
                walkAndGetConst(widths, now_pos,
                    derived_ptr2->init_val_list(), result, next_level);
            }
            now_pos = next_pos;
        }
    }
    // Zero Padding
    for (int i = now_pos; i % widths[level-1] != 0; ++i) {
        result.push_back(std::make_pair(i, 0));
    }
}

void Context::walkAndGetVar(const std::vector<int> &widths, int now_pos,
    const ASTPtrListPtr &ast, ArrayVarList &result, int level,
    eeyore::Program &prog) {
    for (auto &elem : (*ast)) {
        auto base_ptr = elem.get();
        auto derived_ptr = dynamic_cast<InitValListASTNode *>(base_ptr);
        if (derived_ptr == nullptr) {
            // Value
            result.push_back(std::make_pair(now_pos++, base_ptr));
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
            walkAndGetVar(widths, now_pos, derived_ptr->init_val_list(),
                          result, next_level, prog);
            now_pos = next_pos;
        }
    }
    // Zero Padding
    for (int i = now_pos; i % widths[level-1] != 0; ++i) {
        result.push_back(std::make_pair(i, nullptr));
    }
}

void Context::getWidthsFromAST(const ASTPtrListPtr &ast,
    std::vector<int> &result, std::string name) {
    for (auto &const_exp : (*ast)) {
        auto ret = const_exp->eval(*this);
        if (ret)
            result.push_back(ret.value());
        else
            LogError("[ERROR] Index value of \"" 
                      + name + "\" is not a constant");
    }
    if (result.empty()) return;
    for (auto it = result.rbegin() + 1; it != result.rend(); ++it) {
        *it = (*it) * (*(it-1));
    }
} 

void Context::generateIndexEeyore(const ASTPtrListPtr &ast, 
    const std::vector<int> &widths, eeyore::Program &prog) {
    eeyore::VarPtr lastvar;
    for (unsigned pos = 1; pos < widths.size(); ++pos) {
        (*ast)[pos - 1]->generateEeyoreCode(*this, prog);
        auto tvar1 = cur_func_->getLastTemp();
        auto intv = std::make_shared<eeyore::IntValue>(widths[pos]);
        auto tvar2 = cur_func_->addTemp();
        auto inst = std::make_shared<eeyore::BinaryInst>
                        (Operator::kMul, tvar2, tvar1, intv);
        cur_func_->pushInst(std::move(inst));
        if (pos != 1) {
            auto tvar3 = cur_func_->addTemp();
            auto inst2 = std::make_shared<eeyore::BinaryInst>
                (Operator::kAdd, tvar3, tvar2, lastvar);
            cur_func_->pushInst(std::move(inst2));
            lastvar = tvar3;
        } else {
            lastvar = tvar2;
        }
    }
    (*ast)[widths.size() - 1]->generateEeyoreCode(*this, prog);
    auto var_n = cur_func_->getLastTemp();
    eeyore::VarPtr tvar_n;
    if (widths.size() != 1) {
        tvar_n = cur_func_->addTemp();
        auto inst_n = std::make_shared<eeyore::BinaryInst>
            (Operator::kAdd, tvar_n, var_n, lastvar);
        cur_func_->pushInst(std::move(inst_n));
    } else {
        tvar_n = var_n;
    }

    // Remember to *4
    auto tvar_final = cur_func_->addTemp();
    auto ival_final = std::make_shared<eeyore::IntValue>(4);
    auto inst_final = std::make_shared<eeyore::BinaryInst>
        (Operator::kMul, tvar_final, tvar_n, ival_final);
    cur_func_->pushInst(std::move(inst_final));
}

void Context::typeCoercion(const ASTNodePtr &ast) {
    if (ast->value_type() != ValueType::kBoolType) {
        auto cond_1 = cur_func_->getLastTemp();
        auto cond_2 = std::make_shared<eeyore::IntValue>(0);
        auto M = cur_func_->instNum();
        ast->true_list().push_back(M);
        ast->false_list().push_back(M+1);
        auto inst1 = std::make_shared<eeyore::CondInst>
            (Operator::kNeq, cond_1, cond_2);
        cur_func_->pushInst(std::move(inst1));
        auto inst2 = std::make_shared<eeyore::JumpInst>();
        cur_func_->pushInst(std::move(inst2));
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
    auto main_ptr = lookUpFunction("main");
    if (main_ptr->need_return()) {
        auto tvar = global_ctx_->addTemp();
        auto inst = std::make_shared<eeyore::AssignCallInst>(main_ptr, tvar);
        global_ctx_->pushInst(inst);
        auto inst2 = std::make_shared<eeyore::ReturnInst>(tvar);
        global_ctx_->pushInst(inst2);
    } else {
        auto inst = std::make_shared<eeyore::AssignCallInst>(main_ptr);
        global_ctx_->pushInst(inst);
        auto inst2 = std::make_shared<eeyore::ReturnInst>();
        global_ctx_->pushInst(inst2);
    }
    prog.pushFunction(global_ctx_);
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
    nvar->is_const_ = true;

    if (index_list_ptr->empty()) {
        // Variable Case
        std::optional<int> val = ast->const_init_val()->eval(*this);
        if (!val) LogError("[ERROR] Initial value of \"" 
                            + name + "\" is not a constant"); 
        nvar->putVal(val.value());

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
        getWidthsFromAST(index_list_ptr, widths, name);
        for (auto width : widths) {
            nvar->pushWidth(width);
        }
        ArrayConstList init_vals;
        walkAndGetConst(widths, 0, val_list_ptr, init_vals, 1);
        for (auto init_val : init_vals) {
            if (!init_val.second)
                LogError("[ERROR] Initial value of \""
                          + name + "\" is not a constant");
            nvar->putVal(init_val.first, init_val.second.value());
        }
         
        // Add to symtab
        addVariable(name, nvar);

        // Generate toplayer related
        for (auto init_val : init_vals) {
            auto index_const = std::make_shared<eeyore::IntValue>
                (init_val.first * 4);
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
    auto name = ast->ident();
    auto index_ptr = ast->const_index_list().get();
    auto &index_list_ptr = 
        dynamic_cast<ConstIndexListASTNode *>(index_ptr)->const_exps();
    auto nvar = cur_func_->addNative(); 

    if (index_list_ptr->empty()) {
        // Variable Case
        if (ast->init_val() == nullptr) {
            // Case: Not Initialized
            addVariable(name, nvar);
        } else if (cur_func_ == global_ctx_) {
            // Case: Toplayer Variable == Constant
            std::optional<int> val = ast->init_val()->eval(*this);

            nvar->putVal(val.value());
            
            // Add to symtab
            addVariable(name, nvar);
            
            // Generate toplayer related
            auto val_const = std::make_shared<eeyore::IntValue>(val.value());
            auto inst = std::make_shared<eeyore::AssignInst>
                            (nvar, std::move(val_const));
            cur_func_->pushInst(std::move(inst));
        } else {
            // Case: Variable in functions
            // Add to symtab
            addVariable(name, nvar);
            
            // Generate toplayer related
            ast->init_val()->generateEeyoreCode(*this, prog);
            auto temp_var = cur_func_->getLastTemp();
            auto inst = std::make_shared<eeyore::AssignInst>
                            (nvar, std::move(temp_var));
            cur_func_->pushInst(std::move(inst));
        }
    } else {
        // Array Case
        std::vector<int> widths;
        getWidthsFromAST(index_list_ptr, widths, name);
        for (auto width : widths) {
            nvar->pushWidth(width);
        }

        if (ast->init_val() == nullptr) {
            // Add to symtab
            addVariable(name, nvar);
        } else {
            auto val_ptr = ast->init_val().get();
            auto &val_list_ptr =
                dynamic_cast<InitValListASTNode *>(val_ptr)->init_val_list();

            if (cur_func_ == global_ctx_) {
                // Case: Top Variable == Constant
                ArrayConstList init_vals;
                walkAndGetConst(widths, 0, val_list_ptr, init_vals, 1);
                for (auto init_val : init_vals) {
                    if (!init_val.second)
                        LogError("[ERROR] Initial value of \""
                                  + name + "\" is not a constant");
                    nvar->putVal(init_val.first, init_val.second.value());
                }
             
                // Add to symtab
                addVariable(name, nvar);
    
                // Generate toplayer related
                for (auto init_val : init_vals) {
                    auto index_const = std::make_shared<eeyore::IntValue>
                        (init_val.first * 4);
                    auto val_const = std::make_shared<eeyore::IntValue>
                        (init_val.second.value());
                    auto inst = std::make_shared<eeyore::ArrayAssignInst>
                        (nvar, std::move(index_const), std::move(val_const));
                    cur_func_ -> pushInst(std::move(inst));
                }
            } else {
                // Case: Variable in functions
                ArrayVarList init_vals;
                walkAndGetVar(widths, 0, val_list_ptr, init_vals, 1, prog);
    
                // Add to symtab
                addVariable(name, nvar);
                
                // Generate toplayer related
                for (auto init_val : init_vals) {
                    auto index_const = std::make_shared<eeyore::IntValue>
                        (init_val.first * 4);
                    if (init_val.second) {
                        init_val.second->generateEeyoreCode(*this, prog);
                        auto temp_var = cur_func_->getLastTemp();
                        auto inst = std::make_shared<eeyore::ArrayAssignInst>
                            (nvar, std::move(index_const), std::move(temp_var));
                        cur_func_ -> pushInst(std::move(inst));
                    } else {
                        auto temp_var = std::make_shared<eeyore::IntValue>(0);
                        auto inst = std::make_shared<eeyore::ArrayAssignInst>
                            (nvar, std::move(index_const), std::move(temp_var));
                        cur_func_ -> pushInst(std::move(inst));
                    }
                }
            }
        }
    }
}

void Context::generateEeyoreOn(InitValListASTNode *ast, eeyore::Program &prog) {
    LogError("[ERROR] Unexpected Case: InitValListASTNode");
}

void Context::generateEeyoreOn(FuncDefASTNode *ast, eeyore::Program &prog) {
    auto name = ast->ident();
    auto &block = ast->block();
    cur_func_ = std::make_shared<eeyore::FunctionDef>
                    (name, global_ctx_->nativeNum(),
                     ast->return_type() == ReturnType::kIntType);
    prog.pushFunction(cur_func_);    
    addFunction(name, cur_func_);
    
    newScope();
    // Add paramters
    if (ast->func_f_params() != nullptr) {
        // Case: Has paramters
        auto params_ptr = ast->func_f_params().get();
        auto &params_list = dynamic_cast<FuncFParamsASTNode *>(params_ptr)
            ->func_f_params();
        
        for (auto &param : (*params_list)) {
            auto param_ptr = dynamic_cast<FuncFParamASTNode *>(param.get());
            auto var_name = param_ptr->ident();
            auto index_ptr = param_ptr->const_index_list().get();
    
            if (index_ptr == nullptr) {
                // Variable Case
                auto pvar = cur_func_->addParam();
    
                // Add to symtab
                addVariable(var_name, pvar);
            } else {
                // Array Case
                auto &index_list_ptr = dynamic_cast<ConstIndexListASTNode *>
                    (index_ptr)->const_exps();
                std::vector<int> widths;
                getWidthsFromAST(index_list_ptr, widths, var_name);
                auto pvar = cur_func_->addParam();
                pvar->pushWidth(0);
                for (auto width : widths) {
                    pvar->pushWidth(width);
                }
    
                // Add to symtab
                addVariable(var_name, pvar);
            }
        }
    }

    block->generateEeyoreCode(*this, prog);
    
    int M = cur_func_->instNum();
    cur_func_->backpatch(block->next_list(), M);
    if (ast->return_type() == ReturnType::kVoidType) {
        auto inst = std::make_shared<eeyore::ReturnInst>();
        cur_func_->pushInst(inst);
    } else {
        auto intv = std::make_shared<eeyore::IntValue>(-1);
        auto inst = std::make_shared<eeyore::ReturnInst>(std::move(intv));
        cur_func_->pushInst(inst);
    }

    deleteScope();
    cur_func_ = global_ctx_;
}

void Context::generateEeyoreOn(FuncFParamsASTNode *ast, eeyore::Program &prog) {
    LogError("[ERROR] Unexpected Case: FuncFParamsASTNode");
}

void Context::generateEeyoreOn(FuncFParamASTNode *ast, eeyore::Program &prog) {
    LogError("[ERROR] Unexpected Case: FuncFParamASTNode");
}

void Context::generateEeyoreOn(BlockItemsASTNode *ast, eeyore::Program &prog) {
    newScope();
    // L -> L1 M S
    // L -> S
    std::vector<int> next_list;
    int M = cur_func_->instNum();
    for (auto &block_item : (*ast->block_items())) {
        block_item->generateEeyoreCode(*this, prog);
        cur_func_->backpatch(next_list, M);
        next_list = block_item->next_list();
        M = cur_func_->instNum();
    }
    ast->next_list().insert(ast->next_list().end(),
                            next_list.begin(),
                            next_list.end());
    deleteScope();
}

void Context::generateEeyoreOn(AssignASTNode *ast, eeyore::Program &prog) {
    auto lval = dynamic_cast<LValASTNode *>(ast->lval().get());
    auto ident = lval->ident();
    auto index_ptr = lval->index_list().get();
    auto &index_list = dynamic_cast<IndexListASTNode *>(index_ptr)
                           ->index_list();
    if (index_list->empty()) {
        // Case: Variable
        auto lhs = lookUpVariable(ident);
        auto val = ast->exp()->eval(*this);
        if (!val) {
            ast->exp()->generateEeyoreCode(*this, prog);
            auto rhs = cur_func_->getLastTemp();
            auto inst = std::make_shared<eeyore::AssignInst>(lhs, rhs);
            cur_func_->pushInst(std::move(inst));
        } else {
            auto rhs = std::make_shared<eeyore::IntValue>(val.value());
            auto inst = std::make_shared<eeyore::AssignInst>(lhs, rhs);
            cur_func_->pushInst(std::move(inst));
        }
    } else {
        // Case: Array
        auto lhs_var = lookUpVariable(ident);
        std::vector<int> widths;
        auto ptr1 = dynamic_cast<eeyore::NativeVar *>(lhs_var.get());
        if (ptr1 != nullptr) widths = ptr1->widths();
        auto ptr2 = dynamic_cast<eeyore::ParamVar *>(lhs_var.get());
        if (ptr2 != nullptr) widths = ptr2->widths();
        generateIndexEeyore(index_list, widths, prog);
        auto tvar_n = cur_func_->getLastTemp();
        // Index value is in tvar_n
        auto val = ast->exp()->eval(*this);
        if (!val) {
            ast->exp()->generateEeyoreCode(*this, prog);
            auto rhs = cur_func_->getLastTemp();
            auto inst_acc = std::make_shared<eeyore::ArrayAssignInst>
                                (lhs_var, tvar_n, rhs);
            cur_func_->pushInst(std::move(inst_acc));
        } else {
            auto rhs = std::make_shared<eeyore::IntValue>(val.value());
            auto inst_acc = std::make_shared<eeyore::ArrayAssignInst>
                                (lhs_var, tvar_n, rhs);
            cur_func_->pushInst(std::move(inst_acc));
        }
    }
}

void Context::generateEeyoreOn(IfASTNode *ast, eeyore::Program &prog) {
    auto &cond = ast->cond();
    auto &then = ast->then();
    auto &else_then = ast->else_then();
    if (else_then == nullptr) {
        // if (B) M S1
        cond->generateEeyoreCode(*this, prog);
        typeCoercion(cond);
        int M = cur_func_->instNum();
        then->generateEeyoreCode(*this, prog);
        cur_func_->backpatch(cond->true_list(), M);
        ast->next_list().insert(ast->next_list().end(),
                                cond->false_list().begin(),
                                cond->false_list().end());
        ast->next_list().insert(ast->next_list().end(),
                                then->next_list().begin(),
                                then->next_list().end());
    } else {
        // if (B) M1 S1 N else M2 S2
        cond->generateEeyoreCode(*this, prog);
        typeCoercion(cond);
        int M1 = cur_func_->instNum();
        then->generateEeyoreCode(*this, prog);
        // N
        ast->next_list().push_back(cur_func_->instNum());
        auto inst = std::make_shared<eeyore::JumpInst>();
        cur_func_->pushInst(inst);
        // else
        int M2 = cur_func_->instNum();
        else_then->generateEeyoreCode(*this, prog);
        
        cur_func_->backpatch(cond->true_list(), M1);
        cur_func_->backpatch(cond->false_list(), M2);
        ast->next_list().insert(ast->next_list().end(),
                                then->next_list().begin(),
                                then->next_list().end());
        ast->next_list().insert(ast->next_list().end(),
                                else_then->next_list().begin(),
                                else_then->next_list().end());
    }
}

void Context::generateEeyoreOn(WhileASTNode *ast, eeyore::Program &prog) {
    auto &cond = ast->cond();
    auto &stmt = ast->stmt();
    auto temp_loop = cur_loop_;
    cur_loop_ = ast;

    /* Textbook version */
    /*
    int M1 = cur_func_->instNum();
    cond->generateEeyoreCode(*this, prog);
    typeCoercion(cond);
    int M2 = cur_func_->instNum();
    stmt->generateEeyoreCode(*this, prog);
    cur_func_->backpatch(stmt->next_list(), M1);
    cur_func_->backpatch(cond->true_list(), M2);
    ast->next_list().insert(ast->next_list().end(),
                            cond->false_list().begin(),
                            cond->false_list().end());
    auto inst = std::make_shared<eeyore::JumpInst>(cur_func_->lookUpLabel(M1));
    cur_func_->pushInst(std::move(inst));
    */
    
    /* Beautiful version */
    auto inst = std::make_shared<eeyore::JumpInst>();
    cur_func_->pushInst(inst);
    int M1 = cur_func_->instNum();
    stmt->generateEeyoreCode(*this, prog);
    int M2 = cur_func_->instNum();
    cond->generateEeyoreCode(*this, prog);
    typeCoercion(cond);
    cur_func_->backpatch(stmt->next_list(), M2);
    cur_func_->backpatch(cond->true_list(), M1);
    inst->updateGoto(cur_func_->lookUpLabel(M2));
    ast->next_list().insert(ast->next_list().end(),
                            cond->false_list().begin(),
                            cond->false_list().end());
    
    // recover
    cur_loop_ = temp_loop;
}

void Context::generateEeyoreOn(BreakASTNode *ast, eeyore::Program &prog) {
    cur_loop_->next_list().push_back(cur_func_->instNum());
    auto inst = std::make_shared<eeyore::JumpInst>();
    cur_func_->pushInst(std::move(inst));
}

void Context::generateEeyoreOn(ContinueASTNode *ast, eeyore::Program &prog) {
    cur_loop_->stmt()->next_list().push_back(cur_func_->instNum());
    auto inst = std::make_shared<eeyore::JumpInst>();
    cur_func_->pushInst(std::move(inst));
}

void Context::generateEeyoreOn(ReturnASTNode *ast, eeyore::Program &prog) {
    auto &exp = ast->exp();
    if (exp == nullptr) {
        auto inst = std::make_shared<eeyore::ReturnInst>();
        cur_func_->pushInst(std::move(inst));
    } else {
        auto val = exp->eval(*this);
        if (!val) {
            exp->generateEeyoreCode(*this, prog);
            auto ret = cur_func_->getLastTemp();
            auto inst = std::make_shared<eeyore::ReturnInst>(ret);
            cur_func_->pushInst(std::move(inst));
        } else {
            auto ret = std::make_shared<eeyore::IntValue>(val.value());
            auto inst = std::make_shared<eeyore::ReturnInst>(ret);
            cur_func_->pushInst(std::move(inst));
        }
    }
}

void Context::generateEeyoreOn(LValASTNode *ast, eeyore::Program &prog) {
    auto ident = ast->ident();
    auto index_ptr = ast->index_list().get();
    auto &index_list = dynamic_cast<IndexListASTNode *>(index_ptr)
                           ->index_list();
    if (index_list->empty()) {
        // Case: Variable
        auto rhs = lookUpVariable(ident);
        auto lhs = cur_func_->addTemp();
        auto inst = std::make_shared<eeyore::AssignInst>(lhs, rhs);
        cur_func_->pushInst(std::move(inst));
    } else {
        // Case: Array
        auto rhs_var = lookUpVariable(ident);
        std::vector<int> widths;
        auto ptr1 = dynamic_cast<eeyore::NativeVar *>(rhs_var.get());
        if (ptr1 != nullptr) widths = ptr1->widths();
        auto ptr2 = dynamic_cast<eeyore::ParamVar *>(rhs_var.get());
        if (ptr2 != nullptr) widths = ptr2->widths();
        generateIndexEeyore(index_list, widths, prog);
        auto rhs_index = cur_func_->getLastTemp();
        auto lhs = cur_func_->addTemp();
        auto inst = std::make_shared<eeyore::AssignArrayInst>
                        (lhs, rhs_var, rhs_index);
        cur_func_->pushInst(std::move(inst));
    }
    // Type Info
    ast->setValueType(ValueType::kIntType);
}

void Context::generateEeyoreOn(IndexListASTNode *ast, eeyore::Program &prog) {
    LogError("[ERROR] Unexpected Case: IndexListASTNode");
}

void Context::generateEeyoreOn(BinaryExpASTNode *ast, eeyore::Program &prog) {
    auto op = ast->op();
    auto &lhs = ast->lhs();
    auto &rhs = ast->rhs();
    if (op == Operator::kAnd) {
        lhs->generateEeyoreCode(*this, prog);
        typeCoercion(lhs);
        int M = cur_func_->instNum();
        rhs->generateEeyoreCode(*this, prog);
        typeCoercion(rhs);
        cur_func_->backpatch(lhs->true_list(), M);
        ast->true_list() = rhs->true_list();
        ast->false_list() = lhs->false_list();
        ast->false_list().insert(ast->false_list().end(),
                                 rhs->false_list().begin(),
                                 rhs->false_list().end());
        ast->setValueType(ValueType::kBoolType);
    } else if (op == Operator::kOr) {
        lhs->generateEeyoreCode(*this, prog);
        typeCoercion(lhs);
        int M = cur_func_->instNum();
        rhs->generateEeyoreCode(*this, prog);
        typeCoercion(rhs);
        cur_func_->backpatch(lhs->false_list(), M);
        ast->true_list() = lhs->true_list();
        ast->true_list().insert(ast->true_list().end(),
                                rhs->true_list().begin(),
                                rhs->true_list().end());
        ast->false_list() = rhs->false_list();
        ast->setValueType(ValueType::kBoolType);
    } else if (Operator::kEq <= op && op <= Operator::kGreaterEq) {
        // Relation Operator
        lhs->generateEeyoreCode(*this, prog);
        auto lvar = cur_func_->getLastTemp();
        rhs->generateEeyoreCode(*this, prog);
        auto rvar = cur_func_->getLastTemp();
        int M = cur_func_->instNum();
        ast->true_list().push_back(M);
        ast->false_list().push_back(M+1);
        auto inst1 = std::make_shared<eeyore::CondInst>(op, lvar, rvar);
        cur_func_->pushInst(std::move(inst1));
        auto inst2 = std::make_shared<eeyore::JumpInst>();
        cur_func_->pushInst(std::move(inst2));
        ast->setValueType(ValueType::kBoolType);
    } else {
        // Arithmetic Operator
        lhs->generateEeyoreCode(*this, prog);
        auto lvar = cur_func_->getLastTemp();
        rhs->generateEeyoreCode(*this, prog);
        auto rvar = cur_func_->getLastTemp();
        auto tvar = cur_func_->addTemp();
        auto inst = std::make_shared<eeyore::BinaryInst>(op, tvar, lvar, rvar);
        cur_func_->pushInst(std::move(inst));
        ast->setValueType(ValueType::kIntType);
    }
}

void Context::generateEeyoreOn(FunCallASTNode *ast, eeyore::Program &prog) {
    auto lineno = ast->lineno();
    auto ident = ast->ident();
    auto params_ptr = ast->func_r_params().get();
    
    if (params_ptr) {
        auto &params_list = dynamic_cast<FuncRParamsASTNode *>(params_ptr)
                                ->func_r_params();
        eeyore::VarPtrList ptrs;
        for (auto &param : (*params_list)) {
            param->generateEeyoreCode(*this, prog);
            ptrs.push_back(cur_func_->getLastTemp());
        }
        for (auto &ptr : ptrs) {
            auto inst = std::make_shared<eeyore::ParamInst>(ptr);
            cur_func_->pushInst(std::move(inst));
        }
    }

    auto func = lookUpFunction(ident);
    if (ident == "starttime" || ident == "stoptime") {
        auto linevar = std::make_shared<eeyore::IntValue>(lineno);
        auto inst1 = std::make_shared<eeyore::ParamInst>(std::move(linevar));
        cur_func_->pushInst(std::move(inst1));
        auto inst2 = std::make_shared<eeyore::AssignCallInst>(func);
        cur_func_->pushInst(std::move(inst2));
        ast->setValueType(ValueType::kIntType);
        return;
    } 
    
    if (func->need_return()) {
        auto tvar = cur_func_->addTemp();
        auto inst = std::make_shared<eeyore::AssignCallInst>
                        (func, tvar);
        cur_func_->pushInst(std::move(inst));
    } else {
        auto inst = std::make_shared<eeyore::AssignCallInst>(func);
        cur_func_->pushInst(std::move(inst));
    }
    ast->setValueType(ValueType::kIntType);
}

void Context::generateEeyoreOn(FuncRParamsASTNode *ast, eeyore::Program &prog) {
    LogError("[ERROR] Unexpected Case: FuncRParamsASTNode");
}

void Context::generateEeyoreOn(IntASTNode *ast, eeyore::Program &prog) {
    auto ptr = std::make_shared<eeyore::IntValue>(ast->val());
    auto tvar = cur_func_->addTemp();
    auto inst = std::make_shared<eeyore::AssignInst>(tvar, ptr);
    cur_func_->pushInst(std::move(inst));
    // Type Info
    ast->setValueType(ValueType::kIntType);
}

void Context::generateEeyoreOn(UnaryOpASTNode *ast, eeyore::Program &prog) {
    auto op = ast->op();
    auto &operand = ast->operand();
    if (op == Operator::kNot) {
        // Comment: 2021.04.27
        // There is some weird testcase:
        //     -!!x
        // satisfying syntax rule but strange in semantics
        // Code Part:
        // operand->generateEeyoreCode(*this, prog);
        // typeCoercion(operand);
        // ast->true_list() = operand->false_list();
        // ast->false_list() = operand->true_list();
        // ast->setValueType(ValueType::kBoolType);
        operand->generateEeyoreCode(*this, prog);
        auto rhs = cur_func_->getLastTemp();
        auto lhs = cur_func_->addTemp();
        auto inst = std::make_shared<eeyore::UnaryInst>(op, lhs, rhs);
        cur_func_->pushInst(std::move(inst));
        ast->setValueType(ValueType::kIntType);
    } else {
        // Arithmetic
        operand->generateEeyoreCode(*this, prog);
        auto rhs = cur_func_->getLastTemp();
        auto lhs = cur_func_->addTemp();
        if (op == Operator::kAdd) {
            auto inst = std::make_shared<eeyore::AssignInst>(lhs, rhs);
            cur_func_->pushInst(std::move(inst));
        } else {
            auto inst = std::make_shared<eeyore::UnaryInst>(op, lhs, rhs);
            cur_func_->pushInst(std::move(inst));
        }
        ast->setValueType(ValueType::kIntType);
    }
}


