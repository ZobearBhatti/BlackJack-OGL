#version 330 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;

uniform mat4 transformation;
out vec3 Normal;
out vec3 fragPos;

void main()
{
	fragPos = iPos;
	Normal = iNormal;
	gl_Position = transformation * vec4(iPos, 1.0);
}