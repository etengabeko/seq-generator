#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <typeinfo>
#include <vector>

#include "thread_safe_generator.h"

namespace
{

template<typename T>
void unused(const T& v) { (void)(v); }

std::string& strInc(std::string& str)
{
    str += str;
    return str;
}

}

int main(int argc, char* argv[])
{
    ::unused(argc);
    ::unused(argv);

    using namespace sequence;

    ThreadSafeGenerator<int> gen(0);

    auto func = [&gen]()
    {
        for (size_t i = 0; gen.peekNext() < 100; ++i)
        {
            std::cout << "next = " << gen.next() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };

    std::vector<std::unique_ptr<std::thread>> threads;
    for (size_t i = 0; i < 10; ++i)
    {
        threads.emplace_back(new std::thread(func));
    }

    for (auto& each : threads)
    {
        each->join();
    }

    auto inc = ::strInc;

    ThreadSafeGenerator<std::string,
                        decltype(inc)> gens("#", inc);

    auto funcs = [&gens]()
    {
        for (size_t i = 0; gens.peekNext().size() < 255; ++i)
        {
            std::cout << "next = " << gens.next() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };

    threads.clear();
    for (size_t i = 0; i < 1; ++i)
    {
        threads.emplace_back(new std::thread(funcs));
    }

    for (auto& each : threads)
    {
        each->join();
    }

    return EXIT_SUCCESS;
}
