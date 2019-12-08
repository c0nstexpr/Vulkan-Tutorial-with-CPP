#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform transform { mat4 mat; }tf;

layout(location = 0) in vec3 in_position;

layout(location = 0) out vec3 frag_color;

layout(location = 1) in vec3 in_color;

layout(location = 1) out vec2 frag_texture;

layout(location = 2) in vec2 in_texture;

void main() {
	gl_Position = tf.mat * vec4(in_position, 1.0);
	frag_color = in_color;
	frag_texture = in_texture;
}