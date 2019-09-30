#include"vulkan_triangle_sample.h"
#include <chrono>

int main()
{
	using namespace std::chrono;

	const auto pi = std::acos(-1);
	const auto& timer = steady_clock::now;
	decltype(timer()) last_time;
	decltype(timer() - last_time) duration = 0s;
	unsigned short frame_count = 0;
	long long value = 0;
	constexpr unsigned short scale = 500;

	try
	{
		vulkan_triangle_sample sample;
		const auto& buffer_record = [&](const uint32_t& index)
		{
			const auto& delta = timer() - last_time;

			duration += delta;
			if(duration >= 1s)
			{
				std::cout << "fps" << frame_count << '\n';
				frame_count = 0;
				duration -= duration;
			}

			value += duration_cast<milliseconds>(delta).count();
			if(value > pi* scale) value -= pi * scale;
			for(size_t i = 0; i < sample.vertices().size(); ++i)
				sample.vertices()[i].color[i] = static_cast<float>(std::sin(value / scale));
			sample.flush_vertices_to_memory();

			frame_count++;
			last_time = timer();
		};

		sample.initialize();

		while(sample.render(buffer_record));
	}
	catch(const std::exception & e) { std::cerr << e.what(); }
	return 0;
}