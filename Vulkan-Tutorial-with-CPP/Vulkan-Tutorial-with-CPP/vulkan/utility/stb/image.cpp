#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"

namespace vulkan::utility::stb
{
    image_format from_string(const string extension)
    {
        for(size_t i = 0; i < image_format_str.size(); ++i)
            if(image_format_str[i].data() == extension)
                return static_cast<image_format>(i);
        return image_format::unknown;
    }

    string to_string(const image_format format)
    {
        return image_format_str[static_cast<image_format_underlying_type>(format)].data();
    }
}