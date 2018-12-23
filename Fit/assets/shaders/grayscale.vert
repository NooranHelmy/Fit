#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;

out vec2 vUV;
out vec4 myPos;

void main()
{
    vUV = uv;
	//myPos=position;
    gl_Position = vec4(position, 1.0f);
	myPos=gl_Position;
}