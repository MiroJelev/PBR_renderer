#include "common.h"
/*
class Vertex_array{
public:
	Vertex_array(const unsigned int size);
	~Vertex_array();
	void bind(const unsigned int& index) const;
	
private:
	std::vector<unsigned int> vao_list;
};
*/

class Vertex_array{
public:
	Vertex_array();
	~Vertex_array();
	unsigned int get_ID();
	void bind();
	void unbind();
	
private:
	unsigned int ID;
};
