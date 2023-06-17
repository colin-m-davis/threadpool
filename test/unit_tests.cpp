#include "threadpool.hpp"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << argv[1] << std::endl;
    return dreadpools::hello_thread(std::stoi(std::string(argv[1])));
}
