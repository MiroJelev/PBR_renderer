//STB image library
#include "stb_image.h"

#include "texture.h"

/*
Texture2D::Texture2D(const unsigned int tex_count) : texture_list(tex_count){
	glGenTextures(tex_count, texture_list.data());
	
}*/
/*
void Texture2D::bind(const unsigned int& index)const{
	glBindTexture(GL_TEXTURE_2D, texture_list[index]);
}*/

Texture2D::Texture2D(){
	glGenTextures(1, &ID);
}
Texture2D::~Texture2D(){
	glDeleteTextures(1, &ID);
}
unsigned int Texture2D::get_ID(){
	return ID;
}
void Texture2D::bind()const{
	glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture2D::unbind()const{
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::set_parameter(GLenum first, GLenum second){
	bind();
	glTexParameteri(GL_TEXTURE_2D, first, second);
	unbind();
}
void Texture2D::load_from_file(const std::string& name, bool flip){
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char *data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
	
	if(!data){
		std::cerr << "Failed to load texture" << std::endl;
		//throw exception
	}else{
		/*bind();
		int pos = name.find_last_of('.');
		if(name.substr(pos+1) == "jpg"){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
						 GL_RGB, GL_UNSIGNED_BYTE, data);
		}else if(name.substr(pos+1) == "png"){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
						 GL_RGBA, GL_UNSIGNED_BYTE, data);	//Alpha channel for png GL_RGBA
		}else{
			//throw invalid argument ?
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		unbind();*/
		GLenum format;
		//GLenum format2;
		if(nrChannels == 1){
			format = GL_RED;
		}else if(nrChannels == 3){
			format = GL_RGB;
		}else if(nrChannels == 4){
			format = GL_RGB;
			//format2 = GL_RGBA;
		}
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
		unbind();
		
	}
	stbi_image_free(data);
}
void Texture2D::init_as_depthmap(){
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	unbind();
}
void Texture2D::init_as_color_buffer(){
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
				 scr_w, scr_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	unbind();
}

