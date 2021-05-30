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
using ArrayVar = std::pair<int, eeyore::VarPtr>;
using ArrayVarList = std::vector<ArrayVar>;

class Context {
public:
    Context() {
        newScope();
        eeyore::ParamPtr param;
        global_var_num_ = 0;
        global_ctx_ = std::make_shared<eeyore::FunctionDef>("$global$", 0, false);
        cur_func_ = global_ctx_;
        cur_loop_ = nullptr;
        
        // int getint()
        lib_funcs_.insert({"getint", 
            std::make_shared<eeyore::FunctionDef>("getint", 0, true)});

        // int getch()
        lib_funcs_.insert({"getch", 
            std::make_shared<eeyore::FunctionDef>("getch", 0, true)});

        // int getarray(int [])
        lib_funcs_.insert({"getarray", 
            std::make_shared<eeyore::FunctionDef>("getarray", 0, true)});
        param = lib_funcs_["getarray"]->addParam();
        param->pushWidth(0);
       
        // void putint(int)
        lib_funcs_.insert({"putint",
            std::make_shared<eeyore::FunctionDef>("putint", 0, false)});
        param = lib_funcs_["putint"]->addParam(); 

        // void putch(int)
        lib_funcs_.insert({"putch",
            std::make_shared<eeyore::FunctionDef>("putch", 0, false)});
        param = lib_funcs_["putch"]->addParam();
        
        // void putarray(int, int[])
        lib_funcs_.insert({"putarray",
            std::make_shared<eeyore::FunctionDef>("putarray", 0, false)});
        param = lib_funcs_["putarray"]->addParam();
        param = lib_funcs_["putarray"]->addParam(); 
        param->pushWidth(0);

        // starttime() _sysy_starttime(__LINE__)
        lib_funcs_.insert({"starttime",
            std::make_shared<eeyore::FunctionDef>("_sysy_starttime", 0, false)});
        param = lib_funcs_["starttime"]->addParam();

        // stoptime() _sysy_stoptime(__LINE__)
        lib_funcs_.insert({"stoptime",
            std::make_shared<eeyore::FunctionDef>("_sysy_stoptime", 0, false)});
        param = lib_funcs_["stoptime"]->addParam();
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
    
    // Helper Functions
    void walkAndGetConst(const std::vector<int> &widths, int now_pos,
        const ASTPtrListPtr &ast, ArrayConstList &result, int level);
    void walkAndGetVar(const std::vector<int> &widths, int now_pos,
        const ASTPtrListPtr &ast, ArrayVarList &result, int level,
        eeyore::Program &prog);
    void getWidthsFromAST(const ASTPtrListPtr &ast, std::vector<int> &result,
        std::string name);
    void generateIndexEeyore(const ASTPtrListPtr &ast, 
        const std::vector<int> &widths, eeyore::Program &prog);
    void typeCoercion(const ASTNodePtr &operand);

    // Code Generation Function Declaration
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
    int global_var_num() const { return global_var_num_; }

private:
    int global_var_num_;
    eeyore::FuncPtr global_ctx_;
    eeyore::FuncPtr cur_func_;
    WhileASTNode *cur_loop_;
    FuncTable lib_funcs_;
    FuncTable user_funcs_;
    ScopePtrList scope_stack_;
};

#endif // __MINIC_CONTEXT_HPP__
