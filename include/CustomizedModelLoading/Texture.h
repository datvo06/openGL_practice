#ifndef __CUSTOMIZED_TEXTURE__
#define __CUSTOMIZED_TEXTURE__
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <CustomizedImageLoading/stb_image.h>
#include <assimp/scene.h>
#include <iostream>

namespace DatCustom{
	namespace Graphics{
		struct Texture {
			GLuint id;
			std::string type;
		};
	}
}

GLint TextureFromFile(const char* path);
#endif
