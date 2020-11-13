
#pragma once
#include "../Environment/Enviourment.h"
#include <tgmath.h>



float toNumber(MS_POINTER &a) {
    if (a->index() != MS_Types::T_NUMBER) throw std::runtime_error("Expected a number, instead got " + a->typeToString());
    return a->downcast<float>();
};


MS_POINTER applyOperator(MS_POINTER &a, MS_POINTER &b, BINARY_Ops &op) {
    switch(op) {
        case BINARY_Ops::OP_ADDITION: return a + b;
        case BINARY_Ops::OP_SUBTRACTION: return MS_VALUE::make(toNumber(a) - toNumber(b));
        case BINARY_Ops::OP_MULTIPLICATION: return MS_VALUE::make(toNumber(a) * toNumber(b));
        case BINARY_Ops::OP_DIVISION: return MS_VALUE::make(toNumber(a) / toNumber(b));
        case BINARY_Ops::OP_EQUAL: return MS_VALUE::make(a == b);
        case BINARY_Ops::OP_NOT_EQUAL: return MS_VALUE::make(!(a == b));
        case BINARY_Ops::OP_GREATER_THAN: return MS_VALUE::make(toNumber(a) > toNumber(b));
        case BINARY_Ops::OP_GREATER_OR_EQUAL: return MS_VALUE::make(toNumber(a) >= toNumber(b));
        case BINARY_Ops::OP_LESS_THAN: return MS_VALUE::make(toNumber(a) < toNumber(b));
        case BINARY_Ops::OP_LESS_OR_EQUAL: return MS_VALUE::make(toNumber(a) <= toNumber(b));
        case BINARY_Ops::OP_MODULO: return MS_VALUE::make(std::fmod(toNumber(a), toNumber(b)));
        case BINARY_Ops::OP_PLUS_ASSIGN: {
            auto res = a + b;
            a->set(res);
            return res;
        };
        default: return MS_VALUE::make();
    };
};