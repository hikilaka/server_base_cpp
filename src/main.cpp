#include <iostream>

#include "lua_cpp/lua.hpp"
#include "sysd/buffer.hpp"

int main() {
    sysd::buffer b;
    b << std::uint8_t(40) << std::uint8_t(99) << std::uint8_t(1);
    b << std::string("hikilaka");

    std::uint8_t attack, strength, defence;
    std::string username;

    b >> attack;
    b >> strength;
    b >> defence;
    b >> username;

    std::cout << "attack " << static_cast<int>(attack) << "/40" << std::endl;
    std::cout << "strength " << static_cast<int>(strength) << "/99" << std::endl;
    std::cout << "defence " << static_cast<int>(defence) << "/1" << std::endl;
    std::cout << "username " << username << "/hikilaka" << std::endl;
    return 0;
}

