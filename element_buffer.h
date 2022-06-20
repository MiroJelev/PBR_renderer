#include "common.h"

class Element_buffer{
public:
	Element_buffer(const unsigned int size);
	~Element_buffer();
	void bind(const unsigned int& index) const;
	//insert_data();
	
private:
	std::vector<unsigned int> ebo_list;
};
