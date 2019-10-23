#pragma once

#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <shaderc/shaderc.hpp>
#include <map>
#include <set>
#include <filesystem>
#include <chrono>
#include <glm/gtc/constants.hpp>
#include <memory>

using std::string;
using std::optional;
using std::vector;
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
using std::filesystem::path;
using std::ratio;
using std::numeric_limits;
using std::initializer_list;

using namespace std::literals;
using namespace std::chrono;

// ReSharper disable IdentifierTypo
extern ifstream cfin;
extern ofstream cfout;
extern istringstream csin;
extern ostringstream csout;

// ReSharper restore IdentifierTypo

constexpr bool is_debug =
#ifdef _DEBUG
true
#else
false
#endif
;

#define MEMBER_OFFSET(s,m) [](){static_assert(std::is_standard_layout_v<s>, "struct is not standard layout"); return offsetof(s, m);}()

template<typename T, typename U = T, typename Cmp>
bool is_included(const T& required, const U& available, const Cmp& cmp)
{
	return std::all_of(required.cbegin(), required.cend(),
		[&](typename T::const_reference r)
		{
			return std::any_of(available.cbegin(), available.cend(), [&](typename U::const_reference a) { return cmp(r, a); });
		});
}

template<typename T, typename U = T>
bool is_included(const T& required, const U& available)
{
	return std::all_of(required.cbegin(), required.cend(),
		[&](const auto& r)
		{
			return std::find(available.cbegin(), available.cend(), r) != available.cend();
		});
}

struct empty_type {};

template<typename T, typename U, typename W = void>
using enable_if_convertible = std::enable_if<std::is_convertible_v<T, U>, W>;

template<typename T, typename U, typename W = void>
using enable_if_convertible_t = typename enable_if_convertible<T, U, W>::type;

template<typename T, typename U, typename W = void>
using enable_if_same = std::enable_if<std::is_same_v<T, U>, W>;

template<typename T, typename U, typename W = void>
using enable_if_same_t = typename enable_if_same<T, U, W>::type;

template<typename T, typename U, typename W = void>
using enable_if_decay_same = std::enable_if<std::is_same_v<std::decay_t<T>, U>, W>;

template<typename T, typename U, typename W = void>
using enable_if_decay_same_t = typename enable_if_decay_same<T, U, W>::type;

template<typename T>
using decay_to_origin = std::decay<std::remove_pointer_t<T>>;

template<typename T>
using decay_to_origin_t = typename decay_to_origin<T>::type;

inline constexpr auto steady_clock_timer = steady_clock::now;

inline constexpr auto high_resolution_clock_timer = high_resolution_clock::now;

inline constexpr auto system_clock_timer = system_clock::now;

template<typename T>
inline constexpr auto numberic_max = numeric_limits<T>::max();

template<typename T>
inline constexpr auto numberic_min = numeric_limits<T>::min();

template<typename T>
inline constexpr auto numberic_lowest = numeric_limits<T>::lowest();

//waiting for C++20 to remove this constant by using <numbers>
template<typename T = float>
inline constexpr auto pi_v = glm::pi<T>();

template<typename T = float>
constexpr T to_radian(const T deg)
{
	if constexpr(std::is_integral_v<T>)
		return deg * static_cast<T>(pi_v<long double>) / static_cast<T>(180);
	else
		return deg * pi_v<T> / static_cast<T>(180);
}

constexpr long double operator"" _deg(const long double deg)
{
	return to_radian(deg);
}

constexpr long double operator"" _deg(const unsigned long long deg)
{
	return static_cast<long double>(to_radian(deg));
}

template<typename T, typename U>
class property
{
public:
	using class_type = T;
	using value_type = U;
	using value_type_reference = value_type&;
	using value_type_const_reference = const value_type&;
	using value_type_move_reference = value_type&&;
	using getter_type = value_type_const_reference(class_type::*)()const;
	using setter_type = void (class_type::*)(value_type);

private:
	class_type& class_;

public:
	property(class_type& class_, const getter_type getter, const setter_type setter) :
		class_(class_),
		getter(getter),
		setter(setter)
	{}

	value_type_const_reference operator()() const { return (class_.*getter)(); }
	operator value_type_const_reference() const { return (class_.*getter)(); }

	const property& operator=(value_type right) const
	{
		(class_.*setter)(std::move(right));
		return *this;
	}

	const getter_type getter;
	const setter_type setter;
};