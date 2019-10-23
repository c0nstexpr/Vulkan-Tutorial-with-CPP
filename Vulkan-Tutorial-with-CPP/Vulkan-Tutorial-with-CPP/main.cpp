#include "vulkan_sample.h"
#include <thread>
int main()
{
	try
	{
		vulkan_sample sample;
		float angle = 90;
		sample.initialize();
		std::thread([&angle]() { while(true)std::cin >> angle; }).detach();
		while(sample.render([&sample, &angle](const auto index)
			{
				const auto time = duration_cast<milliseconds>(steady_clock_timer().time_since_epoch()).count() / 1000.0;
				static constexpr auto spin_speed = 90_deg;
				const auto& model = rotate<float>(mat4{1}, static_cast<float>(time * spin_speed), vec3{0, 0, 1});
				static const auto& view = lookAt(vec3{2, 2, 2}, vec3{0, 0, 0}, vec3{0, 0, 1});
				static const auto& extent = sample.swap_chain_create_info().imageExtent;
				const auto& proj = perspective(to_radian(angle), extent.width /
					static_cast<float>(extent.height), static_cast<float>(0.1), static_cast<float>(10));

				//proj[1][1] *= -1; invalid

				sample.transform.mat = proj * view * model;
				sample.flush_transform_to_memory();
			}));
	}
	catch(const std::exception & e) { std::cerr << e.what(); }
	return 0;
}