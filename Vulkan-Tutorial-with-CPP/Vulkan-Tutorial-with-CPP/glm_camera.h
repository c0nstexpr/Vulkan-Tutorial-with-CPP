#pragma once
#include "utility/utility.h"

using namespace utility;

class glm_camera
{
    inline static const auto& view = lookAt(vec3{}, {0, 0, -1}, {0, 1, 0});
public:
    //Projection matrix variables
    float aspect_ratio = 4.0f / 3; // aspect ratio
    // fov_angle in radians
    float fov_angle =
#ifdef GLM_FORCE_RADIANS
        static_cast<float>(45_deg)
#else
    45.0f
#endif
    ;
    float near = 0.1f; // near clipping distance
    float far = 100; // far clipping distance

    //View matrix variables
    qua<float> quaternion;
    vec3 pos; // position of the camera

    constexpr glm_camera() noexcept = default;

    mat4 get_view_mat() const noexcept { return translate(mat4_cast(quaternion) * view, pos); }

    mat4 get_proj_mat() const noexcept { return perspective(fov_angle, aspect_ratio, near, far); }
};
