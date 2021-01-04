#pragma once
#include <variant>
#include <memory>
#include <vector>
#include <functional>

enum MS_Types {
    T_STRING,
    T_NUMBER,
    T_BOOL,
    T_NULL,
    T_ARRAY,
    T_FUNCTION,
    C_FUNCTION,
    T_OBJECT
};
;


class MS_VALUE;
using MS_POINTER = std::shared_ptr<MS_VALUE>;
class Enviourment;
void deleteEnv(Enviourment* env);
class Context;

void __initString(MS_VALUE *str);
void __initArray(MS_VALUE *arr);
void __initNumber(MS_VALUE *num);
void __initBoolean(MS_VALUE *boo);
void __initFunc(MS_VALUE *func);

class _MS_Function {
    public:
    Context* ctx;
    Enviourment* scope;
    std::vector<AST_Var*> params;
    AST_NODE* body;

    _MS_Function(AST_NODE* bod, std::vector<AST_Var*> &params, Enviourment* env, Context* ctx) : params(params) {
        body = bod;
        scope = env;
        this->ctx = ctx;
    };

    ~_MS_Function() {
        deleteEnv(scope);
        for (AST_Var* param : params) {
            delete param;
        }

        delete body;
    };

}; 

class _MS_Array {
    public:
    std::vector<MS_POINTER> entries;
    _MS_Array(std::vector<MS_POINTER> &values) : entries(values) {
    };
    _MS_Array() {};

};


using MS_Function = std::shared_ptr<_MS_Function>;
using MS_Array = std::shared_ptr<_MS_Array>;

MS_POINTER _callFunction(Context* ctx, MS_Function &fn, std::vector<MS_POINTER> &params);

using C_Function = std::function<MS_POINTER(std::vector<MS_POINTER>)>;
using RAW_MS_VALUE = std::variant<std::string, float, bool, std::nullptr_t, MS_Array, MS_Function, C_Function, MS_Types>;

struct VALUE_FLAGS {
    unsigned int isConst: 1;
};

class MS_VALUE {
    public: 
    RAW_MS_VALUE value;
    std::unordered_map<std::string, MS_POINTER> properties;
    VALUE_FLAGS flags;

    MS_VALUE(const std::string &str) {
        value = str;
        __initString(this);
    };

    MS_VALUE(float num) {
        value = num;
        __initNumber(this);
    };

    MS_VALUE(bool boolean) {
        value = boolean;
        __initBoolean(this);
    }; 

    MS_VALUE() {
        value = nullptr;
    };

    MS_VALUE(std::vector<MS_POINTER> &arr) {
        value = std::make_shared<_MS_Array>(arr);
        __initArray(this);
    };

    MS_VALUE(MS_Function &fn) {
        value = fn;
        __initFunc(this);
    };

    MS_VALUE(C_Function fn) {
        value = fn;
    };

    MS_VALUE(std::unordered_map<std::string, MS_POINTER> &map) {
        properties = map;
        value = MS_Types::T_OBJECT;
    };

    MS_VALUE(MS_VALUE (*fn)(std::vector<MS_VALUE>)) {
        value = fn;
    };

    size_t index() {
        return value.index();
    };

    void set(MS_POINTER &val) {
        if (flags.isConst) throw std::runtime_error("Cannot assign to a constant variable");
        this->value = val->value;
        this->properties = val->properties;
        this->flags = val->flags;
    };

    template<typename T>
    T downcast() {
        return std::get<T>(value);
    }

    bool isNull() {
        return value.index() == MS_Types::T_NULL;
    };

    const std::string typeToString() {
        switch (value.index()) {
        case MS_Types::T_STRING:
            return "string";
        case MS_Types::T_NUMBER:
            return "number";
        case MS_Types::T_BOOL:
            return "boolean";
        case MS_Types::T_NULL:
            return "null";
        case MS_Types::T_ARRAY:
            return "array";
        case MS_Types::T_FUNCTION:
            return "function";
        case MS_Types::C_FUNCTION:
            return "c-function";
        default:
            return "object";
        };
    };

    std::string toString() {
        switch(value.index()) {
            case MS_Types::T_STRING: return std::get<std::string>(value);
            case MS_Types::T_NUMBER: return std::to_string(std::get<float>(value));
            case MS_Types::T_BOOL: {
                bool val = std::get<bool>(value);
                return (val) ? "true":"false";
            };
            case MS_Types::T_NULL: return "null";
            case MS_Types::T_ARRAY: return "[" + (properties["toString"]->downcast<C_Function>())(std::vector<MS_POINTER> {})->toString() + "]";
            case MS_Types::T_FUNCTION: return "[Function]";
            case MS_Types::C_FUNCTION: return "[C-Function]";
            case MS_Types::T_OBJECT: {
                std::string res = "{";
                if (properties.size()) res += "\n";
                for (std::pair<std::string, MS_POINTER> entry : properties) {
                    if (entry.second->index() == MS_Types::T_OBJECT && entry.second->properties == properties) res += "\"" + entry.first + "\"" + ": " + "[Circular]\n";
                    else res += "\"" + entry.first + "\"" + ": " + entry.second->toString() + "\n";
                };
                res += "}";
                return res;
            }
            default: return "unknown";
        };
    };

    float toNumber() {
        switch(value.index()) {
            case MS_Types::T_NUMBER: return std::get<float>(value);
            case MS_Types::T_STRING: {
                try {
                    return std::stof(std::get<std::string>(value));
                } catch (...) {
                    return 0;
                };
            };
            default: throw std::runtime_error("Type " + typeToString() + " cannot be converted to a number!");
        };
    };

    bool isFalsey() {
        switch(value.index()) {
            case MS_Types::T_NUMBER: return (std::get<float>(value) == 0);
            case MS_Types::T_STRING: return (std::get<std::string>(value) == "");
            case MS_Types::T_BOOL: return (std::get<bool>(value) == false);
            case MS_Types::T_NULL: return true;
            default: return false;
        };
    };


    static MS_POINTER make(float val) {
        return std::make_shared<MS_VALUE>(val);
    };

    static MS_POINTER make(std::string &val) {
        return std::make_shared<MS_VALUE>(val);
    };

    static MS_POINTER make() {
        return std::make_shared<MS_VALUE>();
    };

    static MS_POINTER make(std::vector<MS_POINTER> &arr) {
        return std::make_shared<MS_VALUE>(arr);
    };

    static MS_POINTER make(std::unordered_map<std::string, MS_POINTER> &map) {
        return std::make_shared<MS_VALUE>(map);
    };

    static MS_POINTER make(MS_Function &fn) {
        return std::make_shared<MS_VALUE>(fn);
    }

    static MS_POINTER make(C_Function fn) {
        return std::make_shared<MS_VALUE>(fn);
    }

    static MS_POINTER make(bool val) {
        return std::make_shared<MS_VALUE>(val);
    }; 
    
    static MS_POINTER pass(MS_POINTER &passable) {
        switch (passable->index()) {
            case MS_Types::T_NUMBER: 
            return MS_VALUE::make(passable->downcast<float>());

            case MS_Types::T_BOOL: 
            return MS_VALUE::make(passable->downcast<bool>());

            default:
            return passable;
        }
    }

    MS_POINTER getProperty(MS_POINTER &inside) {
            if (inside->index() == MS_Types::T_NUMBER && value.index() == MS_Types::T_ARRAY) {
            MS_Array arr = downcast<MS_Array>();
            float index = inside->downcast<float>();
            if (index >= arr->entries.size()) {
                auto null = MS_VALUE::make();
                arr->entries.resize(index + 1);
                arr->entries[index] = null;
                return null;
            }
            return arr->entries[index];
        }
        std::string propName = inside->toString();
        if (!this->properties.count(propName)) {
            auto null = MS_VALUE::make();
            this->properties[propName] = null;
            return null;
        }
        return this->properties[propName];
    }


    MS_POINTER getProperty(std::string &propName) {
        if (!this->properties.count(propName)) {
            auto null = MS_VALUE::make();
            this->properties[propName] = null;
            return null;
        }
        return this->properties[propName];
    };

    bool hasProperty(std::string &propName) {
        return this->properties.count(propName);
    };


};


MS_POINTER operator+(MS_POINTER &left, MS_POINTER &right) {
if (left->index() == MS_Types::T_NUMBER && right->index() == MS_Types::T_NUMBER) return MS_VALUE::make(left->downcast<float>() + right->downcast<float>());
else if (left->index() == MS_Types::T_STRING && right->index() == MS_Types::T_STRING) return std::make_shared<MS_VALUE>(left->downcast<std::string>() + right->downcast<std::string>());
else if (left->index() == MS_Types::T_NUMBER && right->index() == MS_Types::T_STRING) return std::make_shared<MS_VALUE>(left->toString() + right->downcast<std::string>());
else if (left->index() == MS_Types::T_STRING && right->index() == MS_Types::T_NUMBER) return std::make_shared<MS_VALUE>(left->downcast<std::string>() + right->toString());
else throw std::runtime_error("Cannot add values of type " + left->typeToString() + " and " + right->typeToString());
};


bool operator==(MS_POINTER &left, MS_POINTER &right) {
    switch(left->index()) {
        case MS_Types::T_NULL: {
            if (right->index() == MS_Types::T_NULL) return true;
            return false;
        };
        case MS_Types::T_NUMBER: {
            if (right->index() == MS_Types::T_NUMBER) return left->downcast<float>() == right->downcast<float>();
            else if (right->index() == MS_Types::T_STRING) return left->downcast<float>() == right->toNumber();
            else throw std::runtime_error("Cannot compare values of type " + left->typeToString() + " and " + right->typeToString());
        };
        case MS_Types::T_STRING: {
            if (right->index() == MS_Types::T_STRING) return left->downcast<std::string>() == right->downcast<std::string>();
            else if (right->index() == MS_Types::T_NUMBER) return left->downcast<std::string>() == right->toString();
             else throw std::runtime_error("Cannot compare values of type " + left->typeToString() + " and " + right->typeToString());
        };
        case MS_Types::T_BOOL: { 
            if (right->index() == MS_Types::T_BOOL) return left->downcast<bool>() == right->downcast<bool>();
            return right->index() != MS_Types::T_NULL;
        };
        default: throw std::runtime_error("Cannot compare values of type " + left->typeToString() + " and " + right->typeToString());
    }
};