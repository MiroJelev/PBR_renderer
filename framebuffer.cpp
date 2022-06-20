#include "framebuffer.h"

Framebuffer::Framebuffer(){
	glGenFramebuffers(1, &ID);
}
Framebuffer::~Framebuffer(){
	glDeleteFramebuffers(1, &ID);
}
void Framebuffer::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}
void Framebuffer::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Framebuffer::attach_texture(const unsigned int& texture){
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	unbind();
}
void Framebuffer::attach_depthmap(const unsigned int& depth_map){
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
	unbind();
}
void Framebuffer::attach_render_buffer(const unsigned int& render_buffer, GLenum attachment){
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, render_buffer);
	unbind();
}
void Framebuffer::remove_color_buffer(){
	bind();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	unbind();
}
