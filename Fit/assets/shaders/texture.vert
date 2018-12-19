#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;
layout(location = 4) in vec3 cF;

uniform mat4 MVP;

out vec2 vUV;
out vec3 colorFactors;

void main()
{
    vUV = uv;
	colorFactors=cF;
    gl_Position = MVP * vec4(position, 1.0f);
}