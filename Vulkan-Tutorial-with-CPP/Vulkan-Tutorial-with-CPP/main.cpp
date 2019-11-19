#include "vulkan_sample.h"
#include <thread>

using namespace vulkan;
using namespace std::chrono;
using std::thread;
int f();

int main()
{
    try
    {
        vulkan_sample sample;
        sample.initialize();
        while(sample.render([&sample](const auto)
            {
                const auto time = duration_cast<milliseconds>(
                    time::steady_clock_timer().time_since_epoch()
                    ).count() / 1000.0;
                const auto& model = rotate<float>(mat4{1}, static_cast<float>(time * 90_deg), vec3{0, 0, 1});
                static const auto& view = lookAt(vec3{2, 2, 2}, vec3{0, 0, 0}, vec3{0, 0, 1});
                static const auto& extent = sample.swap_chain_create_info().imageExtent;
                auto&& proj = perspective<float>(static_cast<float>(45_deg), extent.width /
                    static_cast<float>(extent.height), 0.1f, 10);

                proj[1][1] *= -1;
                sample.set_transform({proj * view * model});
                sample.flush_transform_to_memory();
            }));
    }
    catch(const std::exception & e) { std::cerr << e.what(); }
    return 0;
}