#include <iostream>

#include "sysd/buffer.hpp"

int main() {
    sysd::buffer b;
    b << std::uint64_t(5099901);

    std::cout << b << std::endl;
    
    std::uint64_t something;
    b >> something;

    std::cout << b.data()[0] << std::endl;
    std::cout << "something=" << something << std::endl;
    std::cout << b << std::endl;
    return 0;
}

