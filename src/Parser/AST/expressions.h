#pragma once
#include "primitives.h"
#include <vector>


class AST_Assign {
    public:
    AST_NODE* obj;
    AST_NODE* value;

    AST_Assign(AST_NODE* name, AST_NODE *value) {
        obj = name;
        this->value = value;
    };

    ~AST_Assign() {
        delete this->value;
    };

};


enum BINARY_Ops {
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_POWER,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER_THAN,
    OP_GREATER_OR_EQUAL,
    OP_LESS_THAN,
    OP_LESS_OR_EQUAL,
    OP_MODULO,
    OP_PLUS_ASSIGN
};

class AST_Binary {
    public:
    AST_NODE* left;
    AST_NODE* right;
    BINARY_Ops op;

    AST_Binary(AST_NODE *left, AST_NODE *right, BINARY_Ops op) {
        this->left = left;
        this->right = right;
        this->op = op;
    };

    ~AST_Binary() {
        delete left;
        delete right;
    };

};

class AST_And {
    public:
    AST_NODE* left;
    AST_NODE* right;

    AST_And(AST_NODE *left, AST_NODE *right) {
        this->left = left;
        this->right = right;
    };

    ~AST_And() {
        delete left;
        delete right;
    };
};

class AST_Or {
    public:
    AST_NODE* left;
    AST_NODE* right;

    AST_Or(AST_NODE *left, AST_NODE *right) {
        this->left = left;
        this->right = right;
    };

    ~AST_Or() {
        delete left;
        delete right;
    };

};

class AST_Function {
    public:
    std::vector<std::string> params;
    std::vector<std::string> captures;
    AST_NODE* body;

    AST_Function(AST_List *params, AST_NODE* body) {
        for (AST_NODE* node : params->entries) {
           this->params.push_back(downcast<AST_Var*>(node)->value);
        };
        delete params;
        this->body = body;
    };

        AST_Function(AST_List *params, AST_NODE* body, AST_List *captures) {
        for (AST_NODE* node : params->entries) {
           this->params.push_back(downcast<AST_Var*>(node)->value);
        };
        delete params;
        for (AST_NODE* node : captures->entries) {
           this->captures.push_back(downcast<AST_Var*>(node)->value);
        };
        delete captures;
        this->body = body;
    };

    ~AST_Function() {
        delete body;
    };

};

class AST_Call {
    public:
    AST_List* params;
    AST_NODE* obj;

    AST_Call(AST_List* params, AST_NODE* obj) {
        this->obj = obj;
        this->params = params;
    };

    ~AST_Call() {
        delete params;
    }
};

class AST_Typeof {
    public:
    AST_NODE* value;

    AST_Typeof(AST_NODE* v) {
        value = v;
    };

    ~AST_Typeof() {
        delete value;
    };

};

class AST_Ternery {
    public:
    AST_NODE* condition;
    AST_NODE* ifTrue;
    AST_NODE* ifFalse;

    AST_Ternery(AST_NODE* cond, AST_NODE* ifTrue, AST_NODE* ifFalse) {
        this->condition = cond;
        this->ifTrue = ifTrue;
        this->ifFalse = ifFalse;
    };

    ~AST_Ternery() {
        delete condition;
        delete ifTrue;
        delete ifFalse;
    };
    
};


class AST_Accessor {
    public:
    AST_NODE* start;
    std::vector<AST_NODE*> path;

    AST_Accessor(AST_NODE* start) {
        this->start = start;
    };

    AST_Accessor(AST_NODE* start, AST_NODE* first) {
        this->start = start;
        this->path.push_back(first);
    };

    void push(AST_NODE* p) {
        this->path.push_back(p);
    };

    ~AST_Accessor() {
        for (AST_NODE* node : path) {
            delete node;
        };
    };

};
