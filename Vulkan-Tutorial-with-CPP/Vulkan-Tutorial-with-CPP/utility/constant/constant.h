#pragma once

#include <glm/gtc/constants.hpp>

namespace utility::constant
{
    //waiting for C++20 to remove this constant by using <numbers>
    template<typename T = float>
    static constexpr auto pi_v = glm::pi<T>();

    static constexpr size_t char_bit = CHAR_BIT;
}
