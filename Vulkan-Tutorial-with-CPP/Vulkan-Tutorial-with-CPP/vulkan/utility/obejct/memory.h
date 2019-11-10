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

    [[nodiscard]] pair<device_memory, vector<DeviceSize>> generate_buffer_memory_info(
        const device&,
        const vector<Buffer>&,
        const PhysicalDevice&,
        const MemoryPropertyFlags
    );

    template<typename Input>
    void write(const device_memory&, const device&, const Input, const Input, const DeviceSize = 0);

    template<typename T>
    void write(const device_memory&, const device&, const T&, const DeviceSize = 0);

    template<bool Cached, typename... Types>
    class memory
    {
    public:
        using type_list = type_list<Types...>;

        static constexpr auto host_memory_property = MemoryPropertyFlagBits::eHostVisible |
            (Cached ? MemoryPropertyFlagBits::eHostCached : MemoryPropertyFlagBits{0});

        static constexpr auto device_memory_property = MemoryPropertyFlagBits::eDeviceLocal;

        static constexpr std::array<size_t, type_list::size> type_sizes = {sizeof(Types)...};

        template<typename T>
        static constexpr auto type_index = type_list::template type_index_v<T>;

        template<auto Index>
        using type = typename type_list::template type<Index>;

        template<size_t... Counts>
        class array_values
        {
        public:
            static constexpr array<size_t, type_list::size> counts = {Counts...};
            static_assert(type_list::size == counts.size(), "incompatible size");

            template<typename T, size_t Count>
            struct element_type : type_identity<array<T, Count>> {};

            template<typename T>
            struct element_type<T, 1> : type_identity<T> {};

            template<typename T>
            struct element_type<T, 0> : type_identity<T> {};

            template<typename T>
            using value_type = typename element_type<T, counts[type_index<T>]>::type;

        private:
            tuple<value_type<Types>...> type_values_;

            const device* device_ = nullptr;

            array<buffer, type_list::size> device_local_buffers_;
            device_memory device_local_memory_;

            array<buffer, type_list::size> host_buffers_;
            device_memory host_memory_;

            array<DeviceSize, type_list::size> type_offsets_{};

            void generate_buffer_info(const array<BufferUsageFlags, type_list::size>&);

            void generate_memory_info(const PhysicalDevice&);

            void bind_buffer_memory() const;

            template<typename T>
            void write_impl(value_type<T> values);

        public:
            array_values() = default;
            explicit array_values(const device&);

            array_values(
                const device&,
                const array<BufferUsageFlags, type_list::size>&
            );

            array_values(
                const device&,
                const array<BufferUsageFlags, type_list::size>&,
                const PhysicalDevice&
            );

            void initialize(const PhysicalDevice&);

            template<typename... T>
            void write(value_type<T>... values)
            {
                (write_impl<T>(values), ...);
            }

            template<typename... T>
            void flush();

            template<class T>
            constexpr const auto& read() const;

            void write_transfer_command(const command_buffer&) const;

            constexpr const auto& device_local_buffer(const size_t i) const;
            constexpr const auto& host_buffer(const size_t i) const;
            constexpr const auto& device_local_memory() const;
            constexpr const auto& host_memory() const;
        };
    };
}

#include "memory.tpp"