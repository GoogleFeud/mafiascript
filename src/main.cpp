
#include <iostream>
#include <fstream>

#include "./Interpreter/Context/Context.h"

#include <chrono>
using namespace std::chrono;


Context* eval() {
std::ifstream ifs("script.ms");
    std::string code( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
    Context* ctx = new Context();
    auto start = high_resolution_clock::now();
    ctx->run(code);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time it took: " << duration.count() << " microseconds!";
    return ctx;
};


int main()
{
    Context* ctx = eval();
    std::string key = "fn";
    MS_VALUE val = ctx->global->get(key);
    MS_Function fn = downcast<MS_Function>(val);
    MS_VALUE res = ctx->callFunction(fn, std::vector {MS_VALUE { 5.f }, MS_VALUE { 10.f }});
    std::cout<<downcast<float>(res);
    system("pause");
    return 0;
}
