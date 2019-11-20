#pragma once

namespace vulkan
{
	template<typename T>
	bool vulkan_sample::render([[maybe_unused]] const T& t)
	{
		static auto&& last_time = time::steady_clock_timer().time_since_epoch();

		{
			auto&& now = time::steady_clock_timer().time_since_epoch();
			if(time::duration_cast<time::seconds>(now - last_time).count() >= 1)
			{
				glfwSetWindowTitle(window_, (window_title + " fps:" + std::to_string(fps_)).c_str());
				last_time = std::move(now);
				fps_ = 0;
			}
			else fps_++;
		}

		device_->waitIdle(device_.dispatch());

		if(glfwWindowShouldClose(window_))
			return false;
		glfwPollEvents();

		const auto& swap_chain_image_syn = *swap_chain_image_syn_[frame_count_ % swap_chain_image_syn_.size()];
		const auto index = device_->acquireNextImageKHR(
			*swap_chain_,
			numberic_max<uint64_t>,
			swap_chain_image_syn,
			nullptr,
			device_.dispatch()
		).value;

		try
		{
			if constexpr(!std::is_same_v<T, empty_type>)
				t(index);

			submit_infos_[index].wait_semaphores_property = vector<Semaphore>{swap_chain_image_syn};

			if(device_->getFenceStatus(*gpu_syn_[index], device_.dispatch()) == Result::eSuccess)
			{
				device_->waitForFences({*gpu_syn_[index]}, true, numberic_max<uint64_t>, device_.dispatch());
				device_->resetFences({*gpu_syn_[index]}, device_.dispatch());
			}

			graphics_queue_.submit({submit_infos_[index]}, *gpu_syn_[index], device_.dispatch());

			present_queue_.presentKHR(present_infos_[index], device_.dispatch());
		}
		catch(const SystemError & error)
		{
			if(error.code() == Result::eErrorOutOfDateKHR || error.code() == Result::eSuboptimalKHR)
			{
				re_initialize_vulkan();
				return true;
			}
			std::cerr << error.what();
			return false;
		}
		catch(const std::exception & e) { std::cerr << e.what(); return false; }

		++frame_count_;
		return true;
	}

	constexpr auto vulkan_sample::get_transform() const -> const decltype(transform_mat_)&
	{
		return transform_mat_;
	}

	constexpr auto vulkan_sample::get_vertices() const-> const decltype(transfer_memory_)::value_type<vertex>&
	{
		return transfer_memory_.read<vertex>();
	}

	constexpr auto vulkan_sample::get_indices() const-> const decltype(transfer_memory_)::value_type<uint32_t>&
	{
		return transfer_memory_.read<uint32_t>();
	}
}