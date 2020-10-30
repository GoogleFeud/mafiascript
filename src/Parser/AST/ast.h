#pragma once

#include <variant>

/**
enum AST_Types {
    STRING,
    NUMBER,
    BOOL,
    MS_NULL,
    ARRAY, 
    OBJECT, 
    VAR, 
    ASSIGN, 
    BINARY, 
    IF, 
    LOOP, 
    RETURN, 
    CONTINUE, 
    BLOCK 
}; 
*/

enum AST_Types {
    MS_STRING,
    MS_NUMBER,
    MS_BOOL,
    MS_NULL,
    MS_ARRAY,
    MS_OBJECT,
    MS_VAR,
    MS_BINARY,
    MS_ASSIGN,
    MS_FUNCTION,
    MS_CALL,
    MS_TERNERY,
    MS_IF,
    MS_LOOP,
    MS_RETURN,
    MS_CONTINUE,
    MS_BLOCK
};


class AST_String;
class AST_Number;
class AST_Bool;
class AST_Null;
class AST_Array;
class AST_Object;
class AST_Binary;
class AST_Assign;
class AST_Function;
class AST_Call;
class AST_Ternery;
class AST_Var;
class AST_Block;
class AST_If;
class AST_Loop;
class AST_Return;
class AST_Continue;

using AST_ANY = std::variant<AST_String*, AST_Number*, AST_Bool*, AST_Null*, AST_Array*, AST_Object*, AST_Var*, AST_Binary*, AST_Assign*, AST_Function*, AST_Call*, AST_Ternery*, AST_If*, AST_Loop*, AST_Return*, AST_Continue*, AST_Block*>;
using AST_EXPRESSION = std::variant<AST_String*, AST_Number*, AST_Bool*, AST_Null*, AST_Array*, AST_Object*, AST_Var*, AST_Binary*, AST_Assign*, AST_Function*, AST_Call*, AST_Ternery*>;

template <typename T>
T downcast(AST_EXPRESSION &exp) {
    return std::get<T>(exp);
};

template <typename T>
AST_EXPRESSION anyToExpression(AST_ANY &exp) {
    T val = std::get<T>(exp);
    return AST_EXPRESSION { val };
};

