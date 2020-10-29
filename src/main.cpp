
#include <iostream>
#include <vector>
#include <variant>
#include "./Parser/AST/primitives.h"
#include "./Parser/AST/expressions.h"


float calc(float left, float right, std::string& op) {
    if (op == "+") return left + right;
    else if (op == "-") return left - right;
    else if (op == "/") return left / right;
    else if (op == "*") return left / right;
};

int main() {
  /**  std::string string = "Hello world!";
    std::vector<AST_Node*> vec;
    AST_String* str = new AST_String(string);
    AST_Number* num = new AST_Number(3.14);
    vec.push_back(str);
    vec.push_back(num);
    for (AST_Node* node : vec) {
        if (node->getType() == AST_Types::STRING) {
            AST_String* typecast = (AST_String*) node;
            std::cout<<typecast->getValue();
        } else if (node->getType() == AST_Types::NUMBER) {
            AST_Number* typecast = (AST_Number*) node;
            std::cout<<typecast->getValue();
        };
    } **/
    std::string string = "Hello world!";
    std::string op = "+";
    AST_Array* arr = new AST_Array();
    arr->push(AST_EXPRESSION { new AST_String(string) });
    arr->push(AST_EXPRESSION { new AST_Number(112) });
    arr->push(AST_EXPRESSION { new AST_Bool(true) });
    AST_EXPRESSION someNum = AST_EXPRESSION { new AST_Number(5) };
    arr->push(
        AST_EXPRESSION {
            new AST_Binary(
                someNum,
                AST_EXPRESSION { new AST_Number(10) },
                op
            )
        }
    );
    for (AST_EXPRESSION element : arr->entries) {
        switch(element.index()) {
            case AST_Types::STRING: {
                AST_String* el = downcast<AST_String*>(element);
                std::cout<<"String: " + el->value<<std::endl; 
                break;
            };
            case AST_Types::NUMBER: {
                AST_Number* el = downcast<AST_Number*>(element);
                std::cout<<"Number: " + std::to_string(el->value)<<std::endl;
                break;
            };
            case AST_Types::BOOL: {
                AST_Bool* el = downcast<AST_Bool*>(element);
                std::cout<<"Bool: " + std::to_string(el->value)<<std::endl;
                break;
            }
            case AST_Types::BINARY: {
                AST_Binary* el = downcast<AST_Binary*>(element);
                AST_Number* left = downcast<AST_Number*>(el->left);
                AST_Number* right = downcast<AST_Number*>(el->right);
                std::cout<<"Binary Operation: " + std::to_string(calc(left->value, right->value, el->op))<<std::endl;
            }
        }
    }
    //arr->push(new AST_Ternery(new AST_Bool(true), new AST_Null, new AST_Null));
    system("pause");
}

