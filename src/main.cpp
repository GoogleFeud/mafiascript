
#include <iostream>
#include <fstream>

#include "./Interpreter/Context/std.h"

#include <chrono>
using namespace std::chrono;


Context* eval() {
    std::ifstream ifs("script.ms");
    std::string code( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
    Context* ctx = new Context();
    std::string fnName = "print";
    auto fnVal = MS_VALUE::make([&](std::vector<MS_POINTER> vals) -> MS_POINTER {
        for (MS_POINTER val : vals) {
            std::cout<<val->toString() + "\n";
        };
        return MS_VALUE::make();
    });
    fnVal->isConst = true;
    ctx->global->unsafeSet(fnName, fnVal);
    auto start = high_resolution_clock::now();
    ctx->run(code);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time it took: " << duration.count() << " microseconds!"<<std::endl;
    return ctx;
};


int main()
{

    Context* ctx = eval();
    std::string key = "main";
  MS_POINTER fn = ctx->global->get(key);
  auto start = high_resolution_clock::now();
    std::vector params = {MS_VALUE::make(1.f), MS_VALUE::make(15.f)};
    MS_POINTER res = ctx->callFunction(fn, params);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time function took: " << duration.count() << " microseconds!"<<std::endl;;
    std::cout<<res->toString()<<std::endl;  
    //ctx->global->clear();
    system("pause");
    return 0;
}
