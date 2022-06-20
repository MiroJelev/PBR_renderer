#include "mesh.h"


Mesh::Mesh(std::vector<Vertex> n_vert, std::vector<unsigned int> n_indi, std::vector<Texture> n_text)
			:vertices(n_vert), indices(n_indi), textures(n_text){
	
	setup_mesh();
}


void Mesh::setup_mesh(){
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
				 vertices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
				 indices.data(), GL_STATIC_DRAW);
	
	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates));
	/*// vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		// ids
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

		// weights
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        */
	glBindVertexArray(0);
}


void Mesh::draw(Shader &shader){
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	
	/*unsigned int albedoNr = 1;
	unsigned int normalNr = 1;
	unsigned int metallicNr = 1;
	unsigned int roughnessNr = 1;
	unsigned int aoNr = 1;
	*/
	for(unsigned int i = 0; i < textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 + i);
		
		std::string number;
		std::string name = textures[i].type;
		/*if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++);
		*/
		/*if(name == "albedo_map")
			number = std::to_string(albedoNr++);
		else if(name == "normal_map")
			number = std::to_string(normalNr++);
		else if(name == "metallic_map")
			number = std::to_string(metallicNr++);
		else if(name == "roughness_map")
			number = std::to_string(roughnessNr++);
		else if(name == "ao_map")
			number = std::to_string(aoNr++);
		*/
		
		//shader.set_float(( name + number).c_str(), i); "material." +
		//glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
		glUniform1i(glGetUniformLocation(shader.ID, (name).c_str()), i);
		
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	//draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	glActiveTexture(GL_TEXTURE0);
}
