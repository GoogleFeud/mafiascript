#pragma once

#include <optional>
#include "expressions.h"
#include <vector>

class AST_Block {
    public:
    std::vector<AST_ANY*> nodes;
    AST_Block();
    AST_Block(AST_ANY *val) {
        this->nodes.push_back(val);
    };
    AST_Block(std::vector<AST_ANY*> &nodes) : nodes(nodes) {};

    void push(AST_ANY* node) {
        this->nodes.push_back(node);
    };
    
};

class AST_If {
    public:
    AST_EXPRESSION* condition;
    AST_Block* ifTrue;
    AST_Block* ifFalse;

    AST_If(AST_EXPRESSION *cond, AST_Block *ifTrue, AST_Block *ifFalse) {
        this->condition = cond;
        this->ifTrue = ifTrue;
        this->ifFalse = ifFalse;
    };

};

class AST_Loop {
    public:
    AST_EXPRESSION condition;
    std::optional<AST_EXPRESSION> before;
    std::optional<AST_EXPRESSION> after;

    AST_Loop(AST_EXPRESSION &cond) : condition(cond) {};
    AST_Loop(AST_EXPRESSION &cond, AST_EXPRESSION &after) : condition(cond), after(std::make_optional<AST_EXPRESSION>(after)) {};
    AST_Loop(AST_EXPRESSION &cond, AST_EXPRESSION &before, AST_EXPRESSION &after) : condition(cond), before(std::make_optional<AST_EXPRESSION>(before)), after(std::make_optional<AST_EXPRESSION>(after)) {};
};

class AST_Return {
    public:
    AST_EXPRESSION value;
    AST_Return() {
        this->value = AST_EXPRESSION { new AST_Null };
    };
    AST_Return(AST_EXPRESSION &val) : value(val) {};
};

class AST_Continue {
    public:
    AST_EXPRESSION execute;
    AST_Continue() {
        this->execute = AST_EXPRESSION { new AST_Null };
    };
    AST_Continue(AST_EXPRESSION &exec) : execute(exec) {};
};

   