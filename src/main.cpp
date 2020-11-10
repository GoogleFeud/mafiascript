
#include <iostream>
#include <fstream>

#include "./Interpreter/Context/Context.h"
#include "./Interpreter/Types/constructor.h"

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
    MS_VALUE val = ctx->global->get(key);
    MS_Function fn = downcast<MS_Function>(val);
    auto start = high_resolution_clock::now();
    std::vector params = {MSTypes::makeNumber(12), MSTypes::makeNumber(15)};
    std::string fnName = "print";
    auto fnVal = MSTypes::makeFunction([](std::vector<MS_VALUE> vals, Enviourment* env) -> MS_VALUE {
        for (MS_VALUE val : vals) {
            switch(val.index()) {
                case MS_Types::T_STRING: std::cout<<*downcast<MS_String>(val)<<std::endl; break;
                case MS_Types::T_NUMBER: std::cout<<downcast<float>(val)<<std::endl; break;
                case MS_Types::T_BOOL: std::cout<<(downcast<bool>(val) ? "true":"false")<<std::endl; break;
                default: std::cout<<"Couldn't print value of type " + typeToString(val.index());
            };
        };
        return MSTypes::makeNull();
    });
    ctx->global->unsafeSet(fnName, fnVal);
    MS_VALUE res = ctx->callFunction(fn, params);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time function took: " << duration.count() << " microseconds!"<<std::endl;;
    MS_String str = downcast<MS_String>(res);
    std::cout<<*str<<std::endl;
    system("pause");
    return 0;
}
