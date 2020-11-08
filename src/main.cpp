
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
    ctx->settings.loopTimeout = 0;
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
    MS_VALUE res = ctx->callFunction(fn, std::vector {MSTypes::makeNumber(12), MSTypes::makeNumber(15)});
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time function took: " << duration.count() << " microseconds!"<<std::endl;
    MS_String str = downcast<MS_String>(res);
    std::cout<<*str<<std::endl;
    std::cout<<str.use_count();
    system("pause");
    return 0;
}
