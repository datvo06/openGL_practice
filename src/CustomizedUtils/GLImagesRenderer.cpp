#include <CustomizedUtils/RenderToImageManager.hpp>
#include <CustomizedUtils/graphics_utils.hpp>
#include <CustomizedUtils/logutils.h>


static GLfloat quadVertices[] = {
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// top right
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
	-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,	// bottom left
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f			// top left
};


static GLuint quadIndices[] = {
	1, 0, 3,
	2, 1, 3
};


namespace DatCustom{
	namespace Graphics{

RenderToImageManager::RenderToImageManager(){
	width = 800; 
	height = 600;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &outTextureID);
	glBindTexture(GL_TEXTURE_2D, outTextureID);
 	glTexImage2D(GL_TEXTURE_2D,	0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outTextureID, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderToImageManager& RenderToImageManager::instance(){
	static RenderToImageManager renderToImage;
	return renderToImage;
}

RenderToImageManager::~RenderToImageManager(){
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &outTextureID);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &FBO);
	glDeleteBuffers(1, &RBO);
}


void RenderToImageManager::setupFrameBuffer(unsigned int width, unsigned int height){
	if (width == this->width && height == this->height) return;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, outTextureID);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glViewport(0, 0, width, height);
	glTexImage2D(GL_TEXTURE_2D,	0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outTextureID, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		DatCustom_debug("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void RenderToImageManager::use(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void RenderToImageManager::stop(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GLuint RenderToImageManager::getOutTexture(){
	return outTextureID;
}

cv::Mat RenderToImageManager::getOutputFrame(){
	return DatCustom::ConvertUtil::convertGLFBO2CvMat(width, height, FBO, 3);
}

}
}
