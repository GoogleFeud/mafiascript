
#include "../Types/primitives.h"

class Enviourment {
    public:
    Enviourment* parent;
    Enviourment() {};
    Enviourment(Enviourment* parent) {
        this->parent = parent;
    }

    Enviourment* extend() {
        return new Enviourment(this);
    };

    MS_VALUE get(std::string &key) {
        if (this->values.count(key)) return this->values[key];
        if (this->parent) return this->parent->get(key);
        throw key + " is not defined!";
    }

    bool has(std::string &key) {
        if (this->values.count(key)) true;
        if (this->parent) return this->parent->has(key);
        return false;
    }

    void define(std::string &key, MS_VALUE val) {
        if (this->has(key)) throw key + "is already defined!";
        this->values.insert({key, val});
    };

    void set(std::string &key, MS_VALUE val) {
        if (!this->has(key)) throw key + " is not defined!";
        this->values.insert_or_assign(key, val);
    };

    void clear() {
        this->values.clear();
    }


    private:
    std::unordered_map<std::string, MS_VALUE> values; 
};