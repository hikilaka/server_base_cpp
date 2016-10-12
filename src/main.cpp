#include <iostream>

#include "sysd/buffer.hpp"

int main() {
    sysd::buffer b;
    b.print();
    b << sysd::bits(2, 3);
    b << sysd::bits(3, 4);
    b.print();
    return 0;
}

