#ifndef __CUSTOMIZED_MODEL__
#define __CUSTOMIZED_MODEL__
#include <CustomizedModelLoading/Mesh.h>
#include <CustomizedModelLoading/MeshManager.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace DatCustom{
	namespace Graphics{
		class TexturedModel: public Mesh{
			public:
				/* Functions */
				TexturedModel(const char *path){
					loadModel(path);
				}
				virtual void Draw(Shader shader);
			private:
				std::vector<MeshPtr> meshes;
				std::string directory;
				/* Function */
				void loadModel(std::string path);
				void processNode(aiNode *node, const aiScene *scene);
				MeshPtr processMesh(aiMesh *mesh, const aiScene *scene);
				std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
						std::string typeName);
				std::vector<Texture> textures_loaded;
		};
	}
}
#endif
