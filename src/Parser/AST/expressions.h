#pragma once
#include "primitives.h"
#include <vector>

class AST_Assign {
    public:
    AST_NODE* name;
    AST_NODE* value;

    AST_Assign(AST_NODE* name, AST_NODE *value) {
        this->name = name;
        this->value = value;
    };

};

class AST_Binary {
    public:
    AST_NODE* left;
    AST_NODE* right;
    std::string op;

    AST_Binary(AST_NODE *left, AST_NODE *right, std::string &op) : op(op) {
        this->left = left;
        this->right = right;
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
    std::vector<AST_NODE> params;
    AST_Var name;

    AST_Call(std::vector<AST_NODE> &params, AST_Var& name) : params(params), name(name) {
    };

};

class AST_Ternery {
    public:
    AST_NODE condition;
    AST_NODE ifTrue;
    AST_NODE ifFalse;

    AST_Ternery(AST_NODE& cond, AST_NODE& ifTrue, AST_NODE& ifFalse) : condition(cond), ifTrue(ifTrue), ifFalse(ifFalse) {
    };
};
