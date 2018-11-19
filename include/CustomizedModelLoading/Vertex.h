#ifndef __CUSTOMIZED_VERTEX__
#define __CUSTOMIZED_VERTEX__
#include <glm/glm.hpp>

struct TexturedVertex {
	glm::vec3 Position;
	glm:: vec3 Normal;
	glm::vec2 TexCoords;
};


struct ColoredVertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec4 Colors;
};

#endif
