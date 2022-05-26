#version 330 core
out vec4 frag_color;

in VS_OUT{
	in vec3 normal;
	in vec3 fragment_position;
	in vec2 texture_coordinates;
} fs_in;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Spot_light{
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

struct Directional_light{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Point_light{
	vec3 position;
	
	float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 view_position;
uniform Material material;
uniform Spot_light spot_light;
uniform Directional_light directional_light;
#define NR_POINT_LIGHTS 1
uniform Point_light point_lights[NR_POINT_LIGHTS];


vec3 calc_spot_light(Spot_light light, vec3 norm, vec3 view_direction, vec3 diff_color, vec3 spec_color);
vec3 calc_directional_light(Directional_light light, vec3 norm, vec3 view_direction, vec3 diff_color, vec3 spec_color);
vec3 calc_point_light(Point_light light, vec3 norm, vec3 view_direction, vec3 diff_color, vec3 spec_color);

void main(){
	//normal and vector to fragment from view
	vec3 n_normal = normalize(fs_in.normal);
	vec3 n_view_direction = normalize(view_position - fs_in.fragment_position);
	vec3 diffuse_color  = texture(material.diffuse, fs_in.texture_coordinates).rgb;
	vec3 specular_color = texture(material.specular, fs_in.texture_coordinates).rgb;
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	
	//result += calc_spot_light(spot_light, n_normal, n_view_direction, diffuse_color, specular_color);
	
	result += calc_directional_light(directional_light, n_normal, n_view_direction, diffuse_color, specular_color);
	
	//for(int i = 0; i < NR_POINT_LIGHTS; i++)
	//	result += calc_point_light(point_lights[i], n_normal, n_view_direction, diffuse_color, specular_color);
	
	
	frag_color = vec4(result, 1.0);
}

vec3 calc_spot_light(Spot_light light, vec3 norm, vec3 view_direction, vec3 diff_color, vec3 spec_color){
	vec3 light_direction = normalize(light.position - fs_in.fragment_position);
	//diffusion shading
	float diff = max(dot(norm, light_direction), 0.0);
	
	//specular shading
	vec3 halfway_direction = normalize(light_direction + view_direction);
	float spec = pow(max(dot(norm, halfway_direction), 0.0), material.shininess);
	
	vec3 ambient =  diff_color * light.ambient;
	vec3 diffuse =  diff_color * diff * light.diffuse;
	vec3 specular = spec_color * spec * light.specular;

	//light cuttof inner, outer and middle values
	float theta = dot(light_direction, normalize(-light.direction));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off)/ epsilon, 0.0, 1.0);
	
	diffuse *= intensity;
	specular *= intensity;
	
	
	//attenuation
	float distance = length(light.position - fs_in.fragment_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 calc_directional_light(Directional_light light, vec3 norm, vec3 view_direction, vec3 diff_color, vec3 spec_color){
	vec3 light_direction = normalize(-light.direction);
	//diffusion shading
	float diff = max(dot(norm, light_direction), 0.0);
	
	//specular shading
	vec3 halfway_direction = normalize(light_direction + view_direction);
	float spec = pow(max(dot(norm, halfway_direction), 0.0), material.shininess);
	
	vec3 ambient  = diff_color * light.ambient;
	vec3 diffuse  = diff_color * diff * light.diffuse;
	vec3 specular = spec_color * spec * light.specular;
	
	return (ambient + diffuse + specular);
}

vec3 calc_point_light(Point_light light, vec3 norm, vec3 view_direction, vec3 diff_color, vec3 spec_color){
	vec3 light_direction = normalize(light.position - fs_in.fragment_position);

	//diffusion shading
	float diff = max(dot(norm, light_direction), 0.0);
	
	//specular shading
	vec3 halfway_direction = normalize(light_direction + view_direction);
	float spec = pow(max(dot(norm, halfway_direction), 0.0), material.shininess);
	
	vec3 ambient = diff_color * light.ambient;
	vec3 diffuse = diff_color * diff * light.diffuse;
	vec3 specular = spec_color * spec * light.specular;
	
	//attenuation
	float distance = length(light.position - fs_in.fragment_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); //no gamma correction
	//float attenuation = 1.0 / distance; //with gamma correction but try different values 
	
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

