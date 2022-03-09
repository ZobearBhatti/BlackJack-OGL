#version 330 core

layout(location = 0) in vec3 iPos;

uniform mat4 transformation;

void main()
{
	gl_Position = transformation * vec4(iPos, 1.0);
}