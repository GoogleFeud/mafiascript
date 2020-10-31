
#pragma once

#include <iostream>
#include "../../Parser/index.h"
#include "./util.h"


class Context {
    public:
    Enviourment* global;
    
    MS_VALUE run(std::string &code) {
        return executeAST(new AST_NODE { parse(code) }, this->global);
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
            case AST_Types::MS_BINARY: {
                AST_Binary* binary = downcast<AST_Binary*>(node);
                MS_VALUE a = this->executeAST(binary->left, env);
                MS_VALUE b = this->executeAST(binary->right, env);
                return applyOperator(a, b, binary->op);
            }
            case AST_Types::MS_IF: {
                AST_If* ms_if = downcast<AST_If*>(node);
                MS_VALUE cond = this->executeAST(ms_if->condition, env);
                if (!isFalsey(cond)) {
                    if (downcast<AST_Block*>(ms_if->ifTrue)->nodes.size() == 1) this->executeAST(ms_if->ifTrue, env);
                    else {
                    Enviourment* newEnv = env->extend();
                    this->executeAST(ms_if->ifTrue, newEnv);
                    newEnv->clear();
                    }
                } else if (ms_if->ifFalse) {
                    if (downcast<AST_Block*>(ms_if->ifFalse)->nodes.size() == 1) this->executeAST(ms_if->ifFalse, env);
                    else {
                    Enviourment* newEnv = env->extend();
                    this->executeAST(ms_if->ifFalse, newEnv);
                    newEnv->clear();
                    }
                }
                break;
            };
            case AST_Types::MS_BLOCK: {
                AST_Block* block = downcast<AST_Block*>(node);
                if (block->nodes.size() == 1) return this->executeAST(block->nodes[0], env);
                for (AST_NODE* blockNode : block->nodes) {
                    if (blockNode->index() == AST_Types::MS_RETURN) {
                        AST_Return* ret = downcast<AST_Return*>(blockNode);
                        return this->executeAST(ret->value, env);
                    };
                    this->executeAST(blockNode, env);
                };
                break;
            };
        }
    }



};

