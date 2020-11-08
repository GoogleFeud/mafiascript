
#pragma once
#include "../Environment/Enviourment.h"


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
        default: return true;
    };
};

bool isFalsey(const MS_VALUE &val) {
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
        default: return true;
    };
};


float toNumber(MS_VALUE &a) {
    if (a.index() != MS_Types::T_NUMBER) throw std::runtime_error("Expected a number, instead got " + typeToString(a.index()));
    return downcast<float>(a);
};


MS_VALUE applyOperator(MS_VALUE &a, MS_VALUE &b, BINARY_Ops op) {
    switch(op) {
        case BINARY_Ops::OP_ADDITION: return a + b;
        case BINARY_Ops::OP_SUBTRACTION: return toNumber(a) - toNumber(b);
        case BINARY_Ops::OP_MULTIPLICATION: return toNumber(a) * toNumber(b);
        case BINARY_Ops::OP_DIVISION: return toNumber(a) / toNumber(b);
        case BINARY_Ops::OP_EQUAL: return a == b;
        case BINARY_Ops::OP_NOT_EQUAL: return !(a == b); 
        default: return MS_VALUE { nullptr };
    };
};