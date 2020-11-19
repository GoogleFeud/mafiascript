#pragma once

#include <variant>


enum AST_Types {
    MS_STRING,
    MS_NUMBER,
    MS_BOOL,
    MS_NULL,
    MS_ARRAY,
    MS_OBJECT,
    MS_VAR,
    MS_BINARY,
    MS_UNARY,
    MS_AND,
    MS_OR,
    MS_ASSIGN,
    MS_FUNCTION,
    MS_CALL,
    MS_TERNERY,
    MS_ACCESSOR,
    MS_TYPEOF,
    MS_DECLARE,
    MS_IF,
    MS_LOOP,
    MS_RETURN,
    MS_CONTINUE,
    MS_BLOCK,
    MS_ACCESS_POINT
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
class AST_Declare;
class AST_And;
class AST_Or;
class AST_Typeof;
class AST_Accessor;
class AST_Access_Point;
class AST_Unary;

using AST_NODE = std::variant<AST_String*, AST_Number*, AST_Bool*, AST_Null*, AST_Array*, AST_Object*, AST_Var*, AST_Binary*, AST_Unary*, AST_And*, AST_Or*, AST_Assign*, AST_Function*, AST_Call*, AST_Ternery*, AST_Accessor*, AST_Typeof*, AST_Declare*, AST_If*, AST_Loop*, AST_Return*, AST_Continue*, AST_Block*, AST_Access_Point*>;

template <typename T>
T downcast(AST_NODE &exp) {
    return std::get<T>(exp);
};

template <typename T>
T downcast(AST_NODE *exp) {
    return std::get<T>(*exp);
};
