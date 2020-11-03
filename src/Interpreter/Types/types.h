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
    T_FUNCTION
};


class _MS_Array;
class _MS_Object;
class _MS_Function;

using MS_String = std::shared_ptr<std::string>;
using MS_Array = std::shared_ptr<_MS_Array>;
using MS_Object = std::shared_ptr<_MS_Object>;
using MS_Function = std::shared_ptr<_MS_Function>;

using MS_VALUE = std::variant<MS_String, float, bool, std::nullptr_t, MS_Array, MS_Object, MS_Function>; 

