#pragma once
#include "../Types/primitives.h"
#include <stdexcept>

class Enviourment {
    public:
    Enviourment* parent = NULL;
    Enviourment() {};
    Enviourment(Enviourment* parent) {
        this->parent = parent;
    }

    Enviourment* extend() {
        return new Enviourment(this);
    };

    Enviourment* lookup(std::string &key) {
        if (this->values.count(key)) return this;
        if (this->parent != NULL) return this->parent->lookup(key);
        return NULL;
    }

    MS_VALUE get(std::string &key) {
        if (this->values.count(key)) return this->values[key];
        if (this->parent) return this->parent->get(key);
        throw std::runtime_error(key + " is not defined!");
    }

    void define(std::string &key, MS_VALUE &val) {
        if (this->lookup(key)) throw std::runtime_error(key + " is already defined!");
        this->values.insert({key, val});
    };

    void set(std::string &key, MS_VALUE val) {
        Enviourment* env = this->lookup(key);
        if (!env) throw std::runtime_error(key + " is not defined!");
        env->unsafeSet(key, val);
    };

    void unsafeSet(std::string &key, MS_VALUE &val) {
        this->values[key] = val;
    };

    void clear() {
        this->values.clear();
    }

    ~Enviourment() {
        this->values.clear();
        //delete this;
    }

    private:
    std::unordered_map<std::string, MS_VALUE> values; 
};

void deleteEnv(Enviourment* env) {
    delete env;
}