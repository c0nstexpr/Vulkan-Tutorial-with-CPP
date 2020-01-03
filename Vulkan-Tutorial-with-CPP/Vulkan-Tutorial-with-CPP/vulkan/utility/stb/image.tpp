#pragma once

namespace vulkan::utility::stb
{
	template<channel Channel>
	image<Channel>::image(const size_type width, const size_type height, const channel real_channel) noexcept :
		pixels_(width* height* channel_type),
		height_(height),
		real_channel_type_(real_channel)
	{}

	template<channel Channel>
	image<Channel>::image(
		const stbi_uc* stb_uc_ptr,
		const size_type width,
		const size_type height,
		const channel real_channel
	) :
		height_(height),
		real_channel_type_(real_channel)
	{
		const auto ptr = reinterpret_cast<const pixel_t*>(stb_uc_ptr);
		pixels_ = {ptr, ptr + width * height};
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
		if(!ptr) throw std::runtime_error("cannot open image file:" + path.string());
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
	vector<stbi_uc> image<Channel>::to_stb_vec() const noexcept
	{
		const auto ptr = reinterpret_cast<const stbi_uc*>(pixels_.data());
		return {ptr, ptr + width() * height() * static_cast<channel_underlying_type>(channel_type)};
	}

	template<channel Channel>
	auto image<Channel>::width() const noexcept
	{
		return pixels_.size() / height();
	}

	template<channel Channel>
	auto image<Channel>::height() const noexcept { return height_; }

	template<channel Channel>
	auto image<Channel>::pixel_size() const noexcept { return pixels_.size(); }

	template<channel Channel>
	auto image<Channel>::real_channel() const noexcept { return real_channel_type_; }

	template<channel Channel>
	auto image<Channel>::cbegin() const noexcept { return pixels_.cbegin(); }

	template<channel Channel>
	auto image<Channel>::cend() const noexcept { return pixels_.cend(); }

	template<channel Channel>
	auto image<Channel>::begin() const noexcept { return pixels_.begin(); }

	template<channel Channel>
	auto image<Channel>::end() const noexcept { return pixels_.end(); }

	template<channel Channel>
	auto image<Channel>::begin() noexcept { return pixels_.begin(); }

	template<channel Channel>
	auto image<Channel>::end() noexcept { return pixels_.end(); }

	template<channel Channel>
	auto& image<Channel>::operator[](const size_type i) noexcept { return pixels_[i]; }

	template<channel Channel>
	const auto& image<Channel>::operator[](const size_type i) const noexcept { return pixels_[i]; }
}