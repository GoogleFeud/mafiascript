#pragma once

#include <optional>
#include "expressions.h"
#include <vector>

class AST_Block {
    public:
    std::vector<AST_NODE*> nodes;
    AST_Block() {};
    AST_Block(AST_NODE *val) {
        this->nodes.push_back(val);
    };
    AST_Block(std::vector<AST_NODE*> &nodes) : nodes(nodes) {};

    void push(AST_NODE* node) {
        this->nodes.push_back(node);
    };
    
};

class AST_If {
    public:
    AST_NODE* condition;
    AST_NODE* ifTrue;
    AST_NODE* ifFalse;

    AST_If(AST_NODE *cond, AST_NODE *ifTrue, AST_NODE *ifFalse) {
        this->condition = cond;
        this->ifTrue = ifTrue;
        this->ifFalse = ifFalse;
    };

};

class AST_Loop {
    public:
    AST_NODE condition;
    std::optional<AST_NODE> before;
    std::optional<AST_NODE> after;

    AST_Loop(AST_NODE &cond) : condition(cond) {};
    AST_Loop(AST_NODE &cond, AST_NODE &after) : condition(cond), after(std::make_optional<AST_NODE>(after)) {};
    AST_Loop(AST_NODE &cond, AST_NODE &before, AST_NODE &after) : condition(cond), before(std::make_optional<AST_NODE>(before)), after(std::make_optional<AST_NODE>(after)) {};
};

class AST_Return {
    public:
    AST_NODE value;
    AST_Return() {
        this->value = AST_NODE { new AST_Null };
    };
    AST_Return(AST_NODE &val) : value(val) {};
};

class AST_Continue {
    public:
    AST_NODE execute;
    AST_Continue() {
        this->execute = AST_NODE { new AST_Null };
    };
    AST_Continue(AST_NODE &exec) : execute(exec) {};
};

   