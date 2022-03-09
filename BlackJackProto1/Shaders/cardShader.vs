#version 330 core
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;

uniform mat4 transformation;
uniform vec2 position;
uniform vec2 size;
out vec2 TexCoord;

void main()
{
	TexCoord = vec2((iTexCoord * size) + position);
//	TexCoord = iTexCoord;
	gl_Position = transformation * vec4(iPos, 1.0);
}