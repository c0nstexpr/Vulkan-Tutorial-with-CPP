#pragma once

#include "pixel_traits.h"

namespace vulkan::utility::stb
{
	template<typename T>
	struct coordinate { T x, y; };

	enum class image_format { jpg, png, bmp, unknown };
	using image_format_underlying_type = std::underlying_type_t<image_format>;

	constexpr array<
		string_view,
		static_cast<image_format_underlying_type>(image_format::unknown) + 1
	> image_format_str{".jpg", ".png", ".bmp", ".unknown"};
	image_format from_string(const string extension);
	std::string to_string(const image_format format);

	template<channel Channel = channel::default_desired>
	class image
	{
	public:
		using pixel_t = pixel_t<Channel>;

	private:
		vector<vector<pixel_t>> pixels_;
		channel real_channel_type_ = Channel;

		image(vector<vector<pixel_t>>, const channel = channel_type);

	public:

		// ReSharper disable CppRedundantTypenameKeyword
		// ReSharper or Intellisense Bug
		using size_type = typename decltype(pixels_)::size_type;

		// ReSharper restore CppRedundantTypenameKeyword

		struct rect { coordinate<size_type> upper_left, bottom_right; };

		static constexpr auto channel_type = Channel;

		image() = default;
		image(const size_type, const size_type, const channel real_channel = channel_type);
		image(const stbi_uc*, const size_type, const size_type, const channel = channel_type);
		explicit  image(const path&);
		image(const stbi_uc*, const int);

		template<channel RequiredChannel = channel_type>
		image(image<RequiredChannel> right);
		vector<stbi_uc> to_stb_vec() const;

		auto width() const;
		auto height() const;
		auto real_channel() const;

		auto cbegin() const;
		auto cend() const;
		auto begin() const;
		auto end() const;

		auto& operator[](const size_type);
		const auto& operator[](const size_type) const;

		void save(const path&, const int quality) const;

		template<channel RequiredChannel = channel_type>
		image<RequiredChannel> select(const rect) const;
	};
}

#include "image.tpp"