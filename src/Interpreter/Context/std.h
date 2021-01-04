
#include "./Context.h"
#include "unordered_map"

void __initString(MS_VALUE *str) {
    const std::string val = str->downcast<std::string>();
    str->properties["size"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make((float)val.size());
    });
    str->properties["startsWith"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        if (!params.size()) throw std::runtime_error("Function 'startsWith' requires 1 parameter!");
        std::string prefix = params[0]->toString();
        return MS_VALUE::make(val.rfind(prefix, 0) == 0);
    });
    str->properties["endsWith"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        if (!params.size()) throw std::runtime_error("Function 'startsWith' requires 1 parameter!");
        std::string prefix = params[0]->toString();
        return MS_VALUE::make(val.rfind(prefix) == val.size() - 1);
    });
    str->properties["includes"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        if (!params.size()) throw std::runtime_error("Function 'startsWith' requires 1 parameter!");
        std::string toBeFound = params[0]->downcast<std::string>();
        return MS_VALUE::make(val.find(toBeFound) != std::string::npos);
    });
    str->properties["at"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        if (!params.size()) throw std::runtime_error("Function 'at' requires 1 parameter!");
        float num = params[0]->downcast<float>();
        auto res = std::string(1, val.at(num));
        return MS_VALUE::make(res);
    });
    str->properties["split"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::vector<MS_POINTER> res;
        auto start = 0U;
        std::string delimiter;
        if (params.size() && !params[0]->isFalsey()) delimiter = params[0]->downcast<std::string>();
        else delimiter = " ";
        auto end = val.find(delimiter); 
        while (end != std::string::npos) {
        auto f = val.substr(start, end - start);
        res.push_back(MS_VALUE::make(f));
        start = end + delimiter.length();
        end = val.find(delimiter, start);
    };
    auto fin = val.substr(start, end);
    res.push_back(MS_VALUE::make(fin));
    return MS_VALUE::make(res);
    });
    str->properties["toLowerCase"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::string copy = val;
        std::transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char ch) { return std::tolower(ch); });
        return MS_VALUE::make(copy);
    });
    str->properties["toUpperCase"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::string copy = val;
        std::transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char ch) { return std::toupper(ch); });
        return MS_VALUE::make(copy);
    });
    str->properties["slice"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        float start = params.size() ? params[0]->downcast<float>():0;
        float len = params.size() == 2 ? params[1]->downcast<float>():val.size();
        auto res = val.substr(start, len);
        return MS_VALUE::make(res);
    });
    str->properties["toString"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return std::make_shared<MS_VALUE>(val);
    });
    str->properties["toNumber"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        try {
            return std::make_shared<MS_VALUE>(std::stof(val));
        } catch(...) {
            return MS_VALUE::make();
        }
    });
};

void __initArray(MS_VALUE *arr) {
    MS_Array val = arr->downcast<MS_Array>();
    arr->properties["size"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make((float)val->entries.size());
    });
    arr->properties["push"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        for (MS_POINTER param : params) {
            val->entries.push_back(param);
        };
        return MS_VALUE::make((float)val->entries.size());
    });
    arr->properties["pop"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto lastEl = val->entries[val->entries.size() - 1];
        val->entries.pop_back();
        return lastEl;
    });
    arr->properties["join"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::string joinedBy = params.size() ? params[0]->toString():"";
        std::string res;
        bool isFirst = true;
        for (MS_POINTER item : val->entries) {
            if (isFirst) {
                res += item->toString();
                isFirst = false;
            }
            else res += joinedBy + item->toString();
        };
        return MS_VALUE::make(res);
    });
    arr->properties["toString"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::string res = "";
        bool isFirst = true;
        for (MS_POINTER item : val->entries) {
            std::string str;
            if (item->index() == MS_Types::T_ARRAY && item->downcast<MS_Array>() == val) str = "[Circular]";
            else str = item->toString();
            if (isFirst) {
                res += str;
                isFirst = false;
            }
            else res +=  ", " + str;
        };
        return MS_VALUE::make(res);
    });
    arr->properties["slice"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        float start = params.size() ? params[0]->downcast<float>():0;
        float len = params.size() == 2 ? params[1]->downcast<float>():val->entries.size();
        if (start > val->entries.size() || start > len) start = 0;
        if (len > val->entries.size()) len = val->entries.size();
        auto res = std::vector<MS_POINTER>(val->entries.begin() + start, val->entries.begin() + len);
        return MS_VALUE::make(res);
    });
    arr->properties["delete"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        float start = params.size() ? params[0]->downcast<float>():0;
        if (params.size() < 2) val->entries.erase(val->entries.begin() + start);
        else {
        float end = params.size() == 2 ? params[1]->downcast<float>():1;
        val->entries.erase(val->entries.begin() + start, val->entries.begin() + end);
        };
        return MS_VALUE::make();
    });
    arr->properties["fill"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        if (!params.size()) throw std::runtime_error("Fill function requires at least one parameter!");
        MS_POINTER fillValue = params[0];
        float fillLength = params[1] ? params[1]->downcast<float>():1.f;
        val->entries.resize(fillLength);
        std::fill(val->entries.begin(), val->entries.end(), fillValue);
        return MS_VALUE::make(val->entries);
    });
    arr->properties["filter"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::vector<MS_POINTER> res;
        if (!params[0] || params[0]->index() != MS_Types::T_FUNCTION) throw std::runtime_error("Filter function accepts only a function!");
        MS_Function fn = params[0]->downcast<MS_Function>();
        auto valSize = val->entries.size();
        for (int i=0; i < valSize; i++) {
            std::vector param = std::vector {val->entries[i], MS_VALUE::make((float)i)};
            if (!_callFunction(fn->ctx, fn, param)->isFalsey()) res.push_back(val->entries[i]);
        };
        return MS_VALUE::make(res);
    });
    arr->properties["map"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::vector<MS_POINTER> res;
        if (!params[0] || params[0]->index() != MS_Types::T_FUNCTION) throw std::runtime_error("Map function accepts only a function!");
        MS_Function fn = params[0]->downcast<MS_Function>();
        auto valSize = val->entries.size();
        for (int i =0; i < valSize; i++) {
            std::vector param = std::vector {val->entries[i], MS_VALUE::make((float)i)};
            res.push_back(_callFunction(fn->ctx, fn, param));
        };
        return MS_VALUE::make(res);
    });
    arr->properties["reduce"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::vector<MS_POINTER> res;
        if (!params[0] || params[0]->index() != MS_Types::T_FUNCTION) throw std::runtime_error("Reduce function accepts only a function!");
        MS_Function fn = params[0]->downcast<MS_Function>();
        MS_POINTER accumulator = params[1] ? params[1]:MS_VALUE::make(0.f);
        for (MS_POINTER value : val->entries) {
            std::vector param = std::vector {accumulator, value};
            accumulator = _callFunction(fn->ctx, fn, param);
        };
        return accumulator;
    });
    arr->properties["some"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        if (!params[0] || params[0]->index() != MS_Types::T_FUNCTION) throw std::runtime_error("Reduce function accepts only a function!");
        MS_Function fn = params[0]->downcast<MS_Function>();
        for (MS_POINTER value : val->entries) {
            std::vector param = std::vector {value};
            if (!_callFunction(fn->ctx, fn, param)->isFalsey()) return MS_VALUE::make(true);
        };
        return MS_VALUE::make(false);
    });
    arr->properties["every"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        if (!params[0] || params[0]->index() != MS_Types::T_FUNCTION) throw std::runtime_error("Reduce function accepts only a function!");
        MS_Function fn = params[0]->downcast<MS_Function>();
        for (MS_POINTER value : val->entries) {
            std::vector param = std::vector {value};
            if (_callFunction(fn->ctx, fn, param)->isFalsey()) return MS_VALUE::make(false);
        };
        return MS_VALUE::make(true);
    });
    arr->properties["at"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return val->entries[params.size() ? params[0]->toNumber():0];
    });
    arr->properties["insert"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto toAdd = params[0] ? params[0]:MS_VALUE::make();
        val->entries.insert(val->entries.begin(), toAdd);
        return toAdd;
    });
    arr->properties["each"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto fnObj = params[0];
        if (!fnObj || fnObj->index() != MS_Types::T_FUNCTION) throw std::runtime_error("Each function accepts only a function!");
        auto fn = fnObj->downcast<MS_Function>();
        for (MS_POINTER value : val->entries) {
            std::vector param = std::vector {value};
            _callFunction(fn->ctx, fn, param);
        }
        return MS_VALUE::make();
    });
}; 

void __initNumber(MS_VALUE* num) {
    float val = num->downcast<float>();
    num->properties["toString"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto str = std::to_string(val);
        return MS_VALUE::make(str);
    });
    num->properties["abs"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make(std::abs(val));
    });
    num->properties["floor"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make(std::floor(val));
    });
    num->properties["ceil"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make(std::ceil(val));
    });
    num->properties["log"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make(std::log(val));
    });
    num->properties["log10"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make(std::log10(val));
    });
    num->properties["exp"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make(std::exp(val));
    });
    num->properties["pow"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto powerOf = params[0] && params[0]->index() == MS_Types::T_NUMBER ? params[0]->downcast<float>():1;
        return MS_VALUE::make(std::pow(val, powerOf));
    });
}

void __initBoolean(MS_VALUE* boolean) {
    bool val = boolean->downcast<bool>();
    boolean->properties["toString"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return MS_VALUE::make(val ? "true":"false");
    });
}

void __initFunc(MS_VALUE* func) {
    MS_Function fn = func->downcast<MS_Function>();
    func->properties["params"] = MS_VALUE::make([fn](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::vector<MS_POINTER> fnParams;
        for (AST_Var* param : fn->params) {
            fnParams.push_back(MS_VALUE::make(param->value));
        }
        return MS_VALUE::make(fnParams);
    }); 
    func->properties["bind"] = MS_VALUE::make([fn](std::vector<MS_POINTER> params) -> MS_POINTER {
        MS_Function copied = std::make_shared<_MS_Function>(fn->body, fn->params, fn->scope->extend(), fn->ctx);
        for (int i=0; i < params.size(); i++) {
            auto paramName = copied->params[i];
            copied->params.erase(copied->params.begin() + i);
            copied->scope->unsafeSet(paramName->value, params[i]);
        };
        return MS_VALUE::make(copied);
    }); 
}

void __initGlobal(Enviourment* env) {

    // Array 
    MS_POINTER arr = MS_VALUE::make([&](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::vector<MS_POINTER> arr;
        return MS_VALUE::make(arr);
    }); 

    arr->properties["from"] = MS_VALUE::make([env](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto param = params[0];
        if (!param) {
            std::vector<MS_POINTER> arr;
            return MS_VALUE::make(arr); 
        };
        switch (param->index()) {
            case MS_Types::T_NULL: {
                std::vector<MS_POINTER> arr;
                return MS_VALUE::make(arr); 
            };
            case MS_Types::T_NUMBER: {
                std::vector<MS_POINTER> arr;
                float range = param->downcast<float>();
                for (int i=0; i < range; i++) {
                    arr.push_back(MS_VALUE::make((float)i));
                }
                return MS_VALUE::make(arr);
            };
            case MS_Types::T_STRING: {
                std::vector<MS_POINTER> arr;
                std::string str = param->downcast<std::string>();
                for (char& c : str) {
                    std::string charStr(1, c);
                    arr.push_back(MS_VALUE::make(charStr));
                }
                return MS_VALUE::make(arr);
            }
            case MS_Types::T_ARRAY: {
                std::vector<MS_POINTER> arr(param->downcast<MS_Array>()->entries);
                return MS_VALUE::make(arr);
            }

            case MS_Types::C_FUNCTION:
            case MS_Types::T_FUNCTION:
            throw std::runtime_error("Cannot pass function to Array.from");

            default: {
                std::vector<MS_POINTER> arr;
                for (std::pair<std::string, MS_POINTER> entry : param->properties) {
                    arr.push_back(entry.second);
                }
                return MS_VALUE::make(arr);
            }
        }
    }); 

    env->unsafeSet(std::string{"Array"}, arr);

    // Object

    MS_POINTER object = MS_VALUE::make([&](std::vector<MS_POINTER> params) -> MS_POINTER {
        std::unordered_map<std::string, MS_POINTER> obj;
        return MS_VALUE::make(obj);
    }); 

    object->properties["entries"] = MS_VALUE::make([object](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto obj = params[0];
        if (!obj || obj->index() != MS_Types::T_OBJECT) throw std::runtime_error("Must pass an object to Object.entries");
        std::vector<MS_POINTER> entries;
        for (std::pair<std::string, MS_POINTER> entry : obj->properties) {
            std::vector<MS_POINTER> entryArr {MS_VALUE::make(entry.first), entry.second};
            entries.push_back(MS_VALUE::make(entryArr));
        }
        return MS_VALUE::make(entries);
    }); 

    object->properties["freeze"] = MS_VALUE::make([object](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto obj = params[0];
        if (!obj || obj->index() != MS_Types::T_OBJECT) throw std::runtime_error("Must pass an object to Object.copy");
        for (std::pair<std::string, MS_POINTER> entry : obj->properties) {
            entry.second->flags.isConst = true;
        }
        return MS_VALUE::make();
    }); 

    env->unsafeSet(std::string{"Object"}, object);

    // NaN

    MS_POINTER NaN = MS_VALUE::make([&](std::vector<MS_POINTER> params) -> MS_POINTER {
        auto param = params[0];
        if (!param || param->index() != MS_Types::T_STRING) throw std::runtime_error("Must pass string to NaN");
        try {
            std::make_shared<MS_VALUE>(std::stof(param->downcast<std::string>()));
        } catch(...) {
            return MS_VALUE::make(true);
        }
        return MS_VALUE::make(false);
    }); 

    env->unsafeSet(std::string{"isNaN"}, NaN);
}