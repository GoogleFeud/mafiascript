
#pragma once

#include <iostream>
#include <chrono>

#include "../../Parser/index.h"
#include "./util.h"

struct ContextSettings {
    float loopTimeout = 0;
};

class Context {
    public:
    Enviourment* global = new Enviourment();
    ContextSettings settings;
    
    void run(std::string &code) {
        this->executeAST(new AST_NODE { parseAST(code) }, this->global);
    };

    void run(AST_Block *code) {
        this->executeAST(new AST_NODE { code }, this->global);
    };

    MS_VALUE executeAST(AST_NODE* node, Enviourment* env) {
        if (node == NULL) return MS_VALUE { nullptr };
        switch(node->index()) {
            case AST_Types::MS_STRING: {
            AST_String* str = downcast<AST_String*>(node);
            return MS_VALUE { std::make_shared<std::string>(str->value) };
            };
            case AST_Types::MS_NUMBER: {
                AST_Number* num = downcast<AST_Number*>(node);
                return MS_VALUE { num->value };
            };
            case AST_Types::MS_BOOL: {
                AST_Bool* bol = downcast<AST_Bool*>(node);
                return MS_VALUE { bol->value };
            };
            case AST_Types::MS_NULL: {
                return MS_VALUE { nullptr };
            };
            case AST_Types::MS_ARRAY: {
                AST_Array* arr = downcast<AST_Array*>(node);
                std::vector<MS_VALUE> realArr;
                for (AST_NODE* item : arr->list->entries) {
                    realArr.push_back(this->executeAST(item, env));
                };
                return MS_VALUE { std::make_shared<_MS_Array>(realArr) };
            };
            case AST_Types::MS_OBJECT: {
                AST_Object* obj = downcast<AST_Object*>(node);
                std::unordered_map<std::string, MS_VALUE> realMap;
                for (std::pair<AST_NODE*, AST_NODE*> pair : obj->entries->entries) {
                    AST_String* name = downcast<AST_String*>(pair.first);
                    auto val = this->executeAST(pair.second, env);
                    realMap.insert({name->value, val});
                };
                return MS_VALUE {std::make_shared<_MS_Object>(realMap) };
            };
            case AST_Types::MS_FUNCTION: {
                AST_Function* fn = downcast<AST_Function*>(node);
                MS_Function msFunc = std::make_shared<_MS_Function>(fn->body, fn->params, this->global->extend());
                if (fn->captures.size()) {
                    for (std::string capture : fn->captures) {
                        auto v = env->get(capture);
                        msFunc->scope->define(capture, v);
                    };
                };
                return MS_VALUE { msFunc };
            };
            case AST_Types::MS_VAR: {
                AST_Var* id = downcast<AST_Var*>(node);
                return env->get(id->value);
            };
            case AST_Types::MS_ASSIGN: {
                AST_Assign* assign = downcast<AST_Assign*>(node);
                auto val = this->executeAST(assign->value, env);
                env->set(assign->name, val);
                return val;
            };
            case AST_Types::MS_TERNERY: {
                AST_Ternery* ternery = downcast<AST_Ternery*>(node);
                MS_VALUE cond = this->executeAST(ternery->condition, env);
                if (!isFalsey(cond)) return this->executeAST(ternery->ifTrue, env);
                return this->executeAST(ternery->ifFalse, env);
            };
            case AST_Types::MS_BINARY: {
                AST_Binary* binary = downcast<AST_Binary*>(node);
                MS_VALUE a = this->executeAST(binary->left, env);
                MS_VALUE b = this->executeAST(binary->right, env);
                return applyOperator(a, b, binary->op);
            };
            case AST_Types::MS_DECLARE: {
                AST_Declare* declare = downcast<AST_Declare*>(node);
                auto val = this->executeAST(declare->value, env);
                env->define(declare->name, val);
                return MS_VALUE { nullptr };
            };
            case AST_Types::MS_IF: {
                AST_If* ms_if = downcast<AST_If*>(node);
                MS_VALUE cond = this->executeAST(ms_if->condition, env);
                if (!isFalsey(cond)) {
                    if (ms_if->ifTrue->index() != AST_Types::MS_BLOCK) return this->executeAST(ms_if->ifTrue, env);
                    Enviourment* newEnv = env->extend();
                    auto val = this->executeAST(ms_if->ifTrue, newEnv);
                    delete newEnv;
                    return val;
                } else if (ms_if->ifFalse != NULL) {
                    if (ms_if->ifFalse->index() != AST_Types::MS_BLOCK) return this->executeAST(ms_if->ifFalse, env);
                    Enviourment* newEnv = env->extend();
                    auto val = this->executeAST(ms_if->ifFalse, newEnv);
                    delete newEnv;
                    return val;
                }
                return MS_VALUE { nullptr };
            };
            case AST_Types::MS_LOOP: {
                AST_Loop* loop = downcast<AST_Loop*>(node);
                Enviourment* newEnv = env->extend();
                this->executeAST(loop->before, newEnv);
                if (this->settings.loopTimeout > 0) {
                    auto start = std::chrono::high_resolution_clock::now();
                   while(!isFalsey(this->executeAST(loop->condition, newEnv))) {
                       this->executeAST(loop->body, newEnv);
                       this->executeAST(loop->after, newEnv);
                       if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() > this->settings.loopTimeout) throw std::runtime_error("Loop timeout exceeded.");
                   };
                } else {
                    while(!isFalsey(this->executeAST(loop->condition, newEnv))) {
                       this->executeAST(loop->body, newEnv);
                       this->executeAST(loop->after, newEnv);
                   };
                };
                return MS_VALUE { nullptr };
            };
            case AST_Types::MS_BLOCK: {
                AST_Block* block = downcast<AST_Block*>(node);
                for (AST_NODE* blockNode : block->nodes) {
                    if (blockNode->index() == AST_Types::MS_RETURN) {
                        AST_Return* ret = downcast<AST_Return*>(blockNode);
                        auto v = this->executeAST(ret->value, env);
                        return v;
                    };
                    this->executeAST(blockNode, env);
                };
                return MS_VALUE { nullptr };
            };
        default: throw std::runtime_error("Invalid AST type!");
    };
    };

    MS_VALUE callFunction(MS_Function &func, std::vector<MS_VALUE> params) {
        int size = func->params.size();
        Enviourment* newEnv = func->scope->extend();
        for (int i=0; i < size; i++) {
            newEnv->define(func->params[i], params[i]);
        };
        auto val = this->executeAST(func->body, newEnv);
        delete newEnv;
        return val;
    };

    ~Context() {
        delete this->global;
    };
};

