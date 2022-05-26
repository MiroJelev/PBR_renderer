#version 330 core
out vec4 frag_color;

uniform vec3 light_source_color;

void main(){
	frag_color = vec4(light_source_color, 1.0);
}
