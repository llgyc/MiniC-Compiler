%{

#include <memory>

#include <stdio.h>
#include <stdint.h>

#include "ast.hpp"
#include "operator.hpp"

extern int yylex(void);
extern void yyerror(BaseASTNode **, const char *);
extern int yyparse(BaseASTNode **);

%}

%parse-param {BaseASTNode **root_ptr_ptr}

%union {
    char *str_val;
    int int_val;
    BaseASTNode *node_ptr;
}

%token CONST INT VOID IF WHILE BREAK CONTINUE RETURN
%right THEN ELSE
%token <int_val> INT_CONST 
%token <str_val> IDENT 
%token OR_OP AND_OP EQ_OP NE_OP LE_OP GE_OP 

// type of non-terminals
%type <node_ptr> CompUnit Decl ConstDecl ConstDefList ConstDef
%type <node_ptr> ConstIndexList ConstInitVal ConstInitValList
%type <node_ptr> VarDecl VarDefList VarDef InitVal InitValList
%type <node_ptr> FuncDef FuncFParams FuncFParam Block BlockItems
%type <node_ptr> BlockItem Stmt Exp Cond LVal IndexList PrimaryExp
%type <node_ptr> UnaryExp FuncRParams MulExp AddExp RelExp
%type <node_ptr> EqExp LAndExp LOrExp ConstExp

%%

    ProgramStart
        : CompUnit
          { *root_ptr_ptr = $1; }
        ;

    CompUnit
        : Decl
          { ASTNodePtr ptr($1);
            $$ = new CompUnitASTNode(std::move(ptr));
          }
        | FuncDef
          { ASTNodePtr ptr($1);
            $$ = new CompUnitASTNode(std::move(ptr));
          }
        | CompUnit Decl
          { ASTNodePtr ptr1($1), ptr2($2);
            $$ = new CompUnitASTNode(std::move(ptr1), std::move(ptr2));
          }
        | CompUnit FuncDef
          { ASTNodePtr ptr1($1), ptr2($2);
            $$ = new CompUnitASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    Decl
        : ConstDecl
        | VarDecl
        ;

    ConstDecl
        : CONST INT ConstDefList ';'
          { $$ = $3; }
        ;

    ConstDefList
        : ConstDef
          { ASTNodePtr ptr($1);
            $$ = new ConstDefListASTNode(std::move(ptr));
          }
        | ConstDefList ',' ConstDef
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new ConstDefListASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    ConstDef
        : IDENT ConstIndexList '=' ConstInitVal
          { std::string ident($1);
            ASTNodePtr ptr1($2), ptr2($4);
            $$ = new ConstDefASTNode(ident, std::move(ptr1), std::move(ptr2));
          }
        ;

    ConstIndexList
        : /* nothing */
          { $$ = new ConstIndexListASTNode(); }
        | ConstIndexList '[' ConstExp ']'
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new ConstIndexListASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    ConstInitVal
        : ConstExp
        | '{' '}'
          { $$ = new ConstInitValListASTNode(); }
        | '{' ConstInitValList '}'
          { $$ = $2; }
        ;

    ConstInitValList
        : ConstInitVal
          { ASTNodePtr ptr($1);
            $$ = new ConstInitValListASTNode(std::move(ptr));
          }
        | ConstInitValList ',' ConstInitVal 
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new ConstInitValListASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    VarDecl
        : INT VarDefList ';'
          { $$ = $2; }
        ;

    VarDefList
        : VarDef
          { ASTNodePtr ptr($1);
            $$ = new VarDefListASTNode(std::move(ptr));
          }
        | VarDefList ',' VarDef
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new VarDefListASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    VarDef
        : IDENT ConstIndexList
          { std::string ident($1);
            ASTNodePtr ptr($2);
            $$ = new VarDefASTNode(ident, std::move(ptr));
          }
        | IDENT ConstIndexList '=' InitVal
          { std::string ident($1);
            ASTNodePtr ptr1($2), ptr2($4);
            $$ = new VarDefASTNode(ident, std::move(ptr1), std::move(ptr2));
          }
        ;

    InitVal
        : Exp
        | '{' '}'
          { $$ = new InitValListASTNode(); }
        | '{' InitValList '}'
          { $$ = $2; }
        ;

    InitValList
        : InitVal
          { ASTNodePtr ptr1($1);
            $$ = new InitValListASTNode(std::move(ptr1));
          }
        | InitValList ',' InitVal
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new InitValListASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    FuncDef
        : VOID IDENT '(' ')' Block
          { std::string ident;
            ASTNodePtr ptr($5);
            $$ = new FuncDefASTNode(ReturnType::kVoidType, 
                ident, std::move(ptr));
          }
        | INT IDENT '(' ')' Block
          { std::string ident;
            ASTNodePtr ptr($5);
            $$ = new FuncDefASTNode(ReturnType::kIntType,
                ident, std::move(ptr));
          }
        | VOID IDENT '(' FuncFParams ')' Block
          { std::string ident;
            ASTNodePtr ptr1($4), ptr2($6);
            $$ = new FuncDefASTNode(ReturnType::kVoidType,
                ident, std::move(ptr1), std::move(ptr2));
          }
        | INT IDENT '(' FuncFParams ')' Block
          { std::string ident;
            ASTNodePtr ptr1($4), ptr2($6);
            $$ = new FuncDefASTNode(ReturnType::kIntType,
                ident, std::move(ptr1), std::move(ptr2));
          }
        ;

    FuncFParams
        : FuncFParam
          { ASTNodePtr ptr1($1);
            $$ = new FuncFParamsASTNode(std::move(ptr1));
          }
        | FuncFParams ',' FuncFParam
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new FuncFParamsASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    FuncFParam
        : INT IDENT
          { std::string ident($2);
            $$ = new FuncFParamASTNode(ident);
          }
        | INT IDENT '[' ']' ConstIndexList
          { std::string ident($2);
            ASTNodePtr ptr($5);
            $$ = new FuncFParamASTNode(ident, std::move(ptr));
          }
        ;

    Block
        : '{' BlockItems '}'
          { $$ = $2; }
        ;

    BlockItems
        : /* nothing */
          { $$ = new BlockItemsASTNode(); }
        | BlockItems BlockItem
          { ASTNodePtr ptr1($1), ptr2($2);
            $$ = new BlockItemsASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    BlockItem
        : Decl
        | Stmt
        ;

    Stmt
        : LVal '=' Exp ';'
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new AssignASTNode(std::move(ptr1), std::move(ptr2));
          }
        | ';'
          { $$ = nullptr; }
        | Exp ';'
        | Block
        | IF '(' Cond ')' Stmt %prec THEN
          { ASTNodePtr ptr1($3), ptr2($5);
            $$ = new IfASTNode(std::move(ptr1), std::move(ptr2));
          }
        | IF '(' Cond ')' Stmt ELSE Stmt
          { ASTNodePtr ptr1($3), ptr2($5), ptr3($7);
            $$ = new IfASTNode(std::move(ptr1), std::move(ptr2), std::move(ptr3));
          }
        | WHILE '(' Cond ')' Stmt
          { ASTNodePtr ptr1($3), ptr2($5);
            $$ = new WhileASTNode(std::move(ptr1), std::move(ptr2));
          }
        | BREAK ';'
          { $$ = new BreakASTNode(); }
        | CONTINUE ';'
          { $$ = new ContinueASTNode(); }
        | RETURN ';'
          { $$ = new ReturnASTNode(); }
        | RETURN Exp ';'
          { ASTNodePtr ptr1($2);
            $$ = new ReturnASTNode(std::move(ptr1));
          }
        ;

    Exp
        : AddExp
        ;

    Cond
        : LOrExp
        ;

    LVal
        : IDENT IndexList
          { std::string ident($1);
            ASTNodePtr ptr($2);
            $$ = new LValASTNode(ident, std::move(ptr));
          }
        ;

    IndexList
        : /* nothing */
          { $$ = new IndexListASTNode(); }
        | IndexList '[' Exp ']'
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new IndexListASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    PrimaryExp
        : '(' Exp ')'
          { $$ = $2; }
        | LVal
        | INT_CONST
          { $$ = new IntASTNode($1); }
        ;

    UnaryExp
        : PrimaryExp
        | IDENT '(' ')'
          { std::string ident;
            $$ = new FunCallASTNode(ident);
          }
        | IDENT '(' FuncRParams ')'
          { std::string ident;
            ASTNodePtr ptr($3);
            $$ = new FunCallASTNode(ident, std::move(ptr));
          }
        | '+' UnaryExp 
          { ASTNodePtr ptr($2);
            $$ = new UnaryOpASTNode(Operator::kAdd, std::move(ptr));
          }
        | '-' UnaryExp
          { ASTNodePtr ptr($2);
            $$ = new UnaryOpASTNode(Operator::kSub, std::move(ptr));
          }
        | '!' UnaryExp
          { ASTNodePtr ptr($2);
            $$ = new UnaryOpASTNode(Operator::kNot, std::move(ptr));
          }
        ;

    FuncRParams
        : Exp
          { ASTNodePtr ptr1($1);
            $$ = new FuncRParamsASTNode(std::move(ptr1));
          }
        | FuncRParams ',' Exp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new FuncRParamsASTNode(std::move(ptr1), std::move(ptr2));
          }
        ;

    MulExp
        : UnaryExp
        | MulExp '*' UnaryExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kMul, 
                std::move(ptr1), std::move(ptr2));
          }
        | MulExp '/' UnaryExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kDiv, 
                std::move(ptr1), std::move(ptr2));
          }
        | MulExp '%' UnaryExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kMod, 
                std::move(ptr1), std::move(ptr2));
          }
        ;

    AddExp
        : MulExp
        | AddExp '+' MulExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kAdd, 
                std::move(ptr1), std::move(ptr2));
          }
        | AddExp '-' MulExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kSub, 
                std::move(ptr1), std::move(ptr2));
          }
        ;

    RelExp
        : AddExp
        | RelExp '<' AddExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kLess, 
                std::move(ptr1), std::move(ptr2));
          }
        | RelExp '>' AddExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kGreater, 
                std::move(ptr1), std::move(ptr2));
          }
        | RelExp LE_OP AddExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kLessEq, 
                std::move(ptr1), std::move(ptr2));
          }
        | RelExp GE_OP AddExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kGreaterEq, 
                std::move(ptr1), std::move(ptr2));
          }
        ;

    EqExp
        : RelExp
        | EqExp EQ_OP RelExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kEq, 
                std::move(ptr1), std::move(ptr2));
          }
        | EqExp NE_OP RelExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kNeq, 
                std::move(ptr1), std::move(ptr2));
          }
        ;

    LAndExp
        : EqExp
        | LAndExp AND_OP EqExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kAnd, 
                std::move(ptr1), std::move(ptr2));
          }
        ;

    LOrExp
        : LAndExp
        | LOrExp OR_OP LAndExp
          { ASTNodePtr ptr1($1), ptr2($3);
            $$ = new BinaryExpASTNode(Operator::kOr, 
                std::move(ptr1), std::move(ptr2));
          }
        ;

    ConstExp
        : AddExp
        ;

%%

void
yyerror (BaseASTNode **root_ptr_ptr, const char *s)
{
    fprintf(stderr, "%s\n", s);
}
