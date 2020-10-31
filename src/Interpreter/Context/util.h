
#pragma once
#include "../Environment/Enviourment.h"


template <typename T>
T downcast(MS_VALUE &exp) {
    return std::get<T>(exp);
};

template <typename T>
T downcast(MS_VALUE *exp) {
    return std::get<T>(*exp);
};


bool isFalsey(MS_VALUE &val) {
    switch(val.index()) {
        case MS_Types::T_NULL: return true;
        case MS_Types::T_ARRAY:
        case MS_Types::T_OBJECT:
        return false;
        case MS_Types::T_BOOL: {
           return !downcast<bool>(val);
        };
        case MS_Types::T_STRING: {
            MS_String str = downcast<MS_String>(val);
            if (!str->compare("")) return true;
        };
        case MS_Types::T_NUMBER: {
            float num = downcast<float>(val);
            if (num == 0) return true;
        };
    };
};

float toNumber(MS_VALUE &a) {
    if (a.index() != MS_Types::T_NUMBER) throw "Expected a number";
    return downcast<float>(a);
};

MS_VALUE applyOperator(MS_VALUE &a, MS_VALUE &b, std::string &op) {
    if (op == "+") return toNumber(a) + toNumber(b);
    else if (op == "-") return toNumber(a) - toNumber(b);
    else if (op == "*") return toNumber(a) * toNumber(b);
    else if (op == "/") return toNumber(a) / toNumber(b);
    else if (op == "==") return toNumber(a) == toNumber(b);
};