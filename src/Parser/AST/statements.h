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

class AST_Declare {
    public:
    std::string name;
    AST_NODE* value;
    bool isConst;

    AST_Declare(AST_NODE* name, AST_NODE* value, bool isConst = false) {
        this->name = downcast<AST_Var*>(name)->value;
        this->isConst = isConst;
        delete name;
        this->value = value;
    };

    ~AST_Declare() {
        delete value;
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

    ~AST_If() {
        delete condition;
        delete ifTrue;
        delete ifFalse;
    }

};

class AST_Loop {
    public:
    AST_NODE* condition;
    AST_NODE* body;
    AST_NODE* before;

    AST_Loop(AST_NODE* cond, AST_NODE* body) {
        condition = cond;
        this->body = body;
    };

    AST_Loop(AST_NODE* cond, AST_NODE* after, AST_NODE* body)  {
        condition = cond;
        if (body->index() != AST_Types::MS_BLOCK) {
          AST_Block* bl = new AST_Block(body);
          bl->push(after);
          this->body = new AST_NODE { bl };
        } else {
            downcast<AST_Block*>(body)->push(after);
            this->body = body;
        };
    };

    AST_Loop(AST_NODE* before, AST_NODE* cond, AST_NODE* after, AST_NODE* body) {
        condition = cond;
        this->before = before;
        if (body->index() != AST_Types::MS_BLOCK) {
          AST_Block* bl = new AST_Block(body);
          bl->push(after);
          this->body = new AST_NODE { bl };
        } else {
            downcast<AST_Block*>(body)->push(after);
            this->body = body;
        };
    };

    ~AST_Loop() {
        delete condition;
        delete before;
        delete body;
    };

};

class AST_Return {
    public:
    AST_NODE* value;
    AST_Return() {
        this->value = new AST_NODE { new AST_Null };
    };
    AST_Return(AST_NODE* val) {
        this->value = val;
    }
};

class AST_Continue {
    public:
    AST_NODE* execute;

    AST_Continue() {
        this->execute = NULL;
    };

    AST_Continue(AST_NODE* exec) {
        execute = exec;
    };

    ~AST_Continue() {
        delete execute;
    };

};

   