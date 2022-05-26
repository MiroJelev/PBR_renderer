#include "vertex_array.h"

/*
Vertex_array::Vertex_array(const unsigned int size) : vao_list(size){
	glGenVertexArrays(size, vao_list.data());
}
Vertex_array::~Vertex_array(){
	glDeleteVertexArrays(vao_list.size(), vao_list.data());
}
void Vertex_array::bind(const unsigned int& index) const{
	glBindVertexArray(vao_list[index]);
}
*/



Vertex_array::Vertex_array(){
	glGenVertexArrays(1, &ID);
}
Vertex_array::~Vertex_array(){
	glDeleteVertexArrays(1, &ID);
}
unsigned int Vertex_array::get_ID(){
	return ID;
}
void Vertex_array::bind(){
	glBindVertexArray(ID);
}
void Vertex_array::unbind(){
	glBindVertexArray(0);
}
