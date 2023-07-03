#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "common.h"
#include "mesh.h"
#include "texture.h"


unsigned int texture_from_file(const char *path, const std::string &directory, bool gamma = false);

class Model{
public:
	Model(const std::string& path, const std::vector<std::string>& helmet_textures_paths);
	Model(const std::string& path);
	void draw(Shader &shader);
	
	std::vector<Mesh> meshes;
	std::vector<Texture2D> model_textures{5};
	glm::mat4 model;

	//std::vector<Texture> textures_loaded; //assimp
private:
	
	std::string directory;
	
	void load_model(const std::string& path);
	void process_node(aiNode *node, const aiScene *scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
	void load_textures(const std::vector<std::string>& textures_paths);

	//std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name); //assimp
	
};

