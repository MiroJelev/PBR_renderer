#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "common.h"
#include "mesh.h"


unsigned int texture_from_file(const char *path, const std::string &directory, bool gamma = false);

class Model{
public:
	Model(std::string path);
	void draw(Shader &shader);
	
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
private:

	
	std::string directory;
	
	void load_model(std::string path);
	void process_node(aiNode *node, const aiScene *scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
	
	
};

