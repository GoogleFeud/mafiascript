
#include <iostream>
#include <vector>
#include <variant>
#include "./Parser/AST/statements.h"

#include <chrono> 
using namespace std::chrono; 

extern AST_Block* parse(FILE* file);

int main() {
    FILE *myfile = fopen("script.ms", "r");
    auto start = high_resolution_clock::now(); 
    AST_Block* res = parse(myfile);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    std::cout<<duration.count()<<" microseconds"<<std::endl;
    std::cout<<res->nodes.size();
    for (AST_NODE* block : res->nodes) {
         if (block->index() == AST_Types::MS_IF) {
             AST_If* bl = downcastDestroy<AST_If*>(block);
             AST_Compare* condition = downcastDestroy<AST_Compare*>(bl->condition);
             AST_String* left = downcastDestroy<AST_String*>(condition->left);
             AST_String* right = downcastDestroy<AST_String*>(condition->right);
             if (left->value == right->value) std::cout<<"Condition is true!";
             else std::cout<<"Condition is false!"; 
         }
    }
    system("pause");
}

