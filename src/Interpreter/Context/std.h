
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
};

void __initArray(MS_VALUE *arr) {
    MS_Array* val = arr->downcast<MS_Array*>();
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
        val->entries.pop_back();
        return MS_VALUE::make();
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
        auto res = std::vector<MS_POINTER>(val->entries.begin() + start, val->entries.begin() + start + len);
        return MS_VALUE::make(res);
    });
    arr->properties["at"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return val->entries[params.size() ? params[0]->toNumber():0];
    });
}; 