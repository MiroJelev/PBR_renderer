#ifndef SHADER_H
#define SHADER_H
#include "common.h"



class Shader{
public:
	unsigned int ID;	//the program ID
	//constructor reads and build the shader
	Shader(const char* vertex_path, const char* fragment_path);
	~Shader();
	//use/activate the shader
	void use();
	//utility uniform functions
	void set_bool(const std::string &name, bool value) const;
	void set_int(const std::string &name, int value) const;
	void set_float(const std::string &name, float value) const;
	void set_vec2(const std::string &name, glm::vec2 vector) const;
	void set_vec3(const std::string &name, glm::vec3 vector) const;
	void set_mat4(const std::string &name, const glm::mat4& matrix)const;
};

#endif
