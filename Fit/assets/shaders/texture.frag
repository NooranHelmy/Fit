#version 330 core

in vec2 vUV;
in vec3 colorFactors;//TEST

uniform sampler2D tex;

out vec4 color;

void main()
{
    vec4 frag_color = texture(tex, vUV);

    color = vec4(frag_color.r, frag_color.g, frag_color.b, 1.0);
}