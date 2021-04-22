#ifndef __MINIC_CONTEXT_HPP__
#define __MINIC_CONTEXT_HPP__

#include <memory>
#include <vector>
#include <cassert>
#include <utility>
#include <iostream>
#include <unordered_map>

#include "ast.hpp"
#include "eeyore.hpp"

using FuncTable = std::unordered_map<std::string, eeyore::FuncPtr>;
using VarTable = std::unordered_map<std::string, eeyore::VarPtr>;

class Scope {
public:
    bool addVariable(const std::string &name, eeyore::VarPtr info) {
        if (table_.find(name) != table_.end())
            return false;
        table_[name] = std::move(info);
        return true;
    }
    eeyore::VarPtr lookUpVariable(const std::string &name) {
        if (table_.find(name) != table_.end())
            return table_[name];
        return nullptr;
    }

private:
    VarTable table_;
};

using ScopePtr = std::unique_ptr<Scope>;
using ScopePtrList = std::vector<ScopePtr>;

using ArrayConst = std::pair<int, std::optional<int> >;
using ArrayConstList = std::vector<ArrayConst>;
using ArrayVar = std::pair<int, eeyore::TempVar>;
using ArrayVarList = std::vector<ArrayVar>;

class Context {
public:
    Context() {
        newScope();
        std::shared_ptr<eeyore::ParamVar> param;
        global_ctx_ = std::make_shared<eeyore::FunctionDef>("$global$", 0, 0);
        cur_func_ = global_ctx_;
        
        // int getint()
        lib_funcs_.insert({"getint", 
            std::make_shared<eeyore::FunctionDef>("getint", 0, 0)});

        // int getch()
        lib_funcs_.insert({"getch", 
            std::make_shared<eeyore::FunctionDef>("getch", 0, 0)});

        // int getarray(int [])
        lib_funcs_.insert({"getarray", 
            std::make_shared<eeyore::FunctionDef>("getarray", 0, 0)});
        param = std::make_shared<eeyore::ParamVar>(0);
        param->pushWidth(0);
        lib_funcs_["getarray"]->pushParam(std::move(param));
       
        // void putint(int)
        lib_funcs_.insert({"putint",
            std::make_shared<eeyore::FunctionDef>("putint", 0, 0)});
        param = std::make_shared<eeyore::ParamVar>(0);
        lib_funcs_["putint"]->pushParam(std::move(param));

        // void putch(int)
        lib_funcs_.insert({"putch",
            std::make_shared<eeyore::FunctionDef>("putch", 0, 0)});
        param = std::make_shared<eeyore::ParamVar>(0);
        lib_funcs_["putch"]->pushParam(std::move(param));
        
        // void putarray(int, int[])
        lib_funcs_.insert({"putarray",
            std::make_shared<eeyore::FunctionDef>("putarray", 0, 0)});
        param = std::make_shared<eeyore::ParamVar>(0);
        lib_funcs_["putarray"]->pushParam(std::move(param));
        param = std::make_shared<eeyore::ParamVar>(1);
        param->pushWidth(0);
        lib_funcs_["putarray"]->pushParam(std::move(param));

        // starttime() _sysy_starttime(__LINE__)
        lib_funcs_.insert({"starttime",
            std::make_shared<eeyore::FunctionDef>("starttime", 0, 0)});

        // stoptime() _sysy_stoptime(__LINE__)
        lib_funcs_.insert({"stoptime",
            std::make_shared<eeyore::FunctionDef>("stoptime", 0, 0)});
    }
    void newScope() {
        auto ptr = std::make_unique<Scope>();
        scope_stack_.push_back(std::move(ptr));
    }
    void deleteScope() {
        scope_stack_.pop_back();
    }
    bool addVariable(const std::string &name, eeyore::VarPtr info) {
        return (*scope_stack_.rbegin())->addVariable(name, std::move(info));
    }
    eeyore::VarPtr lookUpVariable(const std::string &name) {
        for (auto it = scope_stack_.rbegin(); it != scope_stack_.rend(); ++it) {
            auto result = (*it)->lookUpVariable(name);
            if (result != nullptr) return result;
        }
        return nullptr;
    }
    bool addFunction(const std::string &name, eeyore::FuncPtr info) {
        if (user_funcs_.find(name) != user_funcs_.end())
            return false;
        user_funcs_.insert({name, std::move(info)});
        return true;
    }
    eeyore::FuncPtr lookUpFunction(const std::string &name) {
        if (user_funcs_.find(name) != user_funcs_.end())
            return user_funcs_[name];
        if (lib_funcs_.find(name) != lib_funcs_.end()) 
            return lib_funcs_[name];
        return nullptr;
    }

    void walkAndGetConst(const std::vector<int> &widths, int now_pos,
        const ASTPtrListPtr &ast, ArrayConstList &result, int level);
    void walkAndGetVar(const std::vector<int> &widths, int now_pos,
        const ASTPtrListPtr &ast, ArrayVarList &result, int level);
    void generateEeyoreOn(CompUnitASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(ConstDefListASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(ConstDefASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(ConstIndexListASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(ConstInitValListASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(VarDefListASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(VarDefASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(InitValListASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(FuncDefASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(FuncFParamsASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(FuncFParamASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(BlockItemsASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(AssignASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(IfASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(WhileASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(BreakASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(ContinueASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(ReturnASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(LValASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(IndexListASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(BinaryExpASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(FunCallASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(FuncRParamsASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(IntASTNode *ast, eeyore::Program &prog);
    void generateEeyoreOn(UnaryOpASTNode *ast, eeyore::Program &prog);

    void LogError(const std::string &message) {
        std::cerr << message << std::endl;
        assert(false);
    }

private:
    eeyore::FuncPtr global_ctx_;
    eeyore::FuncPtr cur_func_;
    FuncTable lib_funcs_;
    FuncTable user_funcs_;
    ScopePtrList scope_stack_;
};

#endif // __MINIC_CONTEXT_HPP__
