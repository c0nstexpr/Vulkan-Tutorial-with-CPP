#include "vulkan_sample.h"
#include "glm_camera.h"

using namespace vulkan;
using namespace std::chrono;

static glm_camera glm_camera{};

vulkan_sample sample;
        

static float horizontal_degree{};
static constexpr vec3 horizontal_axis{0,1,0};
static float vertical_degree{};
static constexpr vec3 vertical_axis{1,0,0};

void perform_camera_update(GLFWwindow* window, const int key,const float speed_up_ratio) {
    switch(key)
    {
    case GLFW_KEY_W: { glm_camera.pos.z += speed_up_ratio; }
    break;
    case GLFW_KEY_S: { glm_camera.pos.z -= speed_up_ratio; }
    break;
    case GLFW_KEY_A: { glm_camera.pos.x += speed_up_ratio; }
    break;
    case GLFW_KEY_D: { glm_camera.pos.x -= speed_up_ratio; }
    break;
    case GLFW_KEY_Q: { glm_camera.pos.y += speed_up_ratio; }
    break;
    case GLFW_KEY_E: { glm_camera.pos.y -= speed_up_ratio; }
    break;
    case GLFW_KEY_KP_4: { horizontal_degree += speed_up_ratio; }
    break;
    case GLFW_KEY_KP_6: { horizontal_degree -= speed_up_ratio; }
    break;
    case GLFW_KEY_KP_8: { vertical_degree += speed_up_ratio; }
    break;
    case GLFW_KEY_KP_2: { vertical_degree -= speed_up_ratio; }
    break;
    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    default: break;
    }

    glm_camera.center =  glm_camera.pos+vec3{0,0,5};

    static constexpr mat4 model{1};
    static const auto& proj = []
    {
        auto&& p = glm_camera.get_proj_mat();
        p[1][1] *= -1;
        return p;
    }();
    sample.flush_transform_to_memory();

    const auto& view = rotate(
        rotate(glm_camera.get_view_mat(), horizontal_degree, horizontal_axis),
        vertical_degree,
        vertical_axis
    );

    sample.set_transform({proj * view * model});
}

void key_callback(GLFWwindow* window, const int key, int, const int action, int)
{
    static auto speed_up_ratio = 1.0f;
    switch(action)
    {
    case GLFW_REPEAT: speed_up_ratio += 0.05f;
    case GLFW_PRESS: perform_camera_update(window, key,speed_up_ratio);
        break;
    case GLFW_RELEASE: speed_up_ratio = 0.05f;
        break;
    default: break;
    }
}



int main()
{
    try
    {
        sample.initialize();

        {
            const auto& extent = sample.swapchain_create_info().imageExtent;
            glm_camera.aspect_ratio = extent.width / static_cast<decltype(glm_camera.aspect_ratio)>(extent.height);
        }

        glm_camera.pos = {0,0,-5};
        glm_camera.up = {0,1,0};

        glfwSetKeyCallback(sample.get_window(),key_callback);

        while(sample.render());
    } catch(const std::exception& e) { std::cerr << e.what(); }
    return 0;
}