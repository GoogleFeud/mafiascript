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
    C_FUNCTION
};
;

class MS_VALUE;
using MS_POINTER = std::shared_ptr<MS_VALUE>;
class Enviourment;
void deleteEnv(Enviourment* env);

class MS_Function {
    public:
    Enviourment* scope;
    std::vector<std::string> params;
    AST_NODE* body;

    MS_Function(AST_NODE* bod, std::vector<std::string> &params, Enviourment* env) : params(params) {
        body = bod;
        scope = env;
    };

    ~MS_Function() {
        deleteEnv(scope);
        delete body;
    };

}; 


class MS_Array {
    public:
    std::vector<MS_POINTER> entries;
    MS_Array(std::vector<MS_POINTER> &values) : entries(values) {
    };
    MS_Array() {};
};



using C_Function = std::function<MS_POINTER(std::vector<MS_POINTER>, Enviourment*)>;
using RAW_MS_VALUE = std::variant<std::string, float, bool, std::nullptr_t, MS_Array*, MS_Function*, C_Function>;

class MS_VALUE {
    public:
    RAW_MS_VALUE value;
    std::unordered_map<std::string, MS_POINTER> properties;

    MS_VALUE(const std::string &str) {
        value = str;
    };
    MS_VALUE(float num) {
        value = num;
    };

    MS_VALUE(bool boolean) {
        value = boolean;
    }; 

    MS_VALUE() {
        value = nullptr;
    };

    MS_VALUE(std::vector<MS_POINTER> &arr) {
        value = new MS_Array(arr);
    };

    MS_VALUE(MS_Function *fn) {
        value = fn;
    }

    MS_VALUE(C_Function fn) {
        value = fn;
    }

    MS_VALUE(std::unordered_map<std::string, MS_POINTER> &map) {
        properties = map;
    };
    MS_VALUE(MS_VALUE (*fn)(std::vector<MS_VALUE>, Enviourment* env)) {
        value = fn;
    };

    size_t index() {
        return value.index();
    };

    void set(MS_POINTER &val) {
        value = val->value;
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
            default: throw std::runtime_error("Type " + typeToString() + " cannot be converted to a string!");
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


    static MS_POINTER make(bool val) {
        return std::make_shared<MS_VALUE>(val);
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

    static MS_POINTER make(MS_Function *fn) {
        return std::make_shared<MS_VALUE>(fn);
    }

    static MS_POINTER make(C_Function fn) {
        return std::make_shared<MS_VALUE>(fn);
    }

    MS_POINTER operator[](MS_POINTER &inside) {
        if (inside->index() == MS_Types::T_NUMBER && value.index() == MS_Types::T_ARRAY) {
            MS_Array* arr = downcast<MS_Array*>();
            float index =  inside->downcast<float>();
            if (index > arr->entries.size()) {
                auto null = MS_VALUE::make();
                arr->entries[index] = null;
                return null;
            }
            return arr->entries[index];
        }
        std::string propName = inside->toString();
        if (!this->properties.count(propName)) return MS_VALUE::make();
        return this->properties[propName];
    };

    MS_POINTER operator[](std::string &propName) {
        if (!this->properties.count(propName)) return MS_VALUE::make();
        return this->properties[propName];
    };

    bool hasProperty(std::string &propName) {
        return this->properties.count(propName);
    };

};


MS_POINTER operator+(MS_POINTER &left, MS_POINTER &right) {
if (left->index() == MS_Types::T_NUMBER && right->index() == MS_Types::T_NUMBER) return std::make_shared<MS_VALUE>(left->downcast<float>() + right->downcast<float>());
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
