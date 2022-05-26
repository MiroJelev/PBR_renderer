#version 330 core

out vec4 frag_color;
//in vec3 our_color;
in vec3 our_pos;
in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float tex_mix;

void main(){
	frag_color = mix(texture(ourTexture1, TexCoord), 
					 texture(ourTexture2, vec2(-TexCoord.x, TexCoord.y)), tex_mix);//0.2//80% of the first and 20% of the second texture
}
