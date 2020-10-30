#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "ast.h"

class AST_String {
    public:
    std::string& value;
    AST_String(std::string& val) : value(val) {
    };
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

class AST_Array {
    public:
    std::vector<AST_EXPRESSION> entries;
    AST_Array() {};
    AST_Array(AST_EXPRESSION &exp) {
        this->entries.push_back(exp);
    };

    void push(AST_EXPRESSION &exp) {
        this->entries.push_back(exp);
    }
};

class AST_Object {
    public:
    std::unordered_map<std::string, AST_EXPRESSION> entries;

};

class AST_Var {
    public:
    std::string value;
    AST_Var(std::string &val) : value(val) {};
};


class AST_Null {};

