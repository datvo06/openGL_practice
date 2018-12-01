#ifndef __GRAPHICS_UTILS_HPP__
#define __GRAPHICS_UTILS_HPP__
#include <opencv2/opencv.hpp>
#ifdef __linux__
    #ifdef __ANDROID__
/*
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
 */

#include <glad/glad_egl.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
    #else
#include <linux_glad/glad.h>
#include <GLFW/glfw3.h>
    #endif
#elif __APPLE__
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#endif


namespace DatCustom{
	namespace ConvertUtil{
		/**
		 * @brief converting opencv matrix (image) into openGL's texture for rendering
		 * @param[in] inputMat : the opencv's matrix, expected BGR 
		 * @param[in] textureID if different than 0, convert to this texture
		 * @return textureID : openGL's textureID
		 */
		GLuint convertCvMat2GLTexture(cv::Mat inputMat, GLuint textureID=0);
		/**
		 * @brief converting openGL texture data into openCV's matrix
		 * @param[in] textureID : the openGL's texture ID
		 * @param[in] width : the width of the image
		 * @param[in] height : the height of the image
		 * @return openGL's textureID
		 */
		cv::Mat convertGLTexture2CvMat(unsigned int texture, int width, int height, int nChannels=3);
		/**
		 * @brief converting openGL's rendered framebuffer data into openCV's matrix
		 * @param[in] width : the width of the image
		 * @param[in] height : the height of the image
		 * @return openGL's textureID
		 */
		cv::Mat convertGLFBO2CvMat(int width, int height, int nChannels=3);

	}
	namespace GraphicsImageUtil{
		/**
		 * @brief apply Sobel kernel X(3x3) on texture image by using framebuffer objects 
		 * @param[in] inTextureID : input texture ID
		 * @param[in] outTextureID : output texture ID (must be different from inTextureID
		 * @param[in] width : texture's width
		 * @param[in] height : texture's height
		 * @param[in] absolute_val : take absolute or not
		 */
		void GLSobelX3(unsigned int inTextureID, unsigned int outTextureID, unsigned int width, unsigned int height, bool absoluteVal=true);
		/**
		 * @brief apply Sobel kernel Y(3x3) on texture image by using framebuffer objects 
		 * @param[in] inTextureID : input texture ID
		 * @param[in] outTextureID : output texture ID (must be different from inTextureID
		 * @param[in] width : texture's width
		 * @param[in] height : texture's height
		 * @param[in] absolute_val : take absolute or not
		 */
		void GLSobelY3(unsigned int inTextureID, unsigned int outTextureID, unsigned int width, unsigned int height, bool absoluteVal=true);
		/**
		 * @brief apply Sobel kernel XY(3x3) on texture image by using framebuffer objects 
		 * @param[in] inTextureID : input texture ID
		 * @param[in] outTextureID : output texture ID (must be different from inTextureID
		 * @param[in] width : texture's width
		 * @param[in] height : texture's height
		 * @param[in] absolute_val : take absolute or not
		 */
		void GLSobelXY3(unsigned int inTextureID, unsigned int outTextureID, unsigned int width, unsigned int height, bool absoluteVal=true);
		/**
		 * @brief apply Sobel kernel XY(3x3) on texture image by using framebuffer objects, followed by thresh hold
		 * @param[in] inTextureID : input texture ID
		 * @param[in] outTextureID : output texture ID (must be different from inTextureID
		 * @param[in] width : texture's width
		 * @param[in] height : texture's height
		 * @param[in] threshVal: range from 0.0f(0) to 1.0f (255)
		 */
		void GLSobelXY3withThresh(unsigned int inTextureID, unsigned int outTextureID, unsigned int width, unsigned int height, float threshVal=0.0f);
		/**
		 * @brief apply medianBlur kernel size = 3
		 * @param[in] inTextureID : input texture ID
		 * @param[in] outTextureID : output texture ID (must be different from inTextureID
		 * @param[in] width : texture's width
		 * @param[in] height : texture's height
		 * @param[in] absolute_val : take absolute or not
		 */
		void GLMedianBlur3(unsigned int inTextureID, unsigned int outTextureID, unsigned int width, unsigned int height);
		void initializeGraphicsModuleOpenGL();
	}
}
#endif
