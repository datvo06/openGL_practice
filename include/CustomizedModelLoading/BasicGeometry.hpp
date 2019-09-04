#ifndef __CUSTOMIZED_MODEL_LOADING_BASIC_GEOMETRY_HPP__
#define __CUSTOMIZED_MODEL_LOADING_BASIC_GEOMETRY_HPP__
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace DatCustom{
	namespace Graphics{
		namespace BasicGeometry{
			const GLuint* getQuadIndices();
			const GLfloat* getQuadVertices();
			size_t getQuadVerticesNumFloat();
			size_t getQuadIndicesNumUInt();
		}
	}
}
#endif
