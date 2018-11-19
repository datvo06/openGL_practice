#include <CustomizedModelLoading/Model.h>


void Model::Draw(Shader shader){
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}


void Model::loadModel(std::string path){
	Assimp::Importer  importer;
	const aiScene *scene = importer.ReadFile(path,
		 	aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
 	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode *node, const aiScene *scene){
	// process all the node's meshes (if any)
	printf("Number of meshes in node: %d\n", node->mNumMeshes);
	for (unsigned int i = 0; i < node->mNumMeshes; i++){
		printf("Accessing mesh: %d\n", node->mMeshes[i]);
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// the do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


TexturedMesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	int dIndex = 0;
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++){
		Vertex vertex;
		
		glm::vec3 the_vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		the_vector.x = mesh->mVertices[i].x;
		the_vector.y = mesh->mVertices[i].y;
		the_vector.z = mesh->mVertices[i].z;
		vertex.Position = the_vector;
		// normals
		the_vector.x = mesh->mNormals[i].x;
		the_vector.y = mesh->mNormals[i].y;
		the_vector.z = mesh->mNormals[i].z;
		vertex.Normal = the_vector;	
		printf("Here %d\n", dIndex++);
		if (mesh->mTextureCoords[0]){
			printf("Here %d\n", dIndex++);
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else  vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName){
	std::vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i ++ ){
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++){
			if(std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0){
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if(!skip){
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}
