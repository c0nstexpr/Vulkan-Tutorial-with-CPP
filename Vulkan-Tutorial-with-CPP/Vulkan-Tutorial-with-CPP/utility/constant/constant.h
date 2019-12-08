#pragma once

#include <glm/gtc/constants.hpp>

namespace utility::constant
{
    //waiting for C++20 to remove this constant by using <numbers>
    template<typename T = float>
    inline constexpr auto pi_v = glm::pi<T>();

    inline constexpr size_t char_bit = CHAR_BIT;
}
