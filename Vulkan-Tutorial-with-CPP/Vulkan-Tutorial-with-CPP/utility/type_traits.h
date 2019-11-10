#pragma once

#include <type_traits>

namespace utility::type_traits
{
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

    //C++ 20 features
    template<typename T>
    struct type_identity { using type = T; };

    template<typename T>
    using type_identity_t = typename type_identity<T>::type;

    template<auto Value>
    struct value_identity { static constexpr auto value = Value; };

    template<auto Value>
    static constexpr auto value_identity_v = value_identity<Value>::value;

    template<bool Test, typename T, typename  U>
    constexpr std::conditional_t<Test, T, U> conditional_value(const T t, const U u)
    {
        if constexpr(Test)
            return t;
        return u;
    }

    template<bool Test, auto T, auto  U>
    constexpr auto conditional_v = conditional_value(T, U);

    template<bool Test, typename T>
    constexpr std::enable_if_t<Test, T> enable_value_if(const T t) { return t; }

    template<bool Test, auto Value>
    constexpr auto enable_value_if_v = enable_value_if(Value);

    template<typename... Types>
    struct type_list;

    template<typename T, typename... Types>
    struct type_list<T, Types...>
    {
        using first = T;
        using rest_type_list = type_list<Types...>;

    private:
        template<auto Index>
        static constexpr auto type_impl()
        {
            if constexpr(Index == 0)
                return std::declval<T>();
            else
                return std::declval<typename type_list<Types...>::template type<Index - 1>>();
        }

        template<typename U>
        static constexpr auto index_impl()
        {
            if constexpr(std::is_same_v<T, U>)
                return 0;
            else
                return type_list<Types...>::template type_index_v<U> +1;
        }

    public:
        using first = T;
        using rest_type_list = type_list<Types...>;

        static constexpr auto size = sizeof...(Types) + 1;

        //from index to type
        template<auto Index>
        using type = decltype(type_impl<Index>());

        template<auto Index>
        struct has_index : std::conditional_t<(Index < size), std::true_type, std::false_type>{};

        template<auto Index>
        static constexpr auto has_index_v = has_index<Index>::value;

        //from type to index
        template<typename U>
        using type_index = std::integral_constant<size_t, index_impl<U>()>;

        template<typename U>
        static constexpr auto type_index_v = type_index<U>::value;

        template<typename U>
        struct has_type : std::conditional_t<
            std::is_same_v<T, U> || type_list<Types...>::has_type_v,
            std::true_type,
            std::false_type
        >
        {};

        template<typename U>
        static constexpr auto has_type_v = has_index<U>::value;
    };

    template<typename T>
    struct type_list<T>
    {
        static constexpr size_t size = 1;

        template<auto Index>
        struct index_type : type_identity<T>
        {
            static_assert(Index == 0, "index out of parameter pack range");
        };

        template<auto Index>
        using type = typename index_type<Index>::type;

        template<auto Index>
        struct has_index : std::conditional_t<(Index < size), std::true_type, std::false_type>{};

        template<auto Index>
        static constexpr auto has_index_v = has_index<Index>::value;

        template<typename U>
        struct type_index :std::integral_constant<size_t, 0>
        {
            static_assert(std::is_same_v<T, U>, "type is not in parameter pack");
        };

        template<typename U>
        static constexpr auto type_index_v = type_index<U>::value;

        template<typename U>
        struct has_type : std::conditional_t<std::is_same_v<T, U>, std::true_type, std::false_type> {};

        template<typename U>
        static constexpr auto has_type_v = has_index<U>::value;
    };
}
