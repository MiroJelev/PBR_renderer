#version 330 core
out vec4 frag_color;
in vec3 normal;
in vec3 fragment_position;


//uniform vec3 light_color;
uniform vec3 view_position;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;
in vec2 texture_coordinates;

struct Light{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;


void main(){
	//ambient
	vec3 ambient = vec3(texture(material.diffuse, texture_coordinates)) * light.ambient;
	
	//normal and vector to light
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(-light.direction);
	//diffusion
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, texture_coordinates)) * light.diffuse;
	
	//specular
	//float specular_strength = 0.5f;
	vec3 view_direction = normalize(view_position - fragment_position);
	vec3 reflection_direction = reflect(-light_direction, norm);
	
	float spec = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
	vec3 specular = vec3(texture(material.specular, texture_coordinates)) * spec * light.specular;
	
	vec3 result = ambient + diffuse + specular;
	frag_color = vec4(result, 1.0);
}
