#include <CustomizedModelLoading/Mesh.h>


namespace DatCustom{
	namespace Graphics{
		ColoredMesh::ColoredMesh(){
		}

		ColoredMesh::ColoredMesh(std::vector<ColoredVertex> vertices, std::vector<GLuint> indices){
			this->vertices = vertices;
			this->indices = indices;
			setupMesh();
		}

		ColoredMesh::ColoredMesh(CTM::Mesh& ctmMesh){
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
				this->vertices[i].Colors = {ctmMesh.mColors[i].x,
					ctmMesh.mColors[i].y,
					ctmMesh.mColors[i].z,
					ctmMesh.mColors[i].w,
				};
			}
			// And Copy indices as well
			for (size_t i = 0; i < ctmMesh.mIndices.size(); i++){
				this->indices[i] = ctmMesh.mIndices[i];
			}

			setupMesh();
		}
		void ColoredMesh::setupMesh()
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ColoredVertex), &vertices[0],
					GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
					&indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, Normal));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, Colors));
			glBindVertexArray(0);
		}

		void ColoredMesh::Draw(Shader shader){
			shader.setBool("hasTexture", false);
			// draw mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	}
};
