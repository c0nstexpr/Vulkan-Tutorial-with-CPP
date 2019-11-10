#pragma once

namespace utility
{
	template<typename T, typename U>
	property<T, U>::property(class_type& class_, const getter_type getter, const setter_type setter) :
		class_(class_),
		getter(getter),
		setter(setter)
	{}

	template<typename T, typename U>
	typename property<T, U>::value_type_const_reference property<T, U>::operator()() const
	{
		return (class_.*getter)();
	}

	template<typename T, typename U>
	property<T, U>::operator const U& () const { return (class_.*getter)(); }

	template<typename T, typename U>
	const property<T, U>& property<T, U>::operator=(value_type right) const
	{
		(class_.*setter)(std::move(right));
		return *this;
	}
}