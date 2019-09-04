#include <CustomizedModelLoading/BlendShapeMesh.h>
#include <DatCustomUtils/CustomIndexer.hpp>
#include <CustomizedModelLoading/TextureManager.hpp>

using namespace DatCustom;

namespace DatCustom{
	namespace Graphics{
		ColoredBlendShapeMesh::ColoredBlendShapeMesh(){
		}
		ColoredBlendShapeMesh::~ColoredBlendShapeMesh(){
			// printf("Deleting ColoredBlendShape\n");
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			glDeleteVertexArrays(1, &VAO);
		}
		ColoredBlendShapeMesh::ColoredBlendShapeMesh(std::vector<std::vector<ColoredVertex>> vertices,
				std::vector<GLuint> indices, bool bHasDiffColors): bHasDistincColors(bHasDiffColors){
			this->vertices = vertices;
			this->indices = indices;
			setupMesh();
		}
		ColoredBlendShapeMesh::ColoredBlendShapeMesh(std::vector<CTM::Mesh>& listCtmMeshes, bool bHasDiffColors):
			bHasDistincColors(bHasDiffColors)
		{
			this->indices.resize(listCtmMeshes[0].mIndices.size());
			printf("Pushing all vertices into internal format....\n");
			// Scaling is not possible anymore since we will have to consider multiple meshes
			// TODO: Research on this
			for (size_t i = 0; i < listCtmMeshes.size(); i++){
				printf("Allocating mem for %d%s mesh....\n", (int)i, i == 1 ? "st" : "th");
				this->vertices.push_back(std::vector<ColoredVertex>(listCtmMeshes[i].mVertices.size()));
				printf("Assigning each vertex for %d%s mesh....\n", (int)i, i == 1 ? "st" : "th");
				for (size_t j = 0; j < listCtmMeshes[i].mVertices.size(); j++){
					this->vertices[i][j].Position = {
						listCtmMeshes[i].mVertices[j].x,
						listCtmMeshes[i].mVertices[j].y,
						listCtmMeshes[i].mVertices[j].z
					};
					this->vertices[i][j].Normal = {
						listCtmMeshes[i].mNormals[j].x,
						listCtmMeshes[i].mNormals[j].y,
						listCtmMeshes[i].mNormals[j].z
					};
					if (listCtmMeshes[i].HasColors()){
						this->vertices[i][j].Colors = {
							listCtmMeshes[i].mColors[j].x,
							listCtmMeshes[i].mColors[j].y,
							listCtmMeshes[i].mColors[j].z,
							listCtmMeshes[i].mColors[j].w
						};
					}
					else{
						this->vertices[i][j].Colors = {0.5, 0.5, 0.5, 1.0};
					}
				}
			}
			printf("Copying the indices....\n");
			// And Copy indices as well
			for (size_t i = 0; i < listCtmMeshes[0].mIndices.size(); i++){
				this->indices[i] = listCtmMeshes[0].mIndices[i];
			}
			printf("Indices size: %d\n", (int)this->indices.size());
			setupMesh();
		}

		void ColoredBlendShapeMesh::setupMesh(){
			// print("Generating VAO, VBOs and EBO)
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// Let's upload vertices on all buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(
					GL_ARRAY_BUFFER,
				 	vertices[0].size() * sizeof(ColoredVertex)*vertices.size(),
				 	NULL,
					GL_STATIC_DRAW);
			for (size_t i = 0; i < vertices.size(); i++){
				glBufferSubData(GL_ARRAY_BUFFER, vertices[0].size() * sizeof(ColoredVertex) * i,
												vertices[i].size() * sizeof(ColoredVertex), &vertices[i][0]);

			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
					&indices[0], GL_STATIC_DRAW);

			glBindVertexArray(0);
		}

		void ColoredBlendShapeMesh::Draw(Shader shader, std::vector<unsigned int> blendShapeIndices,
				std::vector<float> weights){
			shader.setBool("hasTexture", false);
			glBindVertexArray(this->VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			// Only need to rebind appropriate VBOs
			size_t vert_sizes[] = {3, 3, 4};

			// set number of active blendshape
			shader.setInt("nBlendShape", blendShapeIndices.size());
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			for (auto elem: Iterator::index(blendShapeIndices)){
				auto i = elem.first; auto index = elem.second;
				std::vector<int> vertexAttrib = {0, 1, 2};
				if (i != 0){
					if (bHasDistincColors){
						int newVertexAttrib[] = {3*((int)i), 3*((int)i)+1, 3*((int)i)+2};
						vertexAttrib.assign(newVertexAttrib, newVertexAttrib+3);
					}
					else{
						int newVertexAttrib[] = {3+2*((int)i-1), 3+2*((int)i-1)+1};
						vertexAttrib.assign(newVertexAttrib, newVertexAttrib+2);
						vertexAttrib.pop_back();
					}
				}
				for (auto attribElem: Iterator::index(vertexAttrib)){
					glEnableVertexAttribArray(attribElem.second);
					glVertexAttribPointer(
							attribElem.second,
							vert_sizes[attribElem.first], GL_FLOAT, GL_FALSE, sizeof(ColoredVertex),
							(void*)(sizeof(ColoredVertex)*index*vertices[0].size()+attribElem.first*offsetof(ColoredVertex, Normal)));
				}
				// set shader uniform as well
				shader.setFloat("weights[" + std::to_string(i) + "]", weights[i]);
			}
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		TexturedBlendShapeMesh::TexturedBlendShapeMesh(){
		}
		TexturedBlendShapeMesh::~TexturedBlendShapeMesh(){
			printf("Deleting TexturedBlendShape\n");
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			for (auto texture: textures){
				glDeleteTextures(1, &texture.id);
			}
			glDeleteVertexArrays(1, &VAO);
		}

		TexturedBlendShapeMesh::TexturedBlendShapeMesh(
				std::vector<std::vector<TexturedVertex>> vertices,
				std::vector<GLuint> indices, std::vector<Texture> textures){
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			setupMesh();
		}
		TexturedBlendShapeMesh::TexturedBlendShapeMesh(std::vector<CTM::Mesh>& listCtmMeshes)
		{
			this->indices.resize(listCtmMeshes[0].mIndices.size());
			printf("Pushing all vertices into internal format....\n");
			// Scaling is not possible anymore since we will have to consider multiple meshes
			// TODO: Research on this
			for (size_t i = 0; i < listCtmMeshes.size(); i++){
				printf("Allocating mem for %d%s mesh....\n", (int)i, i == 1 ? "st" : "th");
				this->vertices.push_back(std::vector<TexturedVertex>(listCtmMeshes[i].mVertices.size()));
				printf("Assigning each vertex for %d%s mesh....\n", (int)i, i == 1 ? "st" : "th");
				for (size_t j = 0; j < listCtmMeshes[i].mVertices.size(); j++){
					this->vertices[i][j].Position = {
						listCtmMeshes[i].mVertices[j].x,
						listCtmMeshes[i].mVertices[j].y,
						listCtmMeshes[i].mVertices[j].z
					};
					this->vertices[i][j].Normal = {
						listCtmMeshes[i].mNormals[j].x,
						listCtmMeshes[i].mNormals[j].y,
						listCtmMeshes[i].mNormals[j].z
					};
					this->vertices[i][j].TexCoords = {
						listCtmMeshes[i].mTexCoords[j].u,
						listCtmMeshes[i].mTexCoords[j].v,
					};
				}
			printf("Texture File Name: %s\n", listCtmMeshes[i].mTexFileName.c_str());
				textures.push_back(*TextureManager::instance().loadTextureFromFile(listCtmMeshes[i].mTexFileName.c_str(), "texture_diffuse"));
				textures.push_back(*TextureManager::instance().loadTextureFromFile(listCtmMeshes[i].mTexFileName.c_str(), "texture_specular"));
			}
			printf("Copying the indices....\n");
			// And Copy indices as well
			for (size_t i = 0; i < listCtmMeshes[0].mIndices.size(); i++){
				this->indices[i] = listCtmMeshes[0].mIndices[i];
			}
			printf("Indices size: %d\n", (int)this->indices.size());
			setupMesh();
		}

		void TexturedBlendShapeMesh::setupMesh(){
			// print("Generating VAO, VBOs and EBO)
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// Let's upload vertices on all buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// First are Vertices, Normals, Textures
			// Then, only Vertices and Normals
			// TODO: further optimize memory instead of just using full texturedvertices
			glBufferData(
					GL_ARRAY_BUFFER,
				 	vertices[0].size() * sizeof(TexturedVertex)*vertices.size(),
				 	NULL,
					GL_STATIC_DRAW);
			for (size_t i = 0; i < vertices.size(); i++){
				glBufferSubData(GL_ARRAY_BUFFER, vertices[0].size() * sizeof(TexturedVertex) * i,
												vertices[i].size() * sizeof(TexturedVertex), &vertices[i][0]);

			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
					&indices[0], GL_STATIC_DRAW);

			glBindVertexArray(0);
		}

		void TexturedBlendShapeMesh::Draw(Shader shader, std::vector<unsigned int> blendShapeIndices,
				std::vector<float> weights){
			shader.setBool("hasTexture", true);
			glBindVertexArray(this->VAO);
			GLuint diffuseNr = 0;
			GLuint specularNr = 0;
			for (GLuint i = 0; i < textures.size(); i++){
				glActiveTexture(GL_TEXTURE0+i); // Active proper texture uint before binding
				// retrieve texture number ( the N in diffuse_tetureN )
				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++); 	// Transfer unsigned int to stream
				else if(name == "texture_specular")
					number = std::to_string(specularNr++); 	// Transfer unsigned int to stream
				shader.setInt(("blend_" + name + "[" + number + "]").c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			// Only need to rebind appropriate VBOs
			size_t vert_sizes[] = {3, 3, 2};

			// set number of active blendshape
			shader.setInt("nBlendShape", blendShapeIndices.size());
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			for (auto elem: Iterator::index(blendShapeIndices)){
				auto i = elem.first; auto index = elem.second;
				std::vector<int> vertexAttrib = {0, 1, 2};
				std::vector<size_t> offsets = {0, offsetof(TexturedVertex, Normal), offsetof(TexturedVertex, TexCoords)};
				if (i != 0){
					int newVertexAttrib[] = {3+2*((int)i-1), 3+2*((int)i-1)+1};
					vertexAttrib.assign(newVertexAttrib, newVertexAttrib+2);
					vertexAttrib.pop_back();
				}
				for (auto attribElem: Iterator::index(vertexAttrib)){
					glEnableVertexAttribArray(attribElem.second);
					glVertexAttribPointer(
							attribElem.second,
							vert_sizes[attribElem.first], GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
							(void*)(sizeof(TexturedVertex)*index*vertices[0].size()+offsets[attribElem.first]));
				}
				// set shader uniform as well
				shader.setFloat("weights[" + std::to_string(i) + "]", weights[i]);
			}
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	}
}
