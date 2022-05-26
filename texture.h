#pragma once
#include "common.h"

class Texture2D{
public:
	//Texture2D(const unsigned int tex_count);
	//void bind(const unsigned int& index)const;
	
	Texture2D();
	~Texture2D();
	unsigned int get_ID();
	void bind()const;
	void unbind()const;
	void set_parameter(GLenum first, GLenum second);
	void load_from_file(const std::string& name,  bool flip  = false);
	void init_as_depthmap();
	void init_as_color_buffer();
	
private:
	//std::vector<unsigned int> texture_list;
	unsigned int ID;
};
