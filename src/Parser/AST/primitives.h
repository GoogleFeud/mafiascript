#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "ast.h"

class AST_String {
    public:
    std::string value;
    AST_String(std::string& val) : value(val) {};
    AST_String(std::string val) {
        value = val;
    };
};

class AST_Number  {
    public:
    float value;
    AST_Number(float val) {
        this->value = val;
    };
};

class AST_Bool {
    public:
    bool value;
    AST_Bool(bool val) {
        this->value = val;
    }
};


class AST_List {
    public: 
    std::vector<AST_NODE*> entries;
    AST_List() {};
    AST_List(AST_NODE* firstEntry) {
        this->entries.push_back(firstEntry);
    };

    void push(AST_NODE* entry) {
        this->entries.push_back(entry);
    };

    ~AST_List() {
        for (AST_NODE* node : entries) {
            delete node;
        };
    };

};

class AST_PairList {
    public:
    std::unordered_map<AST_NODE*, AST_NODE*> entries;
    AST_PairList() { };
    AST_PairList(AST_NODE* key, AST_NODE* value) {
        this->entries[key] = value;
    };

    void push(AST_NODE* key, AST_NODE* val) {
        this->entries[key] = val;
    };

    ~AST_PairList() {
        for (std::pair<AST_NODE*, AST_NODE*> pair : entries) {
              delete pair.first;
              delete pair.second;
        };
    };

};

class AST_Array {
    public:
    AST_List* list;
    AST_Array(AST_List *list) {
        this->list = list;
    };

    ~AST_Array() {
        delete list;
    };

};

class AST_Object {
    public:
    AST_PairList* entries;
    AST_Object(AST_PairList* list) {
        this->entries = list;
    };

    ~AST_Object() {
        delete entries;
    };

};

class AST_Var {
    public:
    std::string value;
    AST_Var(std::string &val) : value(val) {};
};

class AST_Access_Point {
    public:
    std::string value;
    AST_Access_Point(AST_NODE* val) {
        value = downcast<AST_Var*>(val)->value;
        delete val;
    };
};


class AST_Null {};

