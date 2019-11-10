#pragma once

namespace utility
{
	template<typename T, typename U, typename Cmp>
	bool is_included(const T& required, const U& available, const Cmp& cmp)
	{
		return std::all_of(required.cbegin(), required.cend(),
			[&](typename T::const_reference r)
			{
				return std::any_of(available.cbegin(), available.cend(), [&](typename U::const_reference a) { return cmp(r, a); });
			});
	}

	template<typename T, typename U>
	bool is_included(const T& required, const U& available)
	{
		return std::all_of(required.cbegin(), required.cend(),
			[&](const auto& r)
			{
				return std::find(available.cbegin(), available.cend(), r) != available.cend();
			});
	}

	template<typename T, typename Input, typename UnaryOp>
	T range_transform(const Input begin, const Input end, const UnaryOp& op)
	{
		using input_type = decltype(*begin);
		using output_type = typename T::value_type;

		const auto length = std::distance(begin, end);
		if(length < 0)
			throw std::invalid_argument("input range incorrect, end should larger than begin");
		T t{static_cast<size_t>(length)};
		std::transform(begin, end, t.begin(), [&op](decltype(*begin) element) { return op(element); });

		return t;
	}

	template<typename T, typename Input>
	T range_transform(const Input begin, const Input end)
	{
		return range_transform<T>(begin, end, [](auto& element) { return std::move(element); });
	}

	template<typename T, typename U, typename UnaryOp>
	T container_transform(U&& u, const UnaryOp& op)
	{
		return range_transform<T>(u.begin(), u.end(), op);
	}

	template<typename T, typename U>
	T container_transform(U&& u)
	{
		return container_transform<T>(u, [](auto& element) { return std::move(element); });
	}

	template<typename T>
	constexpr auto to_radian(const T deg)
	{
		if constexpr(std::is_integral_v<T>)
			return deg * constant::pi_v<long double> / 180;
		else
			return deg * constant::pi_v<T> / 180;
	}

	template<typename UnaryOp, typename InputFirst, typename... InputRest>
	constexpr void for_each(const UnaryOp op, InputFirst begin, const InputFirst end, InputRest... rest)
	{
		while(begin != end)
			op(*begin++, *rest++...);
	}

	constexpr long double operator"" _deg(const long double deg) { return to_radian(deg); }

	constexpr long double operator"" _deg(const unsigned long long deg) { return to_radian(deg); }
}