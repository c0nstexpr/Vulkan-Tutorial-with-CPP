#pragma once

#include "pixel_traits.h"

namespace vulkan::utility::stb
{
	using std::array;
	using std::vector;
	using std::string;
	using std::string_view;
	using std::filesystem::path;

	enum class image_format { jpg, png, bmp, unknown };
	using image_format_underlying_type = std::underlying_type_t<image_format>;

	static constexpr array<
		string_view,
		static_cast<image_format_underlying_type>(image_format::unknown) + 1
	> image_format_str{".jpg", ".png", ".bmp", ".unknown"};
	image_format from_string(const string& extension);
	std::string to_string(const image_format format);

	template<channel Channel = channel::default_desired>
	class image
	{
	public:
		using pixel_t = pixel_t<Channel>;
		using pixels_type = vector<pixel_t>;
		using size_type = typename pixels_type::size_type;

	private:
		vector<pixel_t> pixels_;
		size_type height_;
		channel real_channel_type_ = Channel;

	public:

		using rect = vec<2, vec<2, size_type>>;

		static constexpr auto channel_type = Channel;

		constexpr image() noexcept = default;
		image(const size_type, const size_type, const channel real_channel = channel_type) noexcept;
		image(const stbi_uc*, const size_type, const size_type, const channel = channel_type);
		explicit image(const path&);
		image(const stbi_uc*, const int);

		template<channel RequiredChannel = channel_type>
		image(image<RequiredChannel> right);

		[[nodiscard]] vector<stbi_uc> to_stb_vec() const noexcept;

		[[nodiscard]] auto width() const noexcept;
		[[nodiscard]] auto height() const noexcept;
		[[nodiscard]] auto pixel_size() const noexcept;
		[[nodiscard]] auto real_channel() const noexcept;

		[[nodiscard]] auto cbegin() const noexcept;
		[[nodiscard]] auto cend() const noexcept;
		[[nodiscard]] auto begin() const noexcept;
		[[nodiscard]] auto end() const noexcept;
		[[nodiscard]] auto begin() noexcept;
		[[nodiscard]] auto end() noexcept;

		[[nodiscard]] auto& operator[](const size_type) noexcept;
		[[nodiscard]] const auto& operator[](const size_type) const noexcept;
	};
}

#include "image.tpp"