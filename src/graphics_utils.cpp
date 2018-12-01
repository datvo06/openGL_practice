#include <CustomizedUtils/graphics_utils.hpp>
#include <CustomizedUtils/logutils.h>
#include <stdexcept>
#include <CustomizedUtils/image_utils.hpp>
#include <Shader.h>


GLfloat quadVertices[] = {
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// top right
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
	-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,	// bottom left
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f			// top left
};


GLuint quadIndices[] = {
	1, 0, 3,
	2, 1, 3
};


GLuint quadVAO;
GLuint quadVBO;
GLuint quadEBO;
GLuint FBO;
GLuint RBO;


enum kernelTypeShader{SOBELX3, SOBELY3, SOBELXY3NORM1, MEDIAN3};

static const GLchar* textureProgramVertex =
#if defined(__linux__) || defined(__WIN32__)
R"(#version 330 core
)"
#else
R"(#version 300 es
)"
#endif
R"(precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 texCoord;
void main (){
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	texCoord = aTexCoord;
})";

static const GLchar* textureProgramFragment = 
#if defined(__linux__) || defined(__WIN32__)
R"glsl(#version 330 core
)glsl"
#else
R"glsl(#version 300 es
)glsl"
#endif
R"glsl(
precision highp float;
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D frameTexture;
uniform int opType;
uniform int threshType;
uniform float threshValue;
uniform float width, height;
uniform bool useAbs;
float blur_kernel[9] =  float[](
			1.0/16.0, 2.0/16.0, 1.0/16.0,
			2.0/16.0, 4.0/16.0, 2.0/16.0,
			1.0/16.0, 2.0/16.0, 1.0/16.0
);
float edge_kernel[9] = float[](
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
);
float sobelX3[9] = float[](
		-1.0, 0.0, +1.0,
		-2.0, 0.0, +2.0,
		-1.0, 0.0, +1.0
);
float sobelY3[9] = float[](
		-1.0, -2.0, -1.0,
		0.0, 0.0, 0.0,
		+1.0, +2.0, +1.0
);
float sobelX3Scharr[9] = float[](
		-3.0, 0.0, +3.0,
		-10.0, 0.0, +10.0,
		-3.0, 0.0, +3.0
);
float sobelY3Scharr[9] = float[](
		-3.0, -10.0, -3.0,
		0.0, 0.0, 0.0,
		+3.0, +10.0, +3.0
);

void sort9(float A[9]){
	for (int n = 8; n != 0 ; n--) {
		for (int i = 0; i < n; i++) {
			float tmp = min (A[i], A[i+1]);
			A[i+1] = A[i] + A[i+1] - tmp;
			A[i] = tmp;
		}
	}
}

vec4 grayScale(vec4 theFragColor){
	float average = 0.2126 * theFragColor.r + 0.7152 * theFragColor.g + 0.0722 * theFragColor.b;
	return vec4(average, average, average, 1.0f);
}
vec4 median_blur3(float offsetx, float offsety){
	vec2 offsets[9] = vec2[](
		vec2(-offsetx, offsety), 	//top left
		vec2(0.0f, offsety), 			//top-center
		vec2(offsetx, offsety), 	//top-right
		vec2(-offsetx, 0.0f),			//center-left
		vec2(0.0f, 0.0f),					//center
		vec2(offsetx, 0.0f),		  //center-right
		vec2(-offsetx, -offsety),	//bottom-left
		vec2(0.0f, -offsety),			//bottom-center
		vec2(offsetx, -offsety)		//bottom-right
	);
	float sampleVal[9];
	for (int i = 0; i < 9; i++)
		sampleVal[i] = vec3(texture(frameTexture, texCoord.st + offsets[i])).r;
	sort9(sampleVal);
	float val = sampleVal[5];
	return vec4(val, val, val, 1.0f);
}


vec4 kernel3_3_apply(float kernel[9], float offsetx, float offsety){
	vec2 offsets[9] = vec2[](
		vec2(-offsetx, offsety), 	//top left
		vec2(0.0f, offsety), 			//top-center
		vec2(offsetx, offsety), 	//top-right
		vec2(-offsetx, 0.0f),			//center-left
		vec2(0.0f, 0.0f),					//center
		vec2(offsetx, 0.0f),		  //center-right
		vec2(-offsetx, -offsety),	//bottom-left
		vec2(0.0f, -offsety),			//bottom-center
		vec2(offsetx, -offsety)		//bottom-right
	);
	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
		sampleTex[i] = vec3(texture(frameTexture, texCoord.st + offsets[i]));
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i]*kernel[i];
	return vec4(col, 1.0f);
}


vec4 kernel3_3_apply_gray(float kernel[9], float offsetx, float offsety){
	vec2 offsets[9] = vec2[](
		vec2(-offsetx, offsety), 	//top left
		vec2(0.0f, offsety), 			//top-center
		vec2(offsetx, offsety), 	//top-right
		vec2(-offsetx, 0.0f),			//center-left
		vec2(0.0f, 0.0f),					//center
		vec2(offsetx, 0.0f),		  //center-right
		vec2(-offsetx, -offsety),	//bottom-left
		vec2(0.0f, -offsety),			//bottom-center
		vec2(offsetx, -offsety)		//bottom-right
	);
	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
		sampleTex[i] = grayScale(texture(frameTexture, texCoord.st + offsets[i])).rgb;
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i]*kernel[i];
	return vec4(col, 1.0f);
}

void main(){
	FragColor = vec4(texture(frameTexture, texCoord).xyz, 1.0); 	// original color
	// FragColor = vec4(vec3(1.0 - texture(frameTexture, fs_in.texCoord).xyz), 1.0); //invert color
	// FragColor = grayScale(texture(frameTexture, fs_in.texCoord)); //grayScale
	// FragColor = kernel3_3_apply(blur_kernel, 1.0f/400.0f, 1.0f/300.0f); //blur
	// FragColor = kernel3_3_apply(edge_kernel, 1.0f/400.0f, 1.0f/300.0f); // edge
	// FragColor = kernel3_3_apply_gray(edge_kernel, 1.0f/400.0f, 1.0f/300.0f); // edge
	switch (opType){
		case 0:
			FragColor = kernel3_3_apply(sobelX3, 1.0f/width, 1.0f/height);
			if (useAbs) FragColor = abs(FragColor);
			break;
		case 1:
			FragColor = kernel3_3_apply(sobelY3, 1.0f/width, 1.0f/height);
			if (useAbs) FragColor = abs(FragColor);
			break;
		case 2:
			FragColor = vec4(abs(kernel3_3_apply(sobelY3, 1.0f/width, 1.0f/height)).rgb + abs(kernel3_3_apply(sobelX3, 1.0f/width, 1.0f/height)).rgb, 1.0f);
			break;
		case 3:
			FragColor = median_blur3(width, height);
			break;
}
	switch (threshType){
		case 0:
			break;
		case 1:
			if (FragColor.r > threshValue) FragColor.r = 1.0f;
			else FragColor.r = 0.0f;
			break;
	}
})glsl";

Shader *pTextureRenderProgram;
bool isGraphicsModuleInitialized = false;


void DatCustom::GraphicsImageUtil::initializeGraphicsModuleOpenGL(){

	if (isGraphicsModuleInitialized) return;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &RBO);
	isGraphicsModuleInitialized = true;
	pTextureRenderProgram = new Shader(textureProgramVertex, textureProgramFragment);
}


unsigned int DatCustom::ConvertUtil::convertCvMat2GLTexture(cv::Mat inputMat, GLuint textureID)
{
	if (!isGraphicsModuleInitialized) DatCustom::GraphicsImageUtil::initializeGraphicsModuleOpenGL();
   if(inputMat.empty()){
		 DatCustom_debug("DatCustom::ConvertUtil::CVMAT2GLTEXTURE: Image Empty!\n");
		return 0;
  }else{
		if (textureID == 0) glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__WIN32__)
		GLenum inputColourFormat = GL_BGR;
#else
		GLenum inputColourFormat = GL_RGB;
#endif
		GLenum textureFormat = GL_RGB;
		if (inputMat.channels() == 1) {
			inputColourFormat = GL_RED;
			inputMat.convertTo(inputMat, CV_8UC1);
			textureFormat = GL_RED;
		}
		else {
#if !(defined(__linux__) && !defined(__ANDROID__)) && !defined(__WIN32__)
		cv::cvtColor(inputMat, inputMat, CV_BGR2RGB);
#endif
		inputMat.convertTo(inputMat, CV_8UC3);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 // Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0,
				textureFormat, inputMat.cols, inputMat.rows, 0,
				inputColourFormat, GL_UNSIGNED_BYTE,    // Image data type
				inputMat.ptr());        // The actual image data itself
		return textureID;
	}
} 


void setUpFramebufferForDraw(unsigned int width, unsigned int height, unsigned int textureID, bool reInitialize=true){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glViewport(0, 0, width, height);
	if (reInitialize){
		glTexImage2D(GL_TEXTURE_2D,	0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		DatCustom_debug("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
}


void setUpFramebuffer(unsigned int width, unsigned int height, unsigned int inTextureID, unsigned int outTextureID, bool reInitialize=true){
	if (!isGraphicsModuleInitialized){
		DatCustom::GraphicsImageUtil::initializeGraphicsModuleOpenGL();
	}
	if (inTextureID == outTextureID){
		throw std::logic_error("GLSobelX3:: input texture ID and output texture ID are the same!!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, outTextureID);
	setUpFramebufferForDraw(width, height, outTextureID, reInitialize);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// Render the texture with the sobelX program
	pTextureRenderProgram->use();
	pTextureRenderProgram->setFloat("width", width);
	pTextureRenderProgram->setFloat("height", height);
	pTextureRenderProgram->setInt("frameTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inTextureID);
	glBindVertexArray(quadVAO);
}




cv::Mat DatCustom::ConvertUtil::convertGLTexture2CvMat(unsigned int textureID, int width, int height, int nChannels){
	if (!isGraphicsModuleInitialized) DatCustom::GraphicsImageUtil::initializeGraphicsModuleOpenGL();
	cv::Mat output;
	if (nChannels == 3)
		output = cv::Mat(height, width, CV_8UC3);
	else 
		output = cv::Mat(height, width, CV_8UC1);
	glPixelStorei(GL_PACK_ALIGNMENT, (output.step & 3) ? 1 : 4);
	glPixelStorei(GL_PACK_ROW_LENGTH, output.step/output.elemSize());
	glBindTexture(GL_TEXTURE_2D, textureID);
#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__WIN32__)
	if (nChannels == 3)
		glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, output.ptr());
	else
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, output.ptr());
#else
	setUpFramebufferForDraw(width, height, textureID, false);
	return DatCustom::ConvertUtil::convertGLFBO2CvMat(width, height, nChannels);
#endif
	glBindTexture(GL_TEXTURE_2D, 0);
	return output;
}


cv::Mat DatCustom::ConvertUtil::convertGLFBO2CvMat(int width, int height, int nChannels){
	if (!isGraphicsModuleInitialized) DatCustom::GraphicsImageUtil::initializeGraphicsModuleOpenGL();
	cv::Mat output;
#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__WIN32__)
	if (nChannels == 3)
		output = cv::Mat(height, width, CV_8UC3);
	else 
		output = cv::Mat(height, width, CV_8UC1);
#else
	output = cv::Mat(height, width, CV_8UC4);
#endif
	glPixelStorei(GL_PACK_ALIGNMENT, (output.step & 3) ? 1 : 4);
	glPixelStorei(GL_PACK_ROW_LENGTH, output.step/output.elemSize());
	glBindBuffer(GL_READ_FRAMEBUFFER, FBO);
	if (nChannels == 3){
#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__WIN32__)
		glReadPixels(0,  0, width, height, GL_BGR, GL_UNSIGNED_BYTE, output.ptr());
#else
		// OpenGLES 3.0 only accept GL_RGBA
		glReadPixels(0,  0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, output.ptr());
		cv::cvtColor(output, output, CV_RGBA2BGR);
#endif
	}
	else{
#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__WIN32__)
		glReadPixels(0,  0, width, height, GL_RED, GL_UNSIGNED_BYTE, output.ptr());
#else
		// OpenGLES 3.0 only accept GL_RGBA
		glReadPixels(0,  0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, output.ptr());
		int nChannels;
		std::vector<cv::Mat> outputs = DatCustom::ImUtil::getChannels(output, &nChannels);
		output = outputs[0];
#endif
}

	glBindBuffer(GL_READ_FRAMEBUFFER, 0);
	return output;
}



void DatCustom::GraphicsImageUtil::GLSobelX3(unsigned int inTextureID,
	 	unsigned int outTextureID, unsigned int width,
	 	unsigned int height, bool absoluteVal){
	setUpFramebuffer(width, height, inTextureID, outTextureID);
	pTextureRenderProgram->setInt("opType", SOBELX3);
	pTextureRenderProgram->setInt("threshType", 0);
	pTextureRenderProgram->setBool("useAbs", absoluteVal);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void DatCustom::GraphicsImageUtil::GLSobelY3(unsigned int inTextureID,
	 	unsigned int outTextureID, unsigned int width,
	 	unsigned int height, bool absoluteVal
	){
	setUpFramebuffer(width, height, inTextureID, outTextureID);
	pTextureRenderProgram->setInt("opType", SOBELY3);
	pTextureRenderProgram->setInt("threshType", 0);
	pTextureRenderProgram->setBool("useAbs", absoluteVal);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void DatCustom::GraphicsImageUtil::GLSobelXY3(unsigned int inTextureID, unsigned int outTextureID, unsigned int width, unsigned int height, bool absoluteVal){
	setUpFramebuffer(width, height, inTextureID, outTextureID);
	// Render the texture with the sobelX program
	pTextureRenderProgram->setInt("opType", SOBELXY3NORM1);
	pTextureRenderProgram->setInt("threshType", 0);
	pTextureRenderProgram->setBool("useAbs", absoluteVal);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void DatCustom::GraphicsImageUtil::GLMedianBlur3(unsigned int inTextureID, unsigned int outTextureID, unsigned int width, unsigned int height){
	setUpFramebuffer(width, height, inTextureID, outTextureID);
	// Render the texture with the sobelX program
	pTextureRenderProgram->setInt("opType", MEDIAN3);
	pTextureRenderProgram->setInt("threshType", 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



void DatCustom::GraphicsImageUtil::GLSobelXY3withThresh(unsigned int inTextureID, unsigned int outTextureID, unsigned int width, unsigned int height, float threshVal){
	setUpFramebuffer(width, height, inTextureID, outTextureID);
	// Render the texture with the sobelX program
	pTextureRenderProgram->setInt("opType", SOBELXY3NORM1);
	pTextureRenderProgram->setInt("threshType", 1);
	pTextureRenderProgram->setBool("useAbs", true);
	pTextureRenderProgram->setFloat("threshValue", threshVal);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
