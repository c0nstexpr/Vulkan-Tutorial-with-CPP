#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <shaderc/shaderc.hpp>
#include <string>
#include <map>
#include <set>
#include <type_traits>
#include <filesystem>

using std::string;
using std::optional;
using std::vector;
using std::set;
using std::map;
using std::pair;
using std::tuple;
using std::nullopt;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::stringstream;
using std::istringstream;
using std::ostringstream;
using std::filesystem::path;

// ReSharper disable IdentifierTypo
extern ifstream cfin;
extern ofstream cfout;
extern istringstream csin;
extern ostringstream csout;

// ReSharper restore IdentifierTypo

template<typename T, typename U = T, typename Cmp>
static bool is_included(const T& required, const U& available, const Cmp& cmp)
{
	return std::all_of(required.cbegin(), required.cend(),
		[&](typename T::const_reference r)
		{
			return std::any_of(available.cbegin(), available.cend(), [&](typename U::const_reference a) { return cmp(r, a); });
		});
}

template<typename T, typename U = T>
static bool is_included(const T & required, const U & available)
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
