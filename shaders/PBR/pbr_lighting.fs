#version 330 core
out vec4 FragColor;
in vec2 texture_coordinates;
in vec3 world_position;
in vec3 normal;

uniform vec3 camera_position;

//material parameters
uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;

//lights
uniform vec3 light_positions[4];
uniform vec3 light_colors[4];


const float PI = 3.14159265359;

vec3 get_normal_from_map()
{
    vec3 tangent_normal = texture(normal_map, texture_coordinates).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(world_position);
    vec3 Q2  = dFdy(world_position);
    vec2 st1 = dFdx(texture_coordinates);
    vec2 st2 = dFdy(texture_coordinates);

    vec3 N   = normalize(normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangent_normal);
}

float distribution_GGX(vec3 N, vec3 H, float roughness){
	float a		 = roughness * roughness;
	float a2	 = a * a;
	float NdotH	 = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;
	
	float nom 	= a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom 		= PI * denom * denom;
	
	return nom / denom;
}

float geometry_schlick_GGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = geometry_schlick_GGX(NdotV, roughness);
    float ggx1  = geometry_schlick_GGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}




vec3 fresnel_schlick(float cos_theta, vec3 F0){
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cos_theta, 0.0, 1.0), 5.0);
}
void main(){
	vec3 albedo     = pow(texture(albedo_map, texture_coordinates).rgb, vec3(2.2));
    //vec3 n_map     = texture(normal_map, texture_coordinates).rgb;
   // n_map = normalize(n_map * 2.0 - 1.0);
    float metallic  = texture(metallic_map, texture_coordinates).r;
    float roughness = texture(roughness_map, texture_coordinates).r;
    float ao        = texture(ao_map, texture_coordinates).r;
	
	
	//vec3 N = normalize(n_map);
	vec3 N = get_normal_from_map();
	vec3 V = normalize(camera_position - world_position);
	
	vec3 F0 = vec3(0.04); //assumption that most dielectric surfaces look correct with constance 0.4
	F0		= mix(F0, albedo, metallic);
	
	
	//reflectance equation
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; i++){
		vec3 L = normalize(light_positions[i] - world_position);
		vec3 H = normalize(V + L);
		float distance 		= length(light_positions[i] - world_position);
		float attenuation	= 1.0 / (distance * distance);
		vec3  radiance		= light_colors[i] * attenuation;
		
		float NDF = distribution_GGX(N, H, roughness);
		float G   = geometry_smith(N, V, L, roughness);
		vec3 F	= fresnel_schlick(max(dot(H, V), 0.0), F0); //for metalic surfaces
	//////////////////
		//cook-torrance BRDF
		vec3 numerator		= NDF * G * F;
		float denominator	= 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; //0.0001 to prevent division by 0
		vec3 specular		= numerator / denominator;
		
		
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;	//how much light is reflected
		kD *= 1.0 - metallic;	//multipley by inverse metallness because only non metals have diffuse lighting
		
		//final outgoing reflectance value
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;	//result of integlral over omega
		
	}
	
	
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color 	 = ambient + Lo;
	
	//hdr tone mapping
	color = color / (color + vec3(1.0));
	//gamma correction
	//color = pow(color, vec3(1.0 / 2.2));
	
	
	FragColor = vec4(color, 1.0);
}
