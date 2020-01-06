#pragma once

#include "property.h"
#include "type_traits.h"
#include "time.h"
#include "constant/numberic.h"
#include "constant/constant.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <array>
#include <map>
#include <set>
#include <tuple>

#include <iostream>
#include <fstream>
#include <sstream>

#include <string_view>

#include <algorithm>
#include <numeric>

#include <memory>
#include <optional>
#include <functional>
#include <bitset>
#include <filesystem>

#include <stdexcept>

using namespace std::literals;

namespace utility
{
    using namespace type_traits;
    using namespace std::placeholders;

    using std::initializer_list;
    using std::array;
    using std::set;
    using std::map;
    using std::pair;
    using std::tuple;
    using std::nullopt;
    using std::ifstream;
    using std::ofstream;
    using std::istream;
    using std::cin;
    using std::ostream;
    using std::cout;
    using std::cerr;
    using std::stringstream;
    using std::istringstream;
    using std::ostringstream;
    using std::string;
    using std::string_view;
    using std::vector;
    using std::bitset;
    using std::filesystem::path;
    using std::unique_ptr;
    using std::optional;
    using std::size_t;

    // ReSharper disable IdentifierTypo
    extern ifstream cfin;
    extern ofstream cfout;
    extern istringstream csin;
    extern ostringstream csout;

    // ReSharper restore IdentifierTypo

    using namespace glm;

    constexpr bool is_debug =
#ifdef _DEBUG
        true
#else
        false
#endif
        ;

#define MEMBER_OFFSET(s,m) [](){static_assert(std::is_standard_layout_v<s>, "struct is not standard layout"); return offsetof(s, m);}()

    template<typename T, typename U = T, typename Cmp>
    bool is_included(const T&, const U&, const Cmp&);

    template<typename T, typename U = T>
    bool is_included(const T&, const U&);

    template<typename T, typename Input, typename UnaryOp>
    T range_transform(const Input, const Input, UnaryOp&&);

    template<typename T, typename Input>
    T range_transform(const Input, const Input);

    template<typename T, typename U, typename UnaryOp>
    T container_transform(U&&, UnaryOp&&);

    template<typename T, typename U>
    T container_transform(U&&);

    template<typename UnaryOp, typename InputFirst, typename... InputRest>
    constexpr void for_each(UnaryOp&&, InputFirst, const InputFirst, InputRest...);

    template<typename UnaryOp, typename InputFirst, typename... InputRest>
    constexpr void for_each(UnaryOp&&, InputFirst&, InputRest&...);

    template<typename T = float>
    constexpr auto to_radian(const T);

    constexpr long double operator"" _deg(const long double);

    constexpr long double operator"" _deg(const unsigned long long);
}

#include "utility.tpp"