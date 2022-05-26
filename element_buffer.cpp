#include "element_buffer.h"

Element_buffer::Element_buffer(const unsigned int size) : ebo_list(size){
	glGenBuffers(size, ebo_list.data());
}
Element_buffer::~Element_buffer(){
	glDeleteBuffers(ebo_list.size(), ebo_list.data());
}
void Element_buffer::bind(const unsigned int& index) const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_list[index]);
}
/*
void Element_buffer::insert_data(){
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
*/
