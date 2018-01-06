#include <iostream>
#include <Dat/string_utils.h>
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void init();
void render();
void terminate();


GLfloat vertices[] = {
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// top right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f,	// bottom right
	-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, -1.0f, -1.0f,	// bottom left
	-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f			// top left
};
	GLuint indices[] = {
	0, 1, 3,
	1, 2, 3
};

int width, height, nrChannels;
GLuint wallTexture;
GLuint faceTexture;


GLuint VAO;
GLuint VBO;
GLuint EBO;
Shader* shaderProgram;

int main ()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //On Mac only
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Call this before using any openGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	init();
	while(!glfwWindowShouldClose(window)){
		// Input
		processInput(window);
		// Rendering
		render();

		// Check poll events and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	terminate();
	return 0;
}


void init(){
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	shaderProgram = new Shader("vertex.glsl", "fragment.glsl");
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
	unsigned int type_load;
	if(nrChannels == 3)
		type_load = GL_RGB;
	else 
		type_load = GL_RGBA;

	if (data)
	{
		// Texture currently bound, mipmap manually create, type to store, width, height, always 0, format datatype of source image, image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, type_load, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	// done with the image
	stbi_image_free(data);

	glGenTextures(1, &faceTexture);
	glBindTexture(GL_TEXTURE_2D, faceTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if(nrChannels == 3)
		type_load = GL_RGB;
	else 
		type_load = GL_RGBA;

	if (data)
	{
		// Texture currently bound, mipmap manually create, type to store, width, height, always 0, format datatype of source image, image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, type_load, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	// done with the image
	stbi_image_free(data);
	// 0. Bind VAO
	glBindVertexArray(VAO);
	// 1. Setup buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 2. Vertex Attribute pointer
	//location in vertex shader, vertex size, vertex type, normalize to 0-1?, stride - space
	//between consecutive vertex attribute set, (void*) offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	shaderProgram->use();
	shaderProgram->setInt("wallTexture", 0);
	shaderProgram->setInt("faceTexture", 1);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void render(){
	glClear(GL_COLOR_BUFFER_BIT);
	// 4. draw
	shaderProgram->use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, faceTexture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void terminate(){
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}
