#include "vertex_buffer.h"

/*
Vertex_buffer::Vertex_buffer(const unsigned int size) : vbo_list(size){
	glGenBuffers(size, vbo_list.data());				//generate buffer with id VBO
}
Vertex_buffer::~Vertex_buffer(){
	glDeleteBuffers(vbo_list.size(), vbo_list.data());
}
void Vertex_buffer::bind(const unsigned int& index) const{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_list[index]);  //bind type(GL_ARRAY_BUFFER)(thats the VBO type) to the id
}
/*
void Vertex_buffer::insert_data(vertices_array& vertices){
	glBufferData(GL_ARRAY_BUFFER, 		//copy created
				sizeof(vertices), 		//user-def data
				vertices, 				//to buffer
				GL_STATIC_DRAW);	
}


*/

Vertex_buffer::Vertex_buffer(){
	glGenBuffers(1, &ID);				//generate buffer with id VBO
}
Vertex_buffer::~Vertex_buffer(){
	glDeleteBuffers(1, &ID);
}
void Vertex_buffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);  //bind type(GL_ARRAY_BUFFER)(thats the VBO type) to the id
}
void Vertex_buffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);  //bind type(GL_ARRAY_BUFFER)(thats the VBO type) to the id
}
