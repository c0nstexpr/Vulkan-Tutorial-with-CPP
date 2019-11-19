#pragma once

namespace vulkan::utility::stb
{
	template<channel Channel>
	image<Channel>::image(vector<vector<pixel_t>> pixels, const channel real_channel_type) :
		pixels_(std::move(pixels)), real_channel_type_(real_channel_type)
	{}

	template<channel Channel>
	image<Channel>::image(const size_type width, const size_type height, const channel real_channel) :
		image{{width, vector<pixel_t>{height}, real_channel}}
	{}

	template<channel Channel>
	image<Channel>::image(
		const stbi_uc* stb_uc_ptr,
		const size_type width,
		const size_type height,
		const channel real_channel
	) : image({width, vector<pixel_t>(height)}, static_cast<channel>(real_channel))
	{
		for(size_type i = 0; i < width; ++i)
			for(size_type j = 0; j < height; ++j)
				for(size_type k = 0; k < pixel_t::length(); ++k)
					pixels_[i][j][static_cast<length_t>(k)] = std::byte{stb_uc_ptr[i * height + j * pixel_t::length() + k]};
	}

	template<channel Channel>
	image<Channel>::image(const path& path)
	{
		int width = 0;
		int height = 0;
		int real_channel;
		const auto ptr = stbi_load(
			path.generic_u8string().c_str(),
			&width,
			&height,
			&real_channel,
			static_cast<int>(channel_type)
		);
		*this = image{ptr, static_cast<size_t>(width), static_cast<size_t>(height), channel{real_channel}};
		stbi_image_free(ptr);
	}

	template<channel Channel>
	image<Channel>::image(const stbi_uc* stb_uc_ptr, const int size)
	{
		int width, height, channel;
		const auto ptr = stbi_load_from_memory(
			stb_uc_ptr,
			size,
			&width,
			&height,
			&channel,
			static_cast<int>(channel_type)
		);
		*this = image{ptr, width, height, channel};
		stbi_image_free(ptr);
	}

	template<channel Channel>
	template<channel RequiredChannel>
	image<Channel>::image(image<RequiredChannel> right)
	{
		if constexpr(Channel == RequiredChannel)
			*this = std::move(right);
		else
		{
			auto vec = right.to_stb_vec();
			int width, height, real_channel;
			auto converted_ptr = stbi_load_from_memory(
				vec.data(),
				right * width(), right.height() * RequiredChannel,
				&width,
				&height,
				&real_channel,
				static_cast<int>(channel_type)
			);
			*this = image{converted_ptr, width, height, real_channel};
			stbi_image_free(converted_ptr);
		}
	}

	template<channel Channel>
	vector<stbi_uc> image<Channel>::to_stb_vec() const
	{
		vector<stbi_uc> stb_uc_vec(width() * height() * pixel_t::length());
		for(size_type i = 0; i < width(); ++i)
			for(size_type j = 0; j < height(); ++j)
				for(size_type k = 0; k < pixel_t::length(); ++k)
					stb_uc_vec[i * height() + j * pixel_t::length() + k] =
					static_cast<stbi_uc>(pixels_[i][j][static_cast<length_t>(k)]);
		return stb_uc_vec;
	}

	template<channel Channel>
	auto image<Channel>::width() const { return pixels_.size(); }

	template<channel Channel>
	auto image<Channel>::height() const { return width() != 0 ? pixels_[0].size() : 0; }

	template<channel Channel>
	auto image<Channel>::pixel_size() const { return width() * height(); }

	template<channel Channel>
	auto image<Channel>::real_channel() const { return real_channel_type_; }

	template<channel Channel>
	auto image<Channel>::cbegin() const { return pixels_.cbegin(); }

	template<channel Channel>
	auto image<Channel>::cend() const { return pixels_.cend(); }

	template<channel Channel>
	auto image<Channel>::begin() const { return pixels_.begin(); }

	template<channel Channel>
	auto image<Channel>::end() const { return pixels_.end(); }

	template<channel Channel>
	auto& image<Channel>::operator[](const size_type i) { return pixels_[i]; }

	template<channel Channel>
	const auto& image<Channel>::operator[](const size_type i) const { return pixels_[i]; }

	template<channel Channel>
	void image<Channel>::save(const path& save_path, const int quality) const
	{
		if(!save_path.has_extension())
			throw std::invalid_argument("path doesn't have extension");
		auto stb_vec = to_stb_vec();
		switch(from_string(save_path.extension().generic_u8string()))
		{
		case image_format::png:
		stbi_write_png(
			save_path.generic_u8string().c_str(),
			static_cast<int>(width()),
			static_cast<int>(height()),
			static_cast<int>(channel_type),
			stb_vec.data(),
			0
		);
		break;

		case image_format::jpg:
		stbi_write_jpg(
			save_path.generic_u8string().c_str(),
			static_cast<int>(width()),
			static_cast<int>(height()),
			static_cast<int>(channel_type),
			stb_vec.data(),
			quality
		);
		break;

		case image_format::bmp:
		stbi_write_bmp(
			save_path.generic_u8string().c_str(),
			static_cast<int>(width()),
			static_cast<int>(height()),
			static_cast<int>(channel_type),
			stb_vec.data()
		);
		break;

		default: break;
		}
	}

	template<channel Channel>
	template<channel RequiredChannel>
	image<RequiredChannel> image<Channel>::select(const rect selected) const
	{
		const vector<vector<pixel_t>> resized_pixels{selected.bottom_right.x - selected.upper_left.x};

		std::transform(
			pixels_.cbegin() + selected.upper_left.x,
			pixels_.cbegin() + selected.bottom_right.x,
			resized_pixels.begin(),
			[&selected](decltype(*pixels_.cbegin()) row_pixel)
			{
				return {row_pixel.cbegin() + selected.upper_left.y, row_pixel.cbegin() + selected.bottom_right.y};
			}
		);

		return image<RequiredChannel>{image{resized_pixels}};
	}
}