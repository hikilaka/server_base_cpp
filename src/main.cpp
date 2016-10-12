#include <iostream>

#include "sysd/buffer.hpp"

int main() {
    sysd::buffer b;
    b.print();
    b << std::uint32_t(0xdeadbeef);
    b << std::uint8_t(0xff);
    b << std::uint64_t(0x6060606060);
    b.print();
    return 0;
}

