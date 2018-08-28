#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 varying_color;
layout(location = 1) in vec3 varying_normal;
layout(location = 0) out vec4 out_color;

void main() {
	
	out_color = vec4(varying_color.rgb, 1.0);
}