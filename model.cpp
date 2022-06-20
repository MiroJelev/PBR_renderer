#include "model.h"

//STB image library
#include "stb_image.h"

Model::Model(std::string path){
	load_model(path);
	
}

void Model::draw(Shader &shader){
	for(unsigned int i = 0; i < meshes.size(); i++){
		meshes[i].draw(shader);
	}
}


void Model::load_model(std::string path){
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	
	process_node(scene->mRootNode, scene);
}


void Model::process_node(aiNode *node, const aiScene *scene){
	//process all the node's meshes (if any)
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
	}
	//then do the same for each of its children
	for(unsigned int i = 0; i < node->mNumChildren; i++){
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene){
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	
	//process vertex positions, normal and texture coordinates
	for(unsigned int i = 0; i < mesh->mNumVertices; i++){
		Vertex vertex;
		//vertex positions
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		//normal
		if(mesh->HasNormals()){
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		//texture coordinates
		if(mesh->mTextureCoords[0])//does the mesh contain texture coordinates
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texture_coordinates = vec;
			
		}else{
			//std::cout << "no textures " << std::endl;
			vertex.texture_coordinates = glm::vec2(0.0f, 0.0f);
		}
		
		vertices.push_back(vertex);
	}
	
	//process indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++){
			indices.push_back(face.mIndices[j]);
		}
	}
	/*
	//process material
	if(mesh->mMaterialIndex >= 0){
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		
	///*	std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//	textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		
	//	std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	//*
	//	std::vector<Texture> normal_maps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    //    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
		
		std::vector<Texture> albedo_maps = load_material_textures(material, aiTextureType_BASE_COLOR, "texture_albedo");
        textures.insert(textures.end(), albedo_maps.begin(), albedo_maps.end());
		
		std::vector<Texture> normal_maps = load_material_textures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
		
		std::vector<Texture> metallic_maps = load_material_textures(material, aiTextureType_METALNESS, "texture_metallic");
        textures.insert(textures.end(), metallic_maps.begin(), metallic_maps.end());
		
		std::vector<Texture> roughness_maps = load_material_textures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
        textures.insert(textures.end(), roughness_maps.begin(), roughness_maps.end());
		
		std::vector<Texture> ao_maps = load_material_textures(material, aiTextureType_LIGHTMAP, "texture_ao");
        textures.insert(textures.end(), ao_maps.begin(), ao_maps.end());
		
		
		
		
		
		
		
		
		
		
	}
	*/
	return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name){
	std::vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for(unsigned int j = 0; j < textures_loaded.size(); j++){
			if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if(!skip){ //if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = texture_from_file(str.C_Str(), this->directory);
			texture.type = type_name;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); //add to loaded textures
		}
	}
	return textures;
}



unsigned int texture_from_file(const char *path, const std::string &directory, bool gamma){
	stbi_set_flip_vertically_on_load(true);
	
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	
	//std::cout << filename << std::endl;
	
	unsigned int texture_id;
	glGenTextures(1, &texture_id);
	
	int width, height, nr_components;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nr_components, 0);
	if(data){
		GLenum format;
		GLenum internal_format;
		if(nr_components == 1){
			format = GL_RED;
			internal_format = GL_RED;
		}else if(nr_components == 3){
			format = GL_RGB;
			internal_format = GL_SRGB;//gamma correction to remove comment this and change internal_format to format in glTexImage2D
		}else if(nr_components == 4){
			format = GL_RGBA;
			internal_format = GL_SRGB_ALPHA;//this too
		}
		
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		stbi_image_free(data);
		
	}else{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return texture_id;
}



