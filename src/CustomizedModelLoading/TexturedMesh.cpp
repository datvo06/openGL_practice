#include <CustomizedModelLoading/Mesh.h>
#include <CustomizedModelLoading/TextureManager.hpp>

namespace DatCustom{
	namespace Graphics{
		TexturedMesh::TexturedMesh(std::vector<TexturedVertex> vertices, std::vector<GLuint> indices,
				std::vector<Texture> textures){
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}


		TexturedMesh::TexturedMesh(){
		}


		TexturedMesh::TexturedMesh(CTM::Mesh& ctmMesh){
			// Let's create textured vertices
			this->vertices.resize(ctmMesh.mVertices.size());
			this->indices.resize(ctmMesh.mIndices.size());
			for (size_t i = 0; i < ctmMesh.mVertices.size(); i++){
				this->vertices[i].Position = {ctmMesh.mVertices[i].x,
					ctmMesh.mVertices[i].y,
					ctmMesh.mVertices[i].z
				};
				this->vertices[i].Normal = {ctmMesh.mNormals[i].x,
					ctmMesh.mNormals[i].y,
					ctmMesh.mNormals[i].z
				};
				this->vertices[i].TexCoords = {ctmMesh.mTexCoords[i].u,
					ctmMesh.mTexCoords[i].v,
				};
			}
			// And Copy indices as well
			for (size_t i = 0; i < ctmMesh.mIndices.size(); i++){
				this->indices[i] = ctmMesh.mIndices[i];
			}
			textures.push_back(*TextureManager::instance().loadTextureFromFile(ctmMesh.mTexFileName.c_str(), "texture_diffuse"));
			textures.push_back(*TextureManager::instance().loadTextureFromFile(ctmMesh.mTexFileName.c_str(), "texture_specular"));
			setupMesh();
		}


		void TexturedMesh::setupMesh()
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexturedVertex), &vertices[0],
					GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
					&indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, Normal));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, TexCoords));
			glBindVertexArray(0);
		}


		void TexturedMesh::Draw(Shader shader){
			GLuint diffuseNr = 1;
			GLuint specularNr = 1;
			for (GLuint i = 0; i < textures.size(); i++){
				glActiveTexture(GL_TEXTURE0+i); // Active proper texture uint before binding
				// retrieve texture number ( the N in diffuse_tetureN )
				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++); 	// Transfer unsigned int to stream
				else if(name == "texture_specular")
					number = std::to_string(specularNr++); 	// Transfer unsigned int to stream
				shader.setInt(("material." + name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
			// draw mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);
		}
	}
}
