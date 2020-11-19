
#include "./Context.h"

template <typename T>
T __downcastValue(const RAW_MS_VALUE &exp) {
    return std::get<T>(exp);
};


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
    str->properties["at"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        if (!params.size()) throw std::runtime_error("Function 'at' requires 1 parameter!");
        float num = params[0]->downcast<float>();
        auto res = std::string(1, val.at(num));
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
    arr->properties["at"] = MS_VALUE::make([val](std::vector<MS_POINTER> params) -> MS_POINTER {
        return val->entries[params.size() ? params[0]->toNumber():0];
    });
}; 