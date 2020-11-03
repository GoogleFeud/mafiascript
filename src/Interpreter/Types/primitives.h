#pragma once
#include "./types.h"
#include <unordered_map>

class _MS_Array {
    public:
    std::vector<MS_VALUE> entries;
    _MS_Array(std::vector<MS_VALUE> &values) : entries(values) {};
};

class _MS_Object {
    public:
    std::unordered_map<std::string, MS_VALUE> entries;
    _MS_Object(std::unordered_map<std::string, MS_VALUE> &values) : entries(values) {};
};

class Enviourment;
void deleteEnv(Enviourment* env);

class _MS_Function {
    Enviourment* scope;
    std::vector<std::string> params;
    AST_Block* body;

    _MS_Function(AST_Block* bod, std::vector<std::string> &params) : params(params) {
        body = bod;
    };

    _MS_Function(AST_Block* bod, std::vector<std::string> &params, Enviourment* env) : params(params) {
        body = bod;
        scope = env;
    };

    ~_MS_Function() {
        deleteEnv(scope);
        delete body;
    };

};