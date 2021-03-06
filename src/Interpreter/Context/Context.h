
#pragma once

#include <iostream>
#include <chrono>

#include "../../Parser/index.h"
#include "./util.h"

void __initGlobal(Enviourment* env);

class Context
{
public:
    Enviourment *global = new Enviourment();
    AST_NODE* entry;

    Context() {
        __initGlobal(global);
    }

    MS_POINTER run(std::string &code)
    {
        entry = new AST_NODE{parseAST(code)};
        return this->executeAST(entry, this->global);
    };

    MS_POINTER run(AST_Block *code)
    {
        entry = new AST_NODE{code};
        return this->executeAST(entry, this->global);
    };

    MS_POINTER repl(std::string &code) {
        auto parsed = parseAST(code);
        if (parsed->nodes.size() == 1) return this->executeAST(parsed->nodes[0], this->global);
        else return run(parsed);
    }
    
    MS_POINTER executeAST(AST_NODE *node, Enviourment *env)
    {
        if (node == NULL) return MS_VALUE::make();
        switch (node->index())
        {
        case AST_Types::MS_STRING:
        {
            AST_String *str = downcast<AST_String *>(node);
            return MS_VALUE::make(str->value);
        };
        case AST_Types::MS_NUMBER:
        {
            AST_Number *num = downcast<AST_Number *>(node);
            return MS_VALUE::make(num->value);
        };
        case AST_Types::MS_BOOL:
        {
            AST_Bool *bol = downcast<AST_Bool *>(node);
            return MS_VALUE::make(bol->value);
        };
        case AST_Types::MS_NULL:
        {
            return MS_VALUE::make();
        };
        case AST_Types::MS_ARRAY:
        {
            AST_Array *arr = downcast<AST_Array *>(node);
            std::vector<MS_POINTER> realArr;
            for (AST_NODE *item : arr->list->entries)
            {
                realArr.push_back(this->executeAST(item, env));
            };
            return MS_VALUE::make(realArr);
        };
        case AST_Types::MS_OBJECT:
        {
            AST_Object *obj = downcast<AST_Object*>(node);
            std::unordered_map<std::string, MS_POINTER> realMap;
            for (std::pair<AST_NODE *, AST_NODE *> pair : obj->entries->entries)
            {
                AST_String *name = downcast<AST_String *>(pair.first);
                auto val = this->executeAST(pair.second, env);
                realMap.insert({name->value, val});
            };
            return MS_VALUE::make(realMap);
        };
        case AST_Types::MS_FUNCTION:
        {
            AST_Function *fn = downcast<AST_Function *>(node);
            MS_Function msFunc = std::make_shared<_MS_Function>(fn->body, fn->params, this->global->extend(), this);
            if (fn->captures.size())
            {
                for (std::string capture : fn->captures)
                {
                    auto v = env->get(capture);
                    msFunc->scope->define(capture, v);
                };
            };
            return MS_VALUE::make(msFunc);
        };
        case AST_Types::MS_VAR: {
            AST_Var *id = downcast<AST_Var *>(node);
            return env->get(id->value);
        };
        case AST_Types::MS_ACCESSOR: {
            AST_Accessor *access = downcast<AST_Accessor*>(node);
            MS_POINTER last = this->executeAST(access->start, env);
            for (AST_NODE* el : access->path) {
                if (el->index() == AST_Types::MS_ACCESS_POINT) {
                    auto propKey = downcast<AST_Access_Point*>(el)->value;
                    if (last->isNull()) throw std::runtime_error("Cannot read property " + propKey + " of null");
                    last = last->getProperty(propKey);
                } else {
                    auto propKey = this->executeAST(el, env);
                     if (last->isNull()) throw std::runtime_error("Cannot read property " + propKey->toString() + " of null");
                     last = last->getProperty(propKey);
                };
            }; 
            return last;
        }; 
        case AST_Types::MS_ASSIGN:
        {
            AST_Assign *assign = downcast<AST_Assign *>(node);
            auto val = this->executeAST(assign->value, env);
            this->executeAST(assign->obj, env)->set(val);
            return val;
        };
        case AST_Types::MS_TERNERY:
        {
            AST_Ternery *ternery = downcast<AST_Ternery *>(node);
            MS_POINTER cond = this->executeAST(ternery->condition, env);
            if (!cond->isFalsey())
                return this->executeAST(ternery->ifTrue, env);
            return this->executeAST(ternery->ifFalse, env);
        };
        case AST_Types::MS_TYPEOF:
        {
            AST_Typeof *typeOf = downcast<AST_Typeof *>(node);
            auto val = this->executeAST(typeOf->value, env);
            std::string valType = val->typeToString();
            return MS_VALUE::make(valType);
        };
        case AST_Types::MS_CALL:
        {
            AST_Call *call = downcast<AST_Call *>(node);
            MS_POINTER functionObj = this->executeAST(call->obj, env);
            if (functionObj->index() == MS_Types::T_FUNCTION) {
                std::vector<MS_POINTER> paramVals;
                for (AST_NODE *val : call->params->entries)
                {
                    auto ex = this->executeAST(val, env);
                    paramVals.push_back(MS_VALUE::pass(ex));
                };
                return this->callFunction(functionObj, paramVals);
            }
            else if (functionObj->index() == MS_Types::C_FUNCTION)
            {
                C_Function fn = functionObj->downcast<C_Function>();
                std::vector<MS_POINTER> paramVals;
                for (AST_NODE *val : call->params->entries)
                {
                    paramVals.push_back(this->executeAST(val, env));
                };
                return fn(paramVals);
            }
            else throw std::runtime_error("Cannot call " + functionObj->typeToString());
        };
        case AST_Types::MS_BINARY: {
            AST_Binary *binary = downcast<AST_Binary *>(node);
            MS_POINTER a = this->executeAST(binary->left, env);
            MS_POINTER b = this->executeAST(binary->right, env);
            return applyOperator(a, b, binary->op);
        };
        case AST_Types::MS_UNARY: {
            AST_Unary* unary = downcast<AST_Unary*>(node);
            MS_POINTER a = this->executeAST(unary->value, env);
            return applyUnaryOperator(a, unary->op);
        }
        case AST_Types::MS_AND: {
            AST_And *_and = downcast<AST_And *>(node);
            MS_POINTER left = this->executeAST(_and->left, env);
            if (!left->isFalsey()) return this->executeAST(_and->right, env);
            return MS_VALUE::make(false);
        };
        case AST_Types::MS_OR: {
            AST_Or *_or = downcast<AST_Or *>(node);
            MS_POINTER left = this->executeAST(_or->left, env);
            if (left->isFalsey()) return this->executeAST(_or->right, env);
            return left;
        };
        case AST_Types::MS_DECLARE: {
            AST_Declare *declare = downcast<AST_Declare *>(node);
            auto val = this->executeAST(declare->value, env);
            auto passedValue = MS_VALUE::pass(val);
            if (declare->isConst) passedValue->flags.isConst = 1;
            env->define(declare->name, passedValue);
            return val;
        };
        case AST_Types::MS_IF:
        {
            AST_If *ms_if = downcast<AST_If *>(node);
            MS_POINTER cond = this->executeAST(ms_if->condition, env);
            if (!cond->isFalsey())
            {
                if (ms_if->ifTrue->index() != AST_Types::MS_BLOCK) {
                    if (ms_if->ifTrue->index() == AST_Types::MS_RETURN) return this->executeAST(ms_if->ifTrue, env);
                    this->executeAST(ms_if->ifTrue, env);
                } else {
                Enviourment *newEnv = env->extend();
                auto val = this->executeAST(ms_if->ifTrue, newEnv);
                delete newEnv;
                return val;
                }
            }
            else if (ms_if->ifFalse != NULL) {
                if (ms_if->ifFalse->index() != AST_Types::MS_BLOCK) {
                    if (ms_if->ifFalse->index() == AST_Types::MS_RETURN) return this->executeAST(ms_if->ifFalse, env);
                    this->executeAST(ms_if->ifFalse, env);
                } else {
                Enviourment *newEnv = env->extend();
                auto val = this->executeAST(ms_if->ifFalse, newEnv);
                delete newEnv;
                return val;
                }
            }
            return MS_VALUE::make();
        };
        case AST_Types::MS_LOOP: {
            AST_Loop *loop = downcast<AST_Loop *>(node);
            Enviourment *newEnv = env->extend();
            this->executeAST(loop->before, newEnv);;
             while (!this->executeAST(loop->condition, newEnv)->isFalsey()) {
                    auto tempEnv = newEnv->extend();
                    auto block = downcast<AST_Block *>(loop->body);
                    for (AST_NODE *nd : block->nodes) {
                        auto res = this->executeAST(nd, tempEnv);
                        if (res->index() != MS_Types::T_NULL && (nd->index() == AST_Types::MS_IF || nd->index() == AST_Types::MS_LOOP)) {
                            delete newEnv;
                            delete tempEnv;
                            return res;
                        }
                    };
                    delete tempEnv;
                };
            delete newEnv; 
            return MS_VALUE::make();
        };
        case AST_Types::MS_BLOCK: {
            AST_Block *block = downcast<AST_Block *>(node);
            for (AST_NODE *blockNode : block->nodes) {
                if (blockNode->index() == AST_Types::MS_RETURN) {
                    AST_Return *ret = downcast<AST_Return *>(blockNode);
                    auto v = this->executeAST(ret->value, env);
                    return v;
                };
                auto res = this->executeAST(blockNode, env);
                if (res->index() != MS_Types::T_NULL && (blockNode->index() == AST_Types::MS_IF || blockNode->index() == AST_Types::MS_LOOP)) return res;
            };
            return MS_VALUE::make();
        };
        case AST_Types::MS_RETURN: {
            AST_Return *rtrn = downcast<AST_Return *>(node);
            return this->executeAST(rtrn->value, env);
        };
        default: throw std::runtime_error("Invalid AST type!");
        };
    };

    MS_POINTER callFunction(MS_Function &func, std::vector<MS_POINTER> &params)
    {
        int size = func->params.size();
        Enviourment *newEnv = func->scope->extend();
        std::string args = "_arguments";
        auto argsArr = MS_VALUE::make(params);
        newEnv->unsafeSet(args, argsArr);
        for (int i = 0; i < size; i++)
        {
            auto astParam = func->params[i];
            auto param = params.size() > i ? params[i]:MS_VALUE::make();
            if (param->index() == MS_Types::T_NULL && astParam->defaultValue != nullptr) param = executeAST(astParam->defaultValue, newEnv);
            newEnv->define(astParam->value, param);
        };
        auto val = this->executeAST(func->body, newEnv);
        delete newEnv;
        return val;
    };

    MS_POINTER callFunction(MS_POINTER &fnObj, std::vector<MS_POINTER> &params)
    {
        MS_Function func = fnObj->downcast<MS_Function>();
        int size = func->params.size();
        Enviourment *newEnv = func->scope->extend();
        std::string args = "_arguments";
        auto argsArr = MS_VALUE::make(params);
        newEnv->unsafeSet(args, argsArr);
        for (int i = 0; i < size; i++)
        {
            auto astParam = func->params[i];
            auto param = params.size() > i ? params[i]:MS_VALUE::make();
            if (param->index() == MS_Types::T_NULL && astParam->defaultValue != nullptr) param = executeAST(astParam->defaultValue, newEnv);
            newEnv->define(astParam->value, param);
        };
        auto val = this->executeAST(func->body, newEnv);
        delete newEnv;
        return val;
    };

    ~Context()
    {
        delete global;
        delete entry;
    };

};


MS_POINTER _callFunction(Context* ctx, MS_POINTER &fnObj, std::vector<MS_POINTER> &params) {
    if (fnObj->index() == MS_Types::T_FUNCTION) return ctx->callFunction(fnObj, params);
    else if (fnObj->index() == MS_Types::C_FUNCTION) return fnObj->downcast<C_Function>()(params);
    else throw std::runtime_error("Provided argument must be a function!");
};


MS_POINTER _callFunction(Context* ctx, MS_Function &fnObj, std::vector<MS_POINTER> &params) {
    return ctx->callFunction(fnObj, params);
};