
#pragma once
#include "./types.h"
#include <unordered_map>

namespace MSTypes {

    MS_VALUE makeString(std::string &str) {
        return MS_VALUE { std::make_shared<std::string>(str) };
    };

    MS_VALUE makeString(std::string str) {
        return MS_VALUE { std::make_shared<std::string>(str) };
    }; 

    MS_VALUE makeBool(bool boolean) {
        return MS_VALUE {boolean};
    }

    MS_VALUE makeNumber(float num) {
        return MS_VALUE {num};
    };

    MS_VALUE makeNull() {
        return MS_VALUE {nullptr};
    }; 

    MS_VALUE makeArray(std::vector<MS_VALUE> &vec) {
        return MS_VALUE { std::make_shared<_MS_Array>(vec) };
    };


    MS_VALUE makeObject(std::unordered_map<std::string, MS_VALUE> &obj) {
        return MS_VALUE { std::make_shared<_MS_Object>(obj) };
    }


    MS_VALUE makeFunction(MS_VALUE (*fn)(std::vector<MS_VALUE>, Enviourment* env)) {
        return MS_VALUE { std::make_shared<_C_Function>(fn) };
    };

};