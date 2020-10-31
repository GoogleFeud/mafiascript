
#include <iostream>

#include "./Interpreter/Context/Context.h"

#include <chrono> 
using namespace std::chrono; 


int main() {
    std::string code = "return star;";
    Context ctx = Context();
    std::string star = "star";
    ctx.global->set(star, MS_VALUE { std::make_shared<std::string>("Hello World!") });
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

