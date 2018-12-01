#ifndef __SHADER_H
#define __SHADER_H
#ifdef __linux__
#ifdef __ANDROID__
/**
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

#include <iostream>

class Shader
{
	public:
		// the program ID
		GLuint ID;
		/*
		 * @brief Initialize shader
		 * @param vertexPath : path to the vertex shader source
		 * @param fragmentPath : path to the fragment shader source
		 */
		Shader(const GLchar* vertexSource, const GLchar* fragmentSource);
		/* 
		 * @brief Use this shader program
		 */
		void use();
		/*
		 * @brief set the uniform, bool
		 * @param name : the name of the uniform in shader
		 * @param value : the value to be set
		 */
		void setBool(const std::string &name, bool value) const;
		/*
		 * @brief set the uniform, int
		 * @param name : the name of the uniform in shader
		 * @param value : the value to be set
		 */
		void setInt(const std::string &name, GLint value) const;
		/*
		 * @brief set the uniform, float
		 * @param name : the name of the uniform in shader
		 * @param value : the value to be set
		 */
		void setFloat(const std::string &name, GLfloat value) const;
		/*
		 * @brief set the uniform, mat4 
		 * @param name : the name of the uniform in shader
		 * @param value : the value to be set (float pointer)
		 * @param transpose : should the matrix be transposed?
		 */
		void setMat4(const std::string &name, GLfloat* value, GLuint transpose=GL_FALSE) const;
		/*
		 * @brief set the uniform, vec3
		 * @param name : the name of the uniform in shader
		 * @param value : the value to be set (float pointer)
		 * @param _count : number of vecs
		 */
		void setVec3(const std::string &name, GLfloat* value, int _count=1) const;
		/*
		 * @brief set the uniform, vec3
		 * @param name : the name of the uniform in shader
		 * @param v0 : the value to be set (float pointer)
		 * @param v1: the value to be set (float pointer)
		 * @param v2 : the value to be set (float pointer)
		 */
		void setVec3(const std::string &name, GLfloat v0,
				GLfloat v1, GLfloat v2) const;

};
#endif
