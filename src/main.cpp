
#include <iostream>

#include "./Interpreter/Context/Context.h"

#include <chrono> 
using namespace std::chrono; 


int main() {
    std::string code = "if (10 == 10) {return 50;};";
    Context ctx;
    auto start = high_resolution_clock::now(); 
    MS_VALUE mafiascriptRes = ctx.run(code);
   // std::cout<<mafiascriptRes.index();
    float res = downcast<float>(mafiascriptRes);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    std::cout<<"Result: "<<res<<std::endl;
    std::cout<<"Time it took: "<<duration.count()<<" microseconds!";
    system("pause");
}

