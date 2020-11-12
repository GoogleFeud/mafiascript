
#include <iostream>
#include <fstream>

#include "./Interpreter/Context/Context.h"

#include <chrono>
using namespace std::chrono;


Context* eval() {
    std::ifstream ifs("script.ms");
    std::string code( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
    Context* ctx = new Context();
    //ctx->settings.loopTimeout = 5000;
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
    std::string key = "fn";
  MS_POINTER fn = ctx->global->get(key);
  auto start = high_resolution_clock::now();
    std::vector params = {MS_VALUE::make(5.f), MS_VALUE::make(15.f)};
    std::string fnName = "print";
    auto fnVal = MS_VALUE::make([&](std::vector<MS_POINTER> vals, Enviourment* env) -> MS_POINTER {
        for (MS_POINTER val : vals) {
            std::cout<<val->toString()<<std::endl;
        };
        return MS_VALUE::make();
    });
    ctx->global->unsafeSet(fnName, fnVal);
    MS_POINTER res = ctx->callFunction(fn, params);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time function took: " << duration.count() << " microseconds!"<<std::endl;;
    std::cout<<res->toString()<<std::endl;  
    std::cout<<fn.use_count()<<std::endl;
    system("pause");
    return 0;
}
