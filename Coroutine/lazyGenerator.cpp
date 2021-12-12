// lazyGenerator.cpp
#include <iostream>
#include <vector>

generator<int> generatorForNumbers(int begin, int inc = 1) {
    for (int i = begin; ; i += inc) { // (4)
        co_yield i; // (3)
    }
}

int main() {
    const auto numbers = generatorForNumbers(-10); // (1)
    for (int i = 1; i <= 20; ++i) { // (5)
        std::cout << numbers << " ";
    }
    std::cout << "\n";

    for (auto n : generatorForNumbers(0, 5)) { // (2)
        std::cout << n << " ";
    }
    std::cout << "\n";
}