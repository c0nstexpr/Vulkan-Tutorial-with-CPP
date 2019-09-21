#include"vulkan_triangle_sample.h"

int main()
{
    try
    {
        vulkan_triangle_sample sample;
        sample.run();
    }
    catch(const std::exception e)
    {
        std::cerr << e.what();
    }
    return 0;
}