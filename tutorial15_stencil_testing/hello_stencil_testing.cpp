#include <iostream>
#include <Dat/string_utils.h>
#include <Shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <CustomizedModelLoading/Model.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.h>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void draw_model(Camera& theCam);
void draw_lamp(Camera& theCam);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void init();
void render();
void terminate();

Model* pModel;

GLfloat vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f, 2.0f, -12.0f),
	glm::vec3(0.0f, 0.0f, -3.0f),
}; 


GLuint indices[] = {
	0, 1, 3,
	1, 2, 3
};

GLuint screenWidth = 800;
GLuint screenHeight = 600;


GLuint lampVAO;
GLuint VBO;
GLuint EBO;
Shader *shaderProgram, *lampProgram, *stencilProgram;


Camera theCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);

glm::mat4 projection;
glm::vec3 lightPos(1.7f, 1.0f, 4.0f);

float currentTime;
float nextTime;

GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


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
	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	init();
	while(!glfwWindowShouldClose(window)){
		nextTime = glfwGetTime();
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
	std::cout << "Loading model..." << std::endl;
	pModel  = new Model("./nanosuit/nanosuit.obj");
	std::cout << "Loading Shader..." << std::endl;
	shaderProgram = new Shader("vertex.glsl", "fragment.glsl");
	lampProgram = new Shader("vertex.glsl", "lamp_fragment.glsl");
	stencilProgram = new Shader("vertex.glsl", "fragment_stencil.glsl");
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &lampVAO);
		// 0. Bind VAO
	// 1. Setup buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 2. Vertex Attribute pointer
	//location in vertex shader, vertex size, vertex type, normalize to 0-1?, stride - space
	//between consecutive vertex attribute set, (void*) offset
	glBindVertexArray(lampVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window){
	float deltaTime = nextTime - currentTime;
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		theCamera.ProcessKeyboard(FORWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		theCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		theCamera.ProcessKeyboard(LEFT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		theCamera.ProcessKeyboard(RIGHT, deltaTime);
}



void render(){
	currentTime = nextTime;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// 4. draw
	draw_model(theCamera);
	draw_lamp(theCamera);
	glBindVertexArray(0);
}

void draw_model(Camera& theCam){
	shaderProgram->use();
	shaderProgram->setFloat("material.shininess", 32.0f);

	glm::vec3 lightColor;
	lightColor.x = 1.0f;
	lightColor.y = 1.0f;
	lightColor.z = 1.0f;

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
	glm::vec3 lightDir(0.0f, -1.0f, -2.0f);
	shaderProgram->setVec3("dirlight.direction", glm::value_ptr(lightDir));
	shaderProgram->setVec3("dirlight.ambient", glm::value_ptr(ambientColor));
	shaderProgram->setVec3("dirlight.diffuse", glm::value_ptr(diffuseColor));
	shaderProgram->setVec3("dirlight.specular", 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 4; i++){
		std::string pointString = "pointLights";
		pointString  += "[" + std::to_string(i)+ "].";
		shaderProgram->setVec3((pointString+"position"), glm::value_ptr(pointLightPositions[i]));
		shaderProgram->setVec3((pointString+"ambient"), glm::value_ptr(ambientColor));
		shaderProgram->setVec3((pointString+"diffuse"), glm::value_ptr(diffuseColor));
		shaderProgram->setVec3((pointString+"specular"), 1.0f, 1.0f, 1.0f);
		shaderProgram->setFloat((pointString+"constant"), 1.0f);
		shaderProgram->setFloat((pointString+"linear"), 0.09f);
		shaderProgram->setFloat((pointString+"quadratic"), 0.032f);
	}
	
	glm::mat4 model, model_stencil;
	glm::mat4 view = theCam.GetViewMatrix();
	// pos, target, up
	
	projection = glm::perspective(glm::radians(theCam.Zoom), (float)screenWidth/screenHeight, 0.1f, 100.0f);
	shaderProgram->setMat4("view", glm::value_ptr(view));
	shaderProgram->setMat4("projection", glm::value_ptr(projection));
	shaderProgram->setVec3("viewPos", glm::value_ptr(theCam.Position));

	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	shaderProgram->setMat4("model", glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	pModel->Draw(*shaderProgram);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	stencilProgram->use();
	stencilProgram->setMat4("view", glm::value_ptr(view));
	stencilProgram->setMat4("projection", glm::value_ptr(projection));

	model_stencil	= glm::scale(model_stencil, glm::vec3(0.21f, 0.21f, 0.21f));	// it's a bit too big for our scene, so scale it down
	stencilProgram->setMat4("model", glm::value_ptr(model_stencil));
	pModel->Draw(*stencilProgram);
	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xFF);
}



void draw_lamp(Camera& theCam){
	lampProgram->use();
	glm::mat4 view;
	// pos, target, up
	view = theCam.GetViewMatrix();
	projection = glm::perspective(glm::radians(theCam.Zoom), (float)screenWidth/screenHeight, 0.1f, 100.0f);
	lampProgram->setMat4("view", glm::value_ptr(view));
	lampProgram->setMat4("projection", glm::value_ptr(projection));
	glBindVertexArray(lampVAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	for (int i = 0; i < 4; i++){
		glm::mat4 model;
	 	model	= glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		lampProgram->setMat4("model", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	theCamera.ProcessMouseMovement(xoffset, yoffset, true);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	theCamera.ProcessMouseScroll(yoffset);
}


void terminate(){
	glDeleteBuffers(1, &lampVAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}
