#ifndef __CUSTOMIZED_MESH__
#define __CUSTOMIZED_MESH__
#include <CustomizedModelLoading/Texture.h>
#include <CustomizedModelLoading/Vertex.h>
#include <Shader.h>
#include <glad/glad.h>
#include <vector>

class Mesh{
	public:
		/* Mesh Data */
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		/* Function */
		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
		void Draw(Shader shader);
	private:
		/* Render Data */
		GLuint VAO, VBO, EBO;
		/* Functions */
		void setupMesh();
};
#endif
