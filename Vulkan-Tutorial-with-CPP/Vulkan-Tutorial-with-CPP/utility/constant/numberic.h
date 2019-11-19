#pragma once

#include <limits>

namespace utility::constant::numeric
{
    using std::numeric_limits;

    template<typename T>
    static constexpr auto numberic_max = numeric_limits<T>::max();

    template<typename T>
    static constexpr auto numberic_min = numeric_limits<T>::min();

    template<typename T>
    static constexpr auto numberic_lowest = numeric_limits<T>::lowest();
}
