#version 330 core
out vec4 fragment_color;

in vec2 texture_coordinates;

uniform sampler2D texture1;

void main(){
	fragment_color = texture(texture1, texture_coordinates);
}
