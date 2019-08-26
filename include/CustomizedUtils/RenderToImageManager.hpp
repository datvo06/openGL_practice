#ifndef __CUSTOMIZED_UTILS_MESH_MANAGER_HPP__
#define __CUSTOMIZED_UTILS_MESH_MANAGER_HPP__
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
	namespace Graphics{
		class RenderToImageManager{
			RenderToImageManager();
			RenderToImageManager(const RenderToImageManager& rhs);
			RenderToImageManager& operator=(const RenderToImageManager& rhs);

			GLuint outTextureID;
			GLuint VAO;
			GLuint VBO;
			GLuint EBO;
			GLuint FBO;
			GLuint RBO;
			unsigned int width, height;
			virtual ~RenderToImageManager();
			public:
			static RenderToImageManager& instance();
			void setupFrameBuffer(unsigned int width, unsigned int height);
			void use();
			void stop();
			GLuint getOutTexture();
			cv::Mat getOutputFrame();
		};
	}
}

#endif
