#include "model.h"

//STB image library
#include "stb_image.h"

Model::Model(const std::string& path, const std::vector<std::string>& textures_paths){
	model = glm::mat4(1.0f);
	load_model(path);
	load_textures(textures_paths);
}
Model::Model(const std::string& path){
	load_model(path);
}

void Model::draw(Shader &shader){

	for(int i = 0; i < model_textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 + i);
		model_textures[i].bind();
	}

	shader.set_mat4("model", model);

	for(Mesh x : meshes){
		x.draw(shader);
	}

	for(int i = 0; i < model_textures.size(); i++){
		model_textures[i].unbind();
	}
}


void Model::load_model(const std::string& path){
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
	
	//process material
	// if(mesh->mMaterialIndex >= 0){
	// 	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		
	// 	std::vector<Texture> albedo_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_albedo");//aiTextureType_BASE_COLOR
    //     textures.insert(textures.end(), albedo_maps.begin(), albedo_maps.end());
		
	// 	std::vector<Texture> normal_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");//aiTextureType_NORMALS
    //     textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
		
	// 	std::vector<Texture> metallic_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_metallic");//aiTextureType_METALNESS
    //     textures.insert(textures.end(), metallic_maps.begin(), metallic_maps.end());
		
	// 	std::vector<Texture> roughness_maps = load_material_textures(material, aiTextureType_SHININESS, "texture_roughness");//aiTextureType_DIFFUSE_ROUGHNESS
    //     textures.insert(textures.end(), roughness_maps.begin(), roughness_maps.end());
		
	// 	std::vector<Texture> ao_maps = load_material_textures(material, aiTextureType_AMBIENT, "texture_ao");//aiTextureType_LIGHTMAP
    //     textures.insert(textures.end(), ao_maps.begin(), ao_maps.end());

	// }
	
	return Mesh(vertices, indices, textures);
}


void Model::load_textures(const std::vector<std::string>& textures_paths){
	// model_textures.insert(model_textures.end(),{Texture2D{},Texture2D{},Texture2D{},Texture2D{},Texture2D{}});
	for(int i = 0; i < model_textures.size(); i++){
		model_textures[i].load_from_file(textures_paths[i], true);//, true
		model_textures[i].set_parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_REPEAT
		model_textures[i].set_parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		model_textures[i].set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		model_textures[i].set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

// std::vector<Texture> Model::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name){
// 	std::vector<Texture> textures;
// 	std::cout << "LOAD MATERIAL TEXTURES: " <<  mat->GetTextureCount(type) << std::endl;
// 	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
// 		aiString str;
// 		mat->GetTexture(type, i, &str);
// 		std::cout << "\t in for: " <<  str.C_Str() << " - " << type_name << std::endl;
// 		bool skip = false;
// 		for(unsigned int j = 0; j < textures_loaded.size(); j++){
// 			if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){
// 				// std::cout << "LOAD MATERIAL TEXTURES in FOR" << std::endl;
// 				textures.push_back(textures_loaded[j]);
// 				skip = true;
// 				break;
// 			}
// 		}
// 		if(!skip){ //if texture hasn't been loaded already, load it
// 			// std::cout << "LOAD MATERIAL TEXTURES" << std::endl;
// 			Texture texture;
// 			texture.id = texture_from_file(str.C_Str(), this->directory);
// 			texture.type = type_name;
// 			texture.path = str.C_Str();
// 			textures.push_back(texture);
// 			textures_loaded.push_back(texture); //add to loaded textures
// 		}
// 	}
// 	return textures;
// }



unsigned int texture_from_file(const char *path, const std::string &directory, bool gamma){
	std::cout << "FROM FILE LOAD" << std::endl;
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



