#ifndef __CUSTOMIZED_MODEL_LOADING__BLENDSHAPE_MESH_H__
#define __CUSTOMIZED_MODEL_LOADING__BLENDSHAPE_MESH_H__

#include <CustomizedModelLoading/Mesh.h>

namespace DatCustom{
	namespace Graphics{
		class BlendShapeMesh{
			public:
					virtual void Draw(Shader shader,
							std::vector<unsigned int> indices, std::vector<float> weights) = 0;
					virtual GLuint getVAO() = 0;
					virtual GLuint getVBO() = 0;
					virtual GLuint getEBO() = 0;
					virtual ~BlendShapeMesh(){
						printf("Deleting BlendShapeMesh...\n");
					}
		};

		class ColoredBlendShapeMesh: public BlendShapeMesh{
			public:
				std::vector<std::vector<ColoredVertex>> vertices;
				std::vector<GLuint> indices;

				/* Function */
				ColoredBlendShapeMesh(
						std::vector<std::vector<ColoredVertex>> vertices, std::vector<GLuint> indices, bool has_different_color=true);
				ColoredBlendShapeMesh(std::vector<CTM::Mesh>& listCtmMeshes, bool bHasDiffColors=true);
				ColoredBlendShapeMesh();
				void Draw(Shader shader, std::vector<unsigned int> indices, std::vector<float> weights);
				virtual GLuint getVAO() {return VAO;}
				virtual GLuint getVBO() {return VBO;}
				virtual GLuint getEBO() {return EBO;}
				virtual ~ColoredBlendShapeMesh();
			private:
				GLuint VAO, VBO, EBO;
				bool bHasDistincColors;
				void setupMesh();
		};

		class TexturedBlendShapeMesh: public BlendShapeMesh{
			public:
				std::vector<std::vector<TexturedVertex>> vertices;
				std::vector<GLuint> indices;
				std::vector<Texture> textures;
				TexturedBlendShapeMesh(
							std::vector<std::vector<TexturedVertex>> texturedvertices,
							std::vector<GLuint> indices, std::vector<Texture> textures
				);
				TexturedBlendShapeMesh(std::vector<CTM::Mesh>& listCtmMeshes);
				TexturedBlendShapeMesh();
				virtual GLuint getVAO() { return VAO; };
				virtual GLuint getVBO() { return VBO; };
				virtual GLuint getEBO() { return EBO; };

				void Draw(Shader shader, std::vector<unsigned int> indices, std::vector<float> weights);
				virtual ~TexturedBlendShapeMesh();
			private:
				GLuint VAO, VBO, EBO;
				void setupMesh();
		};
	}
}


#endif
