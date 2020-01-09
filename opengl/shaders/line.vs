#version 330

uniform mat4 projection;
uniform mat4 view;

layout (location = 0) in vec3 pos;

void main(void) 
{
	gl_Position = projection * view * vec4(pos, 1.0);
}
