#include <iostream>

#include "sysd/buffer.hpp"

int main() {
    sysd::buffer b;
    b.print();
    b << std::string("hello world!");
    b.print();
    return 0;
}

