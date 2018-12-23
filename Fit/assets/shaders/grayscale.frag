#version 330 core

in vec2 vUV;

uniform sampler2D tex;

out vec4 color;

void main()
{
    vec4 frag_color = texture(tex, vUV);
 //   float gray_color = (frag_color.r + frag_color.g + frag_color.b) / 3.0; // Take the average of the colors, there are better techniques but this is sufficient
    float outputR = 0.393* frag_color.r + 0.769*frag_color.g + 0.189*frag_color.b;
    float outputG = 0.349* frag_color.r + 0.686*frag_color.g + 0.189*frag_color.b;
    float outputB = 0.272* frag_color.r + 0.534*frag_color.g + 0.131*frag_color.b;
    color = vec4(outputR, outputG, outputB, 1.0);
}