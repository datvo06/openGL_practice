#include <CustomizedModelLoading/Mesh.h>
// #include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model{
	public:
		/* Functions */
		Model(char *path){
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
		vector<
};
