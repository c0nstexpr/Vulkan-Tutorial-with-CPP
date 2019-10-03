#include "vulkan_sample.h"
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
	constexpr float scale = 500;
	float a = 0.5;

	try
	{
		vulkan_sample sample;
		const auto& buffer_record = [&](const uint32_t& index)
		{
			const auto& delta = timer() - last_time;
			float angle;
			float sin_v;
			float cos_v;

			duration += delta;
			if(duration >= 1s)
			{
				std::cout << "fps" << frame_count << '\n';
				frame_count = 0;
				duration -= duration;
			}
			value += duration_cast<milliseconds>(delta).count();
			angle = value / scale;
			if(angle > 2 * pi)
			{
				value = 0;
				angle = 0;
			}

			sin_v = std::sin(angle);
			cos_v = std::cos(angle);

			sample.vertices()[0].pos = {sin_v, cos_v * a};
			sample.vertices()[1].pos = {-cos_v, sin_v * a};
			sample.vertices()[2].pos = {-sin_v, -cos_v * a};
			sample.vertices()[3].pos = {cos_v, -sin_v * a};
			sample.flush_vertices_to_memory();
			frame_count++;
			last_time = timer();
		};

		sample.initialize();
		sample.flush_indices_to_memory();

		while(sample.render(buffer_record));
	}
	catch(const std::exception & e) { std::cerr << e.what(); }
	return 0;
}