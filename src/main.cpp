
#include <iostream>
#include <fstream>

#include "./Interpreter/Context/std.h"

#include <chrono>
using namespace std::chrono;


Context* setup() {
    Context* ctx = new Context();
    auto fnVal = MS_VALUE::make([&](std::vector<MS_POINTER> vals) -> MS_POINTER {
        for (MS_POINTER val : vals) {
            std::cout<<val->toString() + "\n";
        };
        return MS_VALUE::make();
    });
    fnVal->flags.isConst = true;
    ctx->global->unsafeSet(std::string{"print"}, fnVal);
    return ctx;
};

void replMode(Context* ctx) {
    while (true) {
        std::string code;
        std::getline(std::cin, code);
        auto res = ctx->repl(code);
        std::cout<<res->toString()<<std::endl;
    };
}


int main(int argCount, char* args[]) {
    Context* ctx = setup();
    auto path = args[1];
    if (!path) replMode(ctx);
    else {
    std::ifstream ifs(path);
    std::string code( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
    auto startParsing = high_resolution_clock::now();
    auto parsed = parseAST(code);
    auto stopParsing = high_resolution_clock::now();
    auto parsingDuration = duration_cast<microseconds>(stopParsing - startParsing);
    auto startExecution = high_resolution_clock::now();
    ctx->run(parsed);
    auto stopExecution = high_resolution_clock::now();
    auto executionDuration = duration_cast<microseconds>(startExecution - stopExecution);
    std::cout << "Parsing took " << duration_cast<microseconds>(stopParsing - startParsing).count() << " microseconds!\n";
    std::cout << "Executing took "<< duration_cast<microseconds>(stopExecution - startExecution).count() << " microseconds!\n";
    }
    system("pause");
    return 0;
}


/**
 * auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Took " << duration.count() << " microseconds!\n";
    */