#pragma once
#include "utility/utility.h"

using namespace utility;

class glm_camera
{
public:
    //Projection matrix variables
    float aspect_ratio= 4.0f / 3; // aspect ratio
    // angle in radians
    float angle =
#ifdef GLM_FORCE_RADIANS
    static_cast<float>(45_deg)
#else
    45.0f
#endif
    ;
    float near = 0.1f; // near clipping distance
    float far = 100; // far clipping distance

    //View matrix variables
    vec3 pos; // position of the camera
    vec3 center; // focal point of the camera
    vec3 up; // the up direction for the camera

    constexpr glm_camera() noexcept = default;

    mat4 get_view_mat() const noexcept { return lookAt(pos, center, up); }

    mat4 get_proj_mat() const noexcept { return perspective(angle, aspect_ratio, near, far); }
};
