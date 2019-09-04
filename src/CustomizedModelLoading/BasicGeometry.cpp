#include <CustomizedModelLoading/BasicGeometry.hpp>


const GLfloat* DatCustom::Graphics::BasicGeometry::getQuadVertices(){
	static const GLfloat quadVertices[] = {
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// top right
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
	-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,	// bottom left
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f			// top left
	};
	return quadVertices;
}


size_t DatCustom::Graphics::BasicGeometry::getQuadVerticesNumFloat(){
	return 20;
}


size_t DatCustom::Graphics::BasicGeometry::getQuadIndicesNumUInt(){
	return 6;
}


const GLuint* DatCustom::Graphics::BasicGeometry::getQuadIndices(){
	static const GLuint indices[] = {
		1, 0, 3,
		2, 1, 3
	};
	return indices;
}
