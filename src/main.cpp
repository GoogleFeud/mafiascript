
#include <iostream>
#include <fstream>

#include "./Interpreter/Context/Context.h"

#include <chrono>
#include <thread>
using namespace std::chrono;


MS_String eval() {
std::ifstream ifs("script.ms");
    std::string code( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
    Context ctx = Context();
    auto start = high_resolution_clock::now();
    ctx.run(code);
    auto stop = high_resolution_clock::now();
    std::string resName = "res";
  /**  float val = downcast<float>(resVal);
    std::cout<<"Value " << val << std::endl; **/
/**    MS_Array val = downcast<MS_Array>(resVal);
    float firstEntry = downcast<float>(val->entries[1]);
    std::cout<<"First entry "<<firstEntry<<std::endl; **/
  /**  MS_Object obj = downcast<MS_Object>(resVal);
    MS_String cEntry = downcast<MS_String>(obj->entries["\"c\""]);
    std::cout<<"C entry "<<*cEntry<<std::endl; **/
   MS_VALUE val = ctx.global->get(resName);
   MS_String str = downcast<MS_String>(&val);
    std::cout<<"Res "<<*str<<std::endl; 
    //delete ctx.global;
    std::cout<<str.use_count();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time it took: " << duration.count() << " microseconds!";
    return str;
};


int main()
{
    std::thread t1(eval);
    system("pause");
    return 0;
}
