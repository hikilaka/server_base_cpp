#include <iostream>

#include "lua_cpp/lua.hpp"
#include "sysd/buffer.hpp"

int main() {
    /*sysd::buffer b;
    b << std::uint64_t(5099901);

    std::cout << b << std::endl;
    
    std::uint64_t something;
    b >> something;

    std::cout << b.data()[0] << std::endl;
    std::cout << "something=" << something << std::endl;
    std::cout << b << std::endl;*/

    LuaContext lua;
    lua.writeVariable("x", 5);
    lua.executeCode("x = x + 2");
    std::cout << lua.readVariable<int>("x") << std::endl;
    return 0;
}

