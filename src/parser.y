%{

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    int yylex(void);
    void yyerror(const char *);
    int yyparse(void);
#ifdef __cplusplus
}
#endif

%}

%union {
    char *str_val;
    int32_t int_val;
}

%token CONST INT VOID IF ELSE WHILE BREAK CONTINUE RETURN
%token <int_val> INT_CONST 
%token <str_val> IDENT 
%token OR_OP AND_OP EQ_OP NE_OP LE_OP GE_OP 

%%

    CompUnit
        : Decl
        | FuncDef
        | CompUnit Decl
        | CompUnit FuncDef
        ;

    Decl
        : ConstDecl
        | VarDecl
        ;

    ConstDecl
        : CONST INT ConstDefList ';'
        ;

    ConstDefList
        : ConstDef
        | ConstDefList ',' ConstDef
        ;

    ConstDef
        : IDENT ConstIndexList '=' ConstInitVal
        ;

    ConstIndexList
        : /* nothing */
        | ConstIndexList '[' ConstExp ']'
        ;

    ConstInitVal
        : ConstExp
        | '{' '}'
        | '{' ConstInitValList '}'
        ;

    ConstInitValList
        : ConstInitVal
        | ConstInitValList ',' ConstInitVal 
        ;

    VarDecl
        : INT VarDefList ';'
        ;

    VarDefList
        : VarDef
        | VarDefList ',' VarDef
        ;

    VarDef
        : IDENT ConstIndexList
        | IDENT ConstIndexList '=' InitVal
        ;

    InitVal
        : Exp
        | '{' '}'
        | '{' InitValList '}'
        ;

    InitValList
        : InitVal
        | InitValList ',' InitVal
        ;

    FuncDef
        : VOID IDENT '(' ')' Block
        | INT IDENT '(' ')' Block
        | VOID IDENT '(' FuncFParams ')' Block
        | INT IDENT '(' FuncFParams ')' Block
        ;

    FuncFParams
        : FuncFParam
        | FuncFParams ',' FuncFParam
        ;

    FuncFParam
        : INT IDENT
        | INT IDENT '[' ']' ConstIndexList
        ;

    Block
        : '{' BlockItems '}'
        ;

    BlockItems
        : /* nothing */
        | BlockItems BlockItem
        ;

    BlockItem
        : Decl
        | Stmt
        ;

    Stmt
        : LVal '=' Exp ';'
        | ';'
        | Exp ';'
        | Block
        | IF '(' Cond ')' Stmt
        | IF '(' Cond ')' Stmt ELSE Stmt
        | WHILE '(' Cond ')' Stmt
        | BREAK ';'
        | CONTINUE ';'
        | RETURN ';'
        | RETURN Exp ';'
        ;

    Exp
        : AddExp
        ;

    Cond
        : LOrExp
        ;

    LVal
        : IDENT IndexList
        ;

    IndexList
        : /* nothing */
        | IndexList '[' Exp ']'
        ;

    PrimaryExp
        : '(' Exp ')'
        | LVal
        | Number
        ;
    
    Number
        : INT_CONST
        ;

    UnaryExp
        : PrimaryExp
        | IDENT '(' ')'
        | IDENT '(' FuncRParams ')'
        | UnaryOp UnaryExp 
        ;

    UnaryOp
        : '+' 
        | '-'
        | '!'
        ;

    FuncRParams
        : Exp
        | FuncRParams ',' Exp
        ;

    MulExp
        : UnaryExp
        | MulExp '*' UnaryExp
        | MulExp '/' UnaryExp
        | MulExp '%' UnaryExp
        ;

    AddExp
        : MulExp
        | AddExp '+' MulExp
        | AddExp '-' MulExp
        ;

    RelExp
        : AddExp
        | RelExp '<' AddExp
        | RelExp '>' AddExp
        | RelExp LE_OP AddExp
        | RelExp GE_OP AddExp
        ;

    EqExp
        : RelExp
        | EqExp EQ_OP RelExp
        | EqExp NE_OP RelExp
        ;

    LAndExp
        : EqExp
        | LAndExp AND_OP EqExp
        ;

    LOrExp
        : LAndExp
        | LOrExp OR_OP LAndExp
        ;

    ConstExp
        : AddExp
        ;

%%

void
yyerror (const char *s)
{
    fprintf(stderr, "%s\n", s);
}
