
#include <iostream>

#include "./Interpreter/Context/Context.h"

#include <chrono>
using namespace std::chrono;

int main()
{
    std::string code = "let res = 50; if (true) { res = res + res; };";
    Context ctx = Context();
    auto start = high_resolution_clock::now();
    ctx.run(code);
    auto stop = high_resolution_clock::now();
    std::string resName = "res";
    MS_VALUE resVal = ctx.global->get(resName);
    float val = downcast<float>(resVal);
    std::cout<<"Value " << val << std::endl;
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time it took: " << duration.count() << " microseconds!";
    system("pause");
    return 0;
}
