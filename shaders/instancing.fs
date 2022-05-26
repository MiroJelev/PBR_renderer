#version 330 core
out vec4 fragment_color;

in vec3 f_color;

void main(){
	fragment_color = vec4(f_color, 1.0);
}
