#include "common.h"

class Render_buffer{
public:
	Render_buffer();
	~Render_buffer();
	unsigned int get_ID();
	void bind();
	void unbind();
private:
	unsigned int ID;
};
