#pragma once
#include "../Types/types.h"
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

    MS_POINTER get(std::string &key) {
        if (this->values.count(key)) return this->values[key];
        if (this->parent) return this->parent->get(key);
        throw std::runtime_error(key + " is not defined!");
    }

    void define(std::string &key, MS_POINTER &val) {
        if (this->lookup(key)) throw std::runtime_error(key + " is already defined!");
        this->values[key] = val;
    };

    void set(std::string &key, MS_POINTER val) {
        Enviourment* env = this->lookup(key);
        if (!env) throw std::runtime_error(key + " is not defined!");
        env->unsafeSet(key, val);
    };

    void unsafeSet(std::string key, MS_POINTER &val) {
        this->values[key] = val;
    };

    void clear() {
        this->values.clear();
    }
    
    size_t size() {
        return this->values.size();
    }

    ~Enviourment() {
        this->values.clear();
    }

    private:
    std::unordered_map<std::string, MS_POINTER> values; 
};

void deleteEnv(Enviourment* env) {
    delete env;
}