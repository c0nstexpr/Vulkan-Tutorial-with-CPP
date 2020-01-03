#pragma once
#include "object.h"
#include "vulkan/utility/constant/constant.h"
#include <type_traits>

namespace vulkan::utility
{
    using std::tuple;
    using std::pair;
    using std::bitset;
    using std::initializer_list;

    [[nodiscard]] optional<decltype(MemoryAllocateInfo::memoryTypeIndex)> search_memory_type_index(
        const PhysicalDevice&,
        const DispatchLoaderDynamic&,
        const MemoryPropertyFlags,
        const decltype(MemoryRequirements::memoryTypeBits)
    );

    [[nodiscard]] pair<DeviceSize, vector<DeviceSize>> generate_memory_size_and_offsets(
        vector<pair<MemoryRequirements, size_t>>&
    );

    [[nodiscard]] pair<device_memory_object, vector<DeviceSize>> generate_buffer_memory_info(
        const device_object&,
        const vector<Buffer>&,
        const PhysicalDevice&,
        const MemoryPropertyFlags
    );

    template<typename Input>
    void write(const device_memory_object&, const device_object&, const Input, const Input, const DeviceSize = 0);

    template<typename T>
    void write(const device_memory_object&, const device_object&, const T&, const DeviceSize = 0);

    template<bool Cached, typename... Types>
    class static_memory
    {
    public:
        using type_list = type_list<Types...>;

        static constexpr auto host_memory_property = MemoryPropertyFlagBits::eHostVisible | (Cached ?
            MemoryPropertyFlagBits::eHostCached :
            MemoryPropertyFlagBits{});

        static constexpr auto device_memory_property = MemoryPropertyFlagBits::eDeviceLocal;

        static constexpr array<size_t, type_list::size> type_sizes = {sizeof(Types)...};

        template<typename T>
        static constexpr auto type_index = type_list::template type_index_v<T>;

        template<auto Index>
        using type = typename type_list::template type<Index>;

        template<template<typename T> typename RangeType>
        class base_array_values
        {
        public:
            template<typename T>
            using value_type = RangeType<T>;
        private:
            tuple<value_type<Types>...> type_values_;

            const device_object* device_ = nullptr;

            array<size_t, type_list::size> sizes_;

            array<buffer_object, type_list::size> device_local_buffers_;

            device_memory_object device_local_memory_;

            array<buffer_object, type_list::size> host_buffers_;

            device_memory_object host_memory_;

            array<DeviceSize, type_list::size> type_offsets_{};

            void generate_buffer_info(const array<BufferUsageFlags, type_list::size>&);
            void generate_memory_info(const PhysicalDevice&);
            void bind_buffer_memory() const;

            template<typename T, typename Op = empty_type>
            void write_impl(value_type<T>, const Op& = {});

        public:
            base_array_values() = default;
            base_array_values(
                const device_object&,
                const array<BufferUsageFlags, type_list::size>&,
                const decltype(sizes_)&


            
            );
            base_array_values(
                const PhysicalDevice&,
                const device_object&,
                const array<BufferUsageFlags, type_list::size>&,
                const decltype(sizes_)&


            
            );

            void initialize(const PhysicalDevice&);

            template<typename... T>
            void write(value_type<T> ...);

            template<typename T, typename Op = empty_type>
            void write(value_type<T>, const Op& = {});

            template<typename...>
            void flush();

            template<typename>
            constexpr const auto& read() const;

            void write_transfer_command(const CommandBuffer&) const;

            constexpr const auto& device_local_buffer(const size_t i) const;
            constexpr const auto& host_buffer(const size_t i) const;
            constexpr const auto& device_local_memory() const;
            constexpr const auto& host_memory() const;
        };

    private:
        template<size_t... Counts>
        struct array_values_type_traits
        {
            static constexpr array<size_t, type_list::size> counts = {Counts...};
            static_assert(type_list::size == counts.size(), "incompatible size");

            template<typename T>
            using value_type = array<T, counts[type_index<T>]>;
        };

    public:
        template<size_t... Counts>
        class array_values : public base_array_values<array_values_type_traits<Counts...>::template value_type>
        {
            using traits = array_values_type_traits<Counts...>;

        public:
            using base = base_array_values<traits::template value_type>;

            array_values() = default;
            array_values(const device_object&, const array<BufferUsageFlags, type_list::size>&);
            array_values(const PhysicalDevice&, const device_object&, const array<BufferUsageFlags, type_list::size>&);
        };

        class vector_values : public base_array_values<vector>
        {
        public:
            using base = base_array_values<vector>;
        };
    };
}

#include "static_memory.tpp"
