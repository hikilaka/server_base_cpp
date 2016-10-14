#ifndef SYSD_TERM_COLOR_HPP
#define SYSD_TERM_COLOR_HPP

#pragma once

#include <string>

namespace sysd::term::color {
    template <typename T = std::string>
    const T clear = T("\033[0m");

    template <typename T = std::string>
    const T blue = T("\033[1;34m");

    template <typename T = std::string>
    const T green = T("\033[1;32m");

    template <typename T = std::string>
    const T cyan = T("\033[1;36m");

    template <typename T = std::string>
    const T red = T("\033[1;31m");

    template <typename T = std::string>
    const T purple = T("\033[1;35m");
    
    template <typename T = std::string>
    const T yellow = T("\033[1;33m");

    template <typename T = std::string>
    const T white = T("\033[1;37m");
}

#endif // SYSD_TERM_COLOR_HPP

