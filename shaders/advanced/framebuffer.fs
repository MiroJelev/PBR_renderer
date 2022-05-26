#version 330 core
out vec4 fragment_color;

in vec2 texture_coordinates;

uniform sampler2D screen_texture;

void main(){
	//inverted colors
	//fragment_color = vec4(vec3(1.0 - texture(screen_texture, texture_coordinates)), 1.0f);
	//grayscale
	fragment_color = texture(screen_texture, texture_coordinates);
	/*float average = 0.2126 * fragment_color.r +
					0.7152 * fragment_color.g + 
					0.0722 * fragment_color.b;
	fragment_color = vec4(average, average, average, 1.0f);*/
	
}
