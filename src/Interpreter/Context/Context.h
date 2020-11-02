
#pragma once

#include <iostream>
#include "../../Parser/index.h"
#include "./util.h"


class Context {
    public:
    Enviourment* global = new Enviourment();
    
    void run(std::string &code) {
        this->executeAST(new AST_NODE { parse(code) }, this->global);
    };

    void run(AST_Block *code) {
        this->executeAST(new AST_NODE { code }, this->global);
    };

    MS_VALUE executeAST(AST_NODE* node, Enviourment* env) {
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
            case AST_Types::MS_VAR: {
                AST_Var* id = downcast<AST_Var*>(node);
                return env->get(id->value);
            };
            case AST_Types::MS_ASSIGN: {
                AST_Assign* assign = downcast<AST_Assign*>(node);
                AST_Var* name = downcast<AST_Var*>(assign->name);
                auto val = this->executeAST(assign->value, env);
                env->set(name->value, val);
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
                AST_Var* name = downcast<AST_Var*>(declare->name);
                auto val = this->executeAST(declare->value, env);
                env->define(name->value, val);
                return val;
            };
            case AST_Types::MS_IF: {
                AST_If* ms_if = downcast<AST_If*>(node);
                MS_VALUE cond = this->executeAST(ms_if->condition, env);
                if (!isFalsey(cond)) {
                    if (ms_if->ifTrue->index() != AST_Types::MS_BLOCK) return this->executeAST(ms_if->ifTrue, env);
                    Enviourment* newEnv = env->extend();
                    auto val = this->executeAST(ms_if->ifTrue, newEnv);
                    newEnv->clear();
                    return val;
                } else if (ms_if->ifFalse != NULL) {
                    if (ms_if->ifFalse->index() != AST_Types::MS_BLOCK) return this->executeAST(ms_if->ifFalse, env);
                    Enviourment* newEnv = env->extend();
                    auto val = this->executeAST(ms_if->ifFalse, newEnv);
                    newEnv->clear();
                    return val;
                }
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
};

