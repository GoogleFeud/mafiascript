
#include "./Context.h"


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
        return std::make_shared<MS_VALUE>(std::stof(val));
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
            if (isFirst) {
                res += item->toString();
                isFirst = false;
            }
            else res +=  ", " + item->toString();
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
}