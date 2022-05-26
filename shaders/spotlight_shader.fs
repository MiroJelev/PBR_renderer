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
	vec3 direction;
	float cut_off;
	float outer_cut_off;
	
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
	//normal and vector to light
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light.position - fragment_position);
	
	//diffusion shading
	float diff = max(dot(norm, light_direction), 0.0);
	
	//specular shading
	vec3 view_direction = normalize(view_position - fragment_position);
	vec3 reflection_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
	
	
	vec3 ambient = vec3(texture(material.diffuse, texture_coordinates)) * light.ambient;
	vec3 diffuse = diff * vec3(texture(material.diffuse, texture_coordinates)) * light.diffuse;
	vec3 specular = vec3(texture(material.specular, texture_coordinates)) * spec * light.specular;
	

	//light cuttof inner, outer and middle values
	float theta = dot(light_direction, normalize(-light.direction));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off)/ epsilon, 0.0, 1.0);
	
	diffuse *= intensity;
	specular *= intensity;
	
	
	//attenuation
	float distance = length(light.position - fragment_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	
	vec3 result = ambient + diffuse + specular;
	frag_color = vec4(result, 1.0);

}
