#ifndef SYSD_UTIL_HPP
#define SYSD_UTIL_HPP

#pragma once

#include <chrono>

namespace sysd {
    template <typename T>
    using time_point = std::chrono::time_point<T>;
}

#endif // SYSD_UTIL_HPP
