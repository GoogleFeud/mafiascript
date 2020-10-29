#pragma once
#include "primitives.h"

class AST_Assign {
    public:
    bool isConst;
    AST_Var* name;
    AST_EXPRESSION value;

    AST_Assign(AST_Var* name, AST_EXPRESSION& value, bool isConst) : value(value) {
        this->name = name;
        this->isConst = isConst;
    };
};

class AST_Binary {
    public:
    AST_EXPRESSION left;
    AST_EXPRESSION right;
    std::string op;

    AST_Binary(AST_EXPRESSION &left, AST_EXPRESSION &right, std::string &op) : left(left), right(right), op(op) {
    };

};

class AST_Function {
    public:
    std::vector<AST_Var> params;

    AST_Function(std::vector<AST_Var> &params) : params(params) {

    };

};

class AST_Call {
    public:
    std::vector<AST_EXPRESSION> params;
    AST_Var name;

    AST_Call(std::vector<AST_EXPRESSION> &params, AST_Var& name) : params(params), name(name) {
    };

};

class AST_Ternery {
    public:
    AST_EXPRESSION condition;
    AST_EXPRESSION ifTrue;
    AST_EXPRESSION ifFalse;

    AST_Ternery(AST_EXPRESSION& cond, AST_EXPRESSION& ifTrue, AST_EXPRESSION& ifFalse) : condition(cond), ifTrue(ifTrue), ifFalse(ifFalse) {
    };
};