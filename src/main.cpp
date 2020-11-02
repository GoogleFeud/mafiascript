
#include <iostream>
#include <fstream>

#include "./Interpreter/Context/Context.h"

#include <chrono>
using namespace std::chrono;

int main()
{
    std::string secondCode = "res = 336;";
    std::ifstream ifs("script.ms");
    std::string code( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
    Context ctx = Context();
    auto start = high_resolution_clock::now();
    ctx.run(code);
    ctx.run(secondCode);
    auto stop = high_resolution_clock::now();
    std::string resName = "res";
    MS_VALUE resVal = ctx.global->get(resName);
    float val = downcast<float>(resVal);
    std::cout<<"Value " << val << std::endl; 
/**    MS_Array val = downcast<MS_Array>(resVal);
    float firstEntry = downcast<float>(val->entries[1]);
    std::cout<<"First entry "<<firstEntry<<std::endl; **/
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time it took: " << duration.count() << " microseconds!";
    system("pause");
    return 0;
}
