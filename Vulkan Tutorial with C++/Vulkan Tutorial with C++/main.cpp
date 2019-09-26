#include"vulkan_triangle_sample.h"

int main()
{
	const auto pi = std::atan(1) * 4;
	const auto& timer = std::chrono::steady_clock::now;
	std::chrono::steady_clock::time_point last_time;
	long long duration = 0;
	constexpr double scale = 500;

	try
	{
		vulkan_triangle_sample sample;
		const auto& buffer_record = [&](const uint32_t& index)
		{
			duration += std::chrono::duration_cast<std::chrono::milliseconds>(timer() - last_time).count();
			if(duration > pi * scale)
				duration -= pi * scale;
			last_time = timer();
			for(size_t i = 0; i < sample.vertices().size(); ++i)
			{
				sample.vertices()[i].color = {
					std::sin(duration / scale),
					std::sin(duration / scale),
					std::sin(duration / scale)
				};
			}
			sample.flush_vertices_to_memory();
		};

		sample.initialize();

		while(sample.render(buffer_record));
	}
	catch(const std::exception& e) { std::cerr << e.what(); }
	return 0;
}