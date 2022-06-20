#include "common.h"

class Framebuffer{
public:
	Framebuffer();
	~Framebuffer();
	void bind();
	void unbind();
	void attach_texture(const unsigned int& texture);
	void attach_depthmap(const unsigned int& depth_map);
	void attach_render_buffer(const unsigned int& render_buffer, GLenum attachment = GL_DEPTH_STENCIL_ATTACHMENT);
	void remove_color_buffer();
private:
	unsigned int ID;
};
