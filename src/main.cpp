#include <iostream>

#include "sysd/buffer.hpp"

int main() {
    sysd::buffer b;
    std::cout << b << std::endl;
    b << sysd::bits(2, 3) << sysd::bits(3, 4);
    std::cout << b << std::endl;
    return 0;
}

