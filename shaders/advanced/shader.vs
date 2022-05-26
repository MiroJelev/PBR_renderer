#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 normal;
	vec3 fragment_position;
	vec2 texture_coordinates;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.normal = vec3(model * vec4(aNormal, 0.0f));
	vs_out.fragment_position = vec3(model * vec4(aPos, 1.0));
	vs_out.texture_coordinates = aTextureCoordinates;
}
