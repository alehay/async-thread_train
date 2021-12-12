#include "lazyGenerator.hpp"

#include <iostream>

generator<int> gen()
{
    for(int i=0;i<5;i++)
    {
        co_yield i;
    }
}

int main()
{
    for(auto&& val : gen())
    {
        std::cout << val << std::endl;
    }
    return 0;
}