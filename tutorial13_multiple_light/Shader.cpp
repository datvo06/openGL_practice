#include "Shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath){
	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLint success;
	GLchar infoLog[512];

  const GLchar* vertexSource = DatTools::Util::string_from_file_c(vertexPath);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
  const GLchar* fragmentSource = DatTools::Util::string_from_file_c(fragmentPath);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	glDetachShader(ID, vertexShader);
	glDetachShader(ID, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void Shader::use(){
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, GLint value) const{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int) value);
}

void Shader::setFloat(const std::string &name, GLfloat value) const{
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}


void Shader::setMat4(const std::string &name, GLfloat* value, GLuint transpose) const{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1,  transpose, value);
}


void Shader::setVec3(const std::string &name, GLfloat* value, int _count) const{
	glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), _count, value);
}


void Shader::setVec3(const std::string &name, GLfloat v1, GLfloat v2, GLfloat v3) const{
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3);
}
