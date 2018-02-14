#include <iostream>
#include <Dat/string_utils.h>
#include <Shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <CustomizedImageLoading/stb_image.h>
#include <CustomizedModelLoading/Mesh.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void draw_cubes(Camera& theCam);
void draw_lamp(Camera& theCam);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void init();
void render();
void terminate();


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


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, 3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
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

int width, height, nrChannels;
GLuint screenWidth = 800;
GLuint screenHeight = 600;
GLuint diffuseMap;
GLuint specMap;


GLuint VAO;
GLuint lampVAO;
GLuint VBO;
GLuint EBO;
Shader *shaderProgram, *lampProgram;


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


void loadTexture(GLuint textureID, const char* filepath, bool flipped=true){
	stbi_set_flip_vertically_on_load(flipped);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data;
	data = stbi_load(filepath, &width, &height, &nrChannels, 0);
	unsigned int type_load;
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

}


void initTexture(){
	glGenTextures(1, &diffuseMap);
	loadTexture(diffuseMap, "container2.png");
	glGenTextures(1, &specMap);
	loadTexture(specMap, "container2_specular.png");
}


void init(){
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	initTexture();
	shaderProgram = new Shader("vertex.glsl", "fragment.glsl");
	lampProgram = new Shader("vertex.glsl", "lamp_fragment.glsl");
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lampVAO);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// 4. draw
	draw_cubes(theCamera);
	draw_lamp(theCamera);
	glBindVertexArray(0);
}

void draw_cubes(Camera& theCam){
	shaderProgram->use();
	shaderProgram->setInt("material.diffuse", 0);
	shaderProgram->setInt("material.specular", 1);
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
	
	glm::mat4 model;
	glm::mat4 view;
	// pos, target, up
	view = theCam.GetViewMatrix();
	projection = glm::perspective(glm::radians(theCam.Zoom), (float)screenWidth/screenHeight, 0.1f, 100.0f);
	shaderProgram->setMat4("view", glm::value_ptr(view));
	shaderProgram->setMat4("projection", glm::value_ptr(projection));
	shaderProgram->setVec3("viewPos", glm::value_ptr(theCam.Position));
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specMap);
	for(unsigned int i = 0; i < 10; i++){
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f*i;
		model= glm::rotate(model, currentTime+angle, glm::vec3(1.0, 0.3, 0.5f));
		// model = glm::scale(model, glm::vec3(fabs(sin(angle)), fabs(sin(angle)), 1.0f));
		shaderProgram->setMat4("model", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}
