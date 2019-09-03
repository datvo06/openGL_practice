#include <CustomizedModelLoading/BlendShapeMesh.h>
#include <DatCustomUtils/CustomIndexer.hpp>

using namespace DatCustom;

namespace DatCustom{
	namespace Graphics{
		ColoredBlendShapeMesh::ColoredBlendShapeMesh(){
		}
		ColoredBlendShapeMesh::~ColoredBlendShapeMesh(){
			// printf("Deleting ColoredBlendShape\n");
			glDeleteBuffers(VBOs.size(), &VBOs[0]);
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
			// Scaling is not possible anymore since we will have to consider multiple meshes
			// TODO: Research on this
			for (size_t i = 0; i < listCtmMeshes.size(); i++){
				this->vertices.push_back(std::vector<ColoredVertex>(listCtmMeshes[i].mVertices.size()));
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
			// And Copy indices as well
			for (size_t i = 0; i < listCtmMeshes[0].mIndices.size(); i++){
				this->indices[i] = listCtmMeshes[0].mIndices[i];
			}
			printf("Indices size: %d\n", (int)this->indices.size());
			setupMesh();
		}

		void ColoredBlendShapeMesh::setupMesh(){
			// print("Generating VAO, VBOs and EBO)
			VBOs.resize(this->vertices.size());
			glGenVertexArrays(1, &VAO);
			glGenBuffers((GLuint) this->vertices.size(), &VBOs[0]);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// Let's upload vertices on all buffers
			for (size_t i = 0; i < VBOs.size(); i++){
				glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
				glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(ColoredVertex), &vertices[i][0],
						GL_STATIC_DRAW);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
					&indices[0], GL_STATIC_DRAW);

			glBindVertexArray(0);
		}

		void ColoredBlendShapeMesh::Draw(Shader shader, std::vector<unsigned int> indices,
				std::vector<float> weights){
			shader.setBool("hasTexture", false);
			glBindVertexArray(this->VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			// Only need to rebind appropriate VBOs
			size_t vert_sizes[] = {3, 3, 4};

			// set number of active blendshape
			shader.setInt("nBlendShape", indices.size());
			for (auto elem: Iterator::index(indices)){
				auto i = elem.first; auto index = elem.second;
				glBindBuffer(GL_ARRAY_BUFFER, VBOs[index]);
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
							(void*)(attribElem.first*offsetof(ColoredVertex, Normal)));
				}
				// set shader uniform as well
				shader.setFloat("weights[" + std::to_string(i) + "]", weights[i]);
			}
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}
