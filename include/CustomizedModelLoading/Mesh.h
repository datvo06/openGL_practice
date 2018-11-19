#ifndef __CUSTOMIZED_MESH__
#define __CUSTOMIZED_MESH__
#include <CustomizedModelLoading/Texture.h>
#include <CustomizedModelLoading/Vertex.h>
#include <Shader.h>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>


class Mesh{
	public:
		Mesh();
		virtual void Draw(Shader shader) = 0;
};


class glMesh: public Mesh{
	public:
		glMesh();
		virtual void Draw(Shader shader) = 0;
		virtual GLuint getVAO() = 0;
		virtual GLuint getVBO() = 0;
		virtual GLuint getEBO() = 0;
};

class TexturedMesh: public glMesh{
	public:
		/* Mesh Data */
		std::vector<TexturedVertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		std::vector<glm::vec4> mColors;

		bool hasColors();

		/* Function */
		TexturedMesh(std::vector<TexturedVertex> vertices,
			 	std::vector<GLuint> indices, std::vector<Texture> textures);
		TexturedMesh();
		void Draw(Shader shader);
		virtual GLuint getVAO() { return VAO; };
		virtual GLuint getVBO() { return VBO; };
		virtual GLuint getEBO() { return EBO; };
	private:
		/* Render Data */
		GLuint VAO, VBO, EBO;
		/* Functions */
		void setupMesh();
};


class ColoredMesh: public glMesh{
	public:
		/* Mesh Data */
		std::vector<ColoredVertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		std::vector<glm::vec4> mColors;

		bool hasColors();

		/* Function */
		ColoredMesh(std::vector<ColoredVertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
		ColoredMesh();
		void Draw(Shader shader);
		virtual GLuint getVAO() {return VAO;}
		virtual GLuint getVBO() {return VBO;}
		virtual GLuint getEBO() {return EBO;}
	private:
		/* Render Data */
		GLuint VAO, VBO, EBO;
		/* Functions */
		void setupMesh();
};
#endif
