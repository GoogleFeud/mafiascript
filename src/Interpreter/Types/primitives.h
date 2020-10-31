#include "./types.h"
#include <unordered_map>

class _MS_Array {
    public:
    std::vector<MS_VALUE> entries;
    _MS_Array(std::vector<MS_VALUE> &values) : entries(values) {};
};

class _MS_Object {
    public:
    std::unordered_map<std::string, MS_VALUE> entries;
    _MS_Object(std::unordered_map<std::string, MS_VALUE> &values) : entries(values) {};
};