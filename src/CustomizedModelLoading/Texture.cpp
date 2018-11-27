#include <CustomizedModelLoading/Texture.h>
using namespace std;

	
GLint TextureFromFile(const char* path, string directory){
	//Generate texture ID and load texture data
	string filepath = string(path);
	filepath = directory + '/' + filepath;
	int width, height, nrChannels;
	GLuint textureID;
	glGenTextures(1, &textureID);
	stbi_set_flip_vertically_on_load(true);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data = NULL;
	data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	unsigned int type_load;
	if(nrChannels == 3)
		type_load = GL_RGB;
	else 
		type_load = GL_RGBA;

	if (data != NULL)
	{
		// Texture currently bound, mipmap manually create, type to store, width, height, always 0, format datatype of source image, image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, type_load, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		throw std::logic_error("Failed to load texture\n");
	}
	// done with the image
	stbi_image_free(data);
	return textureID;
}
