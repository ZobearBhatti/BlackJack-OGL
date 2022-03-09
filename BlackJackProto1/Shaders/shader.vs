#version 330 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iColor;

uniform mat4 transformation;
out vec3 color;

void main()
{
	color = iColor;
	gl_Position = transformation * vec4(iPos, 1.0);
}