#pragma once

#include <utility>

namespace utility
{
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
		property(class_type&, const getter_type, const setter_type);

		value_type_const_reference operator()() const;
		operator value_type_const_reference() const;

		const property& operator=(value_type right) const;

		const getter_type getter;
		const setter_type setter;
	};
}

#include "property.tpp"