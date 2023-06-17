#include "threadpool.hpp"
#include <string>
#include <iostream>
#include <chrono>


using namespace std::chrono_literals;


int main(int argc, char* argv[]) {
    dreadpools::ThreadPool my_threadpool(4);
    std::this_thread::sleep_for(20ms);
    return 0;
}
