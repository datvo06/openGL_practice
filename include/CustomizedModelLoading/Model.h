#ifndef __CUSTOMIZED_MODEL__
#define __CUSTOMIZED_MODEL__
#include <CustomizedModelLoading/Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model{
	public:
		/* Functions */
		Model(const char *path){
			loadModel(path);
		}
		void Draw(Shader shader);
	private:
		std::vector<Mesh> meshes;
		std::string directory;
		/* Function */
		void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
				std::string typeName);
		std::vector<Texture> textures_loaded;
};
#endif
