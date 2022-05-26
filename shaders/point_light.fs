#version 330 core
out vec4 frag_color;
in vec3 normal;
in vec3 fragment_position;
in vec2 texture_coordinates;


struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

uniform vec3 view_position;
uniform Material material;
uniform Light light;


void main(){
	//ambient
	vec3 ambient = vec3(texture(material.diffuse, texture_coordinates)) * light.ambient;
	
	//normal and vector to light
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light.position - fragment_position);
	//diffusion
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, texture_coordinates)) * light.diffuse;
	
	//specular
	//float specular_strength = 0.5f;
	vec3 view_direction = normalize(view_position - fragment_position);
	vec3 reflection_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
	vec3 specular = vec3(texture(material.specular, texture_coordinates)) * spec * light.specular;
	
	//attenuation
	float distance = length(light.position - fragment_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	vec3 result = ambient + diffuse + specular;
	frag_color = vec4(result, 1.0);
}
