#version 330 core

in vec2 vUV;

uniform sampler2D tex;

uniform vec3 factors1;
uniform vec3 factors2;
uniform vec3 factors3;

out vec4 color;

void main()
{
    vec4 frag_color = texture(tex, vUV);
 //   float gray_color = (frag_color.r + frag_color.g + frag_color.b) / 3.0; // Take the average of the colors, there are better techniques but this is sufficient
    float outputR = factors1.x* frag_color.r + factors1.y*frag_color.g + factors1.z*frag_color.b;
    float outputG = factors2.x* frag_color.r + factors2.y*frag_color.g + factors2.z*frag_color.b;
    float outputB = factors3.x* frag_color.r + factors3.y*frag_color.g + factors3.z*frag_color.b;
    color = vec4(outputR, outputG, outputB, 1.0);
}