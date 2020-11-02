#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "ast.h"

class AST_String {
    public:
    std::string value;
    AST_String(std::string& val) : value(val) {};
};

class AST_Number  {
    public:
    float value;
    AST_Number(float val) {
        this->value = val;
    };
};

class AST_Bool {
    public:
    bool value;
    AST_Bool(bool val) {
        this->value = val;
    }
};


class AST_List {
    public: 
    std::vector<AST_NODE*> entries;
    AST_List() {};
    AST_List(AST_NODE* firstEntry) {
        this->entries.push_back(firstEntry);
    };

    void push(AST_NODE* entry) {
        this->entries.push_back(entry);
    };
};

class AST_Array {
    public:
    AST_List* list;
    AST_Array() {};
    AST_Array(AST_List *list) {
        this->list = list;
    };
};

class AST_Object {
    public:
    std::unordered_map<std::string, AST_NODE> entries;

};

class AST_Var {
    public:
    std::string value;
    AST_Var(std::string &val) : value(val) {};
};


class AST_Null {};

