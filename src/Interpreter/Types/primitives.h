#pragma once
#include "./types.h"
#include <unordered_map>
#include <functional>
#include <stdarg.h>


class Enviourment;
void deleteEnv(Enviourment* env);

class _MS_Function {
    public:
    Enviourment* scope;
    std::vector<std::string> params;
    AST_NODE* body;

    _MS_Function(AST_NODE* bod, std::vector<std::string> &params, Enviourment* env) : params(params) {
        body = bod;
        scope = env;
    };

    ~_MS_Function() {
        deleteEnv(scope);
        delete body;
    };

}; 

class _C_Function {
    public:
    MS_VALUE (*fn)(std::vector<MS_VALUE>, Enviourment* env);

    _C_Function(MS_VALUE (*f)(std::vector<MS_VALUE>, Enviourment* env)) {
        fn = f;
    };

};


class _MS_Array {
    public:
    std::vector<MS_VALUE> entries;
    std::unordered_map<std::string, MS_VALUE> properties;
    _MS_Array(std::vector<MS_VALUE> &values) : entries(values) {
    };

    MS_VALUE push(std::vector<MS_VALUE> vals, Enviourment* env) {
        for (MS_VALUE val : vals) {
            this->entries.push_back(val);
        };
        return MS_VALUE { (float)this->entries.size() };
    };
};

class _MS_Object {
    public:
    std::unordered_map<std::string, MS_VALUE> properties;
    _MS_Object(std::unordered_map<std::string, MS_VALUE> &values) : properties(values) {};
};
