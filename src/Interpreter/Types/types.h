#pragma once
#include <variant>
#include <memory>
#include <vector>


enum MS_Types {
    T_STRING,
    T_NUMBER,
    T_BOOL,
    T_NULL,
    T_ARRAY,
    T_OBJECT,
    T_FUNCTION,
    C_FUNCTION
};


class _MS_Array;
class _MS_Object;
class _MS_Function;

using MS_String = std::shared_ptr<std::string>;
using MS_Array = std::shared_ptr<_MS_Array>;
using MS_Object = std::shared_ptr<_MS_Object>;
using MS_Function = std::shared_ptr<_MS_Function>;
using C_Function = void (*)(std::vector<std::variant<MS_String, float, bool, std::nullptr_t, MS_Array, MS_Object, MS_Function>>);

using MS_VALUE = std::variant<MS_String, float, bool, std::nullptr_t, MS_Array, MS_Object, MS_Function, C_Function>; 

std::string typeToString(int type) {
    switch(type) {
        case MS_Types::T_STRING: return "string";
        case MS_Types::T_NUMBER: return "number";
        case MS_Types::T_BOOL: return "boolean";
        case MS_Types::T_NULL: return "null";
        case MS_Types::T_ARRAY: return "array";
        case MS_Types::T_OBJECT: return "object";
        case MS_Types::T_FUNCTION: return "function";
        case MS_Types::C_FUNCTION: return "c-function";
        default: return "";
    };
}

template <typename T>
T downcast(MS_VALUE &exp) {
    //std::cout<<"Downcast with val of type: "<<exp.index()<<std::endl;
    return std::get<T>(exp);
};

template <typename T>
T downcast(MS_VALUE *exp) {
    return std::get<T>(*exp);
};

template <typename T>
T downcast(const MS_VALUE &exp) {
    //std::cout<<"Downcast with val of type: "<<exp.index()<<std::endl;
    return std::get<T>(exp);
};

template <typename T>
T downcast(const MS_VALUE *exp) {
    return std::get<T>(*exp);
};

float stringToFloat(MS_VALUE &str) {
    try {
        return std::stof(*downcast<MS_String>(str));
    } catch(...) {
        return -1;
    };
};


MS_VALUE operator+(MS_VALUE &left, MS_VALUE &right) {
    if (left.index() == MS_Types::T_NUMBER && right.index() == MS_Types::T_NUMBER) return downcast<float>(left) + downcast<float>(right);
    else if (left.index() == MS_Types::T_STRING && right.index() == MS_Types::T_STRING) return std::make_shared<std::string>(*downcast<MS_String>(left) + *downcast<MS_String>(right));
    else if (left.index() == MS_Types::T_NUMBER && right.index() == MS_Types::T_STRING) return std::make_shared<std::string>(std::to_string(downcast<float>(left)) + *downcast<MS_String>(right));
    else if (left.index() == MS_Types::T_STRING && right.index() == MS_Types::T_NUMBER) return std::make_shared<std::string>(*downcast<MS_String>(left) + std::to_string(downcast<float>(right)));
    else throw std::runtime_error("Cannot add values of type " + typeToString(left.index()) + " and " + typeToString(right.index()));
};

bool operator==(MS_VALUE &left, MS_VALUE &right) {
    switch(left.index()) {
        case MS_Types::T_NULL: {
            if (right.index() == MS_Types::T_NULL) return true;
            return false;
        };
        case MS_Types::T_NUMBER: {
            if (right.index() == MS_Types::T_NUMBER) return downcast<float>(left) == downcast<float>(right);
            else if (right.index() == MS_Types::T_STRING) return downcast<float>(left) == stringToFloat(right);
            else throw std::runtime_error("Cannot compare values of type " + typeToString(left.index()) + " and " + typeToString(right.index()));
        };
        case MS_Types::T_STRING: {
            if (right.index() == MS_Types::T_STRING) return downcast<MS_String>(left) == downcast<MS_String>(right);
            else if (right.index() == MS_Types::T_NUMBER) return stringToFloat(right) == downcast<float>(right);
            else throw std::runtime_error("Cannot compare values of type " + typeToString(left.index()) + " and " + typeToString(right.index()));
        };
        case MS_Types::T_BOOL: { 
            if (right.index() == MS_Types::T_BOOL) return downcast<bool>(left) == downcast<bool>(right);
            return right.index() != MS_Types::T_NULL;
        };
        default: throw std::runtime_error("Cannot compare values of type " + typeToString(left.index()) + " and " + typeToString(right.index()));
    }
};