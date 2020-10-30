
#include <iostream>
#include <vector>
#include <variant>
#include "./Parser/AST/statements.h"
#include "./Parser/index.h"

#include <chrono> 
using namespace std::chrono; 


int main() {
    std::string code = "if (10 == 10) 1 + 1;";
    auto start = high_resolution_clock::now(); 
    AST_Block* res = parse(code);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    std::cout<<duration.count()<<" microseconds"<<std::endl;
    std::cout<<res->nodes.size();
    for (AST_NODE* block : res->nodes) {
         if (block->index() == AST_Types::MS_IF) {
             AST_If* bl = downcastDestroy<AST_If*>(block);
             AST_Compare* condition = downcastDestroy<AST_Compare*>(bl->condition);
             AST_Number* left = downcastDestroy<AST_Number*>(condition->left);
             AST_Number* right = downcastDestroy<AST_Number*>(condition->right);
             if (left->value == right->value) std::cout<<"Condition is true!";
             else std::cout<<"Condition is false!"; 
         }
    }
    destroy();
    system("pause");
}

