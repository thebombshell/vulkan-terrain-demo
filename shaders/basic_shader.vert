#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0)in vec3 attribute_position;
layout(location = 1)in vec4 attribute_color;
layout(location = 2)in vec3 attribute_normal;
layout(location = 0)out vec4 varying_color;
layout(location = 1)out vec3 varying_normal;

void main() {
	
	gl_Position = vec4(attribute_position, 1.0);
	varying_color = vec4(attribute_color.rgb, 1.0);
	varying_normal = attribute_normal;
}
