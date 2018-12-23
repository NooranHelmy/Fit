#version 330 core

in vec2 vUV;
in vec4 myPos;

in Interpolators {
    vec3 view;
    vec3 normal;
} fs_in;

uniform sampler2D tex;

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};
uniform Material material;

struct DirectionalLight {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 direction;
};
uniform DirectionalLight light;

out vec4 color;

float diffuse(vec3 n, vec3 l){
    //Diffuse (Lambert) term computation: reflected light = cosine the light incidence angle on the surface
    //max(0, ..) is used since light shouldn't be negative
    return max(0, dot(n,l));
}

float specular(vec3 n, vec3 l, vec3 v, float shininess){
    //Phong Specular term computation
    return pow(max(0,  dot(v,reflect(-l, n))), shininess);
}

void main()
{
    vec4 frag_color = texture(tex, vUV);

	float distance = sqrt((0-myPos.x)*(0-myPos.x) + (4-myPos.y)*(4-myPos.y) + (-4-myPos.z)*(-4-myPos.z));
	
	float brightnessFactor=0.6;
	if(distance <10){
	
		brightnessFactor=1;
	}
	else if(distance <15)
	{
		brightnessFactor=0.8f;
	}
	else{
		brightnessFactor=1;
	}
	
	vec3 BF = {brightnessFactor,brightnessFactor,brightnessFactor};
	vec3 n = normalize(fs_in.normal);
    vec3 v = normalize(fs_in.view);
    vec3 colorLight = vec3(
        material.ambient*light.ambient + 
        material.diffuse*light.diffuse*diffuse(n, -light.direction) + 
        material.specular*light.specular*specular(n, -light.direction, v, material.shininess)
    );
	
	
	color = vec4(brightnessFactor*frag_color.r*colorLight.x, brightnessFactor*frag_color.g*colorLight.y, brightnessFactor*frag_color.b*colorLight.z, 1.0);

    //color = vec4(brightnessFactor*frag_color.r, brightnessFactor*frag_color.g, brightnessFactor*frag_color.b, 1.0);

	/* vec4 frag_color = texture(tex, vUV);

	float distance = sqrt((0-myPos.x)*(0-myPos.x) + (4-myPos.y)*(4-myPos.y) + (-4-myPos.z)*(-4-myPos.z));
	
	float brightnessFactor=0.6;
	if(distance <10){
	
		brightnessFactor=1;
	}
	else if(distance <30)
	{
		brightnessFactor=0.8f;
	}
	else{
		brightnessFactor=0.6f;
	}
	
    color = vec4(brightnessFactor*frag_color.r, brightnessFactor*frag_color.g, brightnessFactor*frag_color.b, 1.0); */
}