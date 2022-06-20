#include "common.h"
#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coordinates;
	/*	 // tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;
		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];*/
};


struct Texture{
	unsigned int id;
	std::string type;	//diffuse or specular texture
	std::string path; //we store the path of the texture to compare with other textures
};


class Mesh{
public:
	//mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	
	unsigned int vao;
	
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader &shader);
private:
	//render data
	unsigned int vbo, ebo;
	
	void setup_mesh();
};
