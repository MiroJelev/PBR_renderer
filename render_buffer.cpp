#include "render_buffer.h"

Render_buffer::Render_buffer(){
	glGenRenderbuffers(1, &ID);
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scr_w, scr_h);
	unbind();
}
Render_buffer::~Render_buffer(){
	glDeleteRenderbuffers(1, &ID);
}
unsigned int Render_buffer::get_ID(){
	return ID;
}
void Render_buffer::bind(){
	glBindRenderbuffer(GL_RENDERBUFFER, ID);
}
void Render_buffer::unbind(){
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
