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
					virtual std::vector<GLuint> getVBOs() = 0;
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
				virtual std::vector<GLuint>getVBOs() {return VBOs;}
				virtual GLuint getEBO() {return EBO;}
				virtual ~ColoredBlendShapeMesh();
			private:
				std::vector<GLuint> VBOs;
				GLuint VAO, EBO;
				bool bHasDistincColors;
				void setupMesh();
		};
	}
}


#endif
