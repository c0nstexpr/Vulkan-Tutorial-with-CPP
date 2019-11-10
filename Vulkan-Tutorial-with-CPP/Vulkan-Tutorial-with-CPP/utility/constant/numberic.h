#pragma once

#include <limits>

namespace utility::constant::numeric
{
    using std::numeric_limits;

    template<typename T>
    inline constexpr auto numberic_max = numeric_limits<T>::max();

    template<typename T>
    inline constexpr auto numberic_min = numeric_limits<T>::min();

    template<typename T>
    inline constexpr auto numberic_lowest = numeric_limits<T>::lowest();
}
