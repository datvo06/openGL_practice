#ifndef _CUSTOMIZED_UTILS_GL_MANAGER_HPP__
#define _CUSTOMIZED_UTILS_GL_MANAGER_HPP__
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
		class GLManager{
				GLManager();
				GLManager(const GLManager& rhs);
			public:
		};
	}
};
#endif
