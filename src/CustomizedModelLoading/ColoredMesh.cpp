#include <CustomizedModelLoading/Mesh.h>


namespace DatCustom{
	namespace Graphics{
		ColoredMesh::ColoredMesh(){
		}
		ColoredMesh::~ColoredMesh(){
			printf("Delete Here 0\n");
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			glDeleteVertexArrays(1, &VAO);
		}

		ColoredMesh::ColoredMesh(std::vector<ColoredVertex> vertices, std::vector<GLuint> indices){
			this->vertices = vertices;
			this->indices = indices;
			setupMesh();
		}

		ColoredMesh::ColoredMesh(CTM::Mesh& ctmMesh){
			CTM::Vector3 aMin, aMax;
			ctmMesh.BoundingBox(aMin, aMax);
			float max_x, max_y, max_z;
			max_x = aMax.x - aMin.x;
			max_y = aMax.y - aMin.y;
			max_z = aMax.z - aMin.z;
			printf("Min: %f, %f, %f\n", aMin.x, aMin.y, aMin.z);
			printf("Max: %f, %f, %f\n", aMax.x, aMax.y, aMax.z);
			float max = (max_x > max_y) ? max_x : max_y;
			max = (max > max_z) ? max : max_z;
			max = max/10;
			// Let's create textured vertices
			this->vertices.resize(ctmMesh.mVertices.size());
			this->indices.resize(ctmMesh.mIndices.size());
			for (size_t i = 0; i < ctmMesh.mVertices.size(); i++){
				this->vertices[i].Position = {(ctmMesh.mVertices[i].x - aMin.x)/max,
					(ctmMesh.mVertices[i].y - aMin.y)/max,
					(ctmMesh.mVertices[i].z - aMin.z)/max
				};
				this->vertices[i].Normal = {ctmMesh.mNormals[i].x,
					ctmMesh.mNormals[i].y,
					ctmMesh.mNormals[i].z
				};
				if (ctmMesh.HasColors()){
					this->vertices[i].Colors = {ctmMesh.mColors[i].x,
						ctmMesh.mColors[i].y,
						ctmMesh.mColors[i].z,
						ctmMesh.mColors[i].w,
					};
				}
				else{
					this->vertices[i].Colors = {0.5, 0.5, 0.5, 1.0};
				}
			}
			// And Copy indices as well
			for (size_t i = 0; i < ctmMesh.mIndices.size(); i++){
				this->indices[i] = ctmMesh.mIndices[i];
			}
			printf("Indices size: %d\n", (int)this->indices.size());

			setupMesh();
		}
		void ColoredMesh::setupMesh()
		{
			printf("Gen Here 0\n");
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			printf("Gen Here 1\n");
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ColoredVertex), &vertices[0],
					GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
					&indices[0], GL_STATIC_DRAW);

			printf("Gen Here 2\n");
			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, Normal));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)offsetof(ColoredVertex, Colors));
			printf("Gen Here 3\n");
			glBindVertexArray(0);
		}

		void ColoredMesh::Draw(Shader shader){
			shader.setBool("hasTexture", false);
			glDisable(GL_CULL_FACE);
			// draw mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glEnable(GL_CULL_FACE);
		}

	}
};
