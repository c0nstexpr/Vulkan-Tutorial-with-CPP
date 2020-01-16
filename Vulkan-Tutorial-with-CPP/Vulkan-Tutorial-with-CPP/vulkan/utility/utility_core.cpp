#include "utility_core.h"

namespace vulkan
{
    PFN_vkVoidFunction get_instance_proc_addr(const VkInstance instance, const char* name) noexcept
    {
        return DispatchLoaderStatic{}.vkGetInstanceProcAddr(instance, name);
    }
}