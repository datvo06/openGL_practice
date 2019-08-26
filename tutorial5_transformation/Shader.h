#ifndef __SHADER_H
#define __SHADER_H
#include <glad/glad.h>
#include <iostream>
#include <Dat/string_utils.h>


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
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
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
};
#endif
