#include <iostream>
#include <Dat/string_utils.h>
#include <Shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <CustomizedModelLoading/Model.h>
#include <CustomizedUtils/RenderToImageManager.hpp>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.h>
#include <CustomizedModelLoading/MeshManager.hpp>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void draw_model(Camera& theCam);
void draw_lamp(Camera& theCam);
void draw_framebuffer();
void draw_cubemap(Camera& theCam);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void init(std::string modelPath = "");
void render();
void terminate();

using DatCustom::Graphics::MeshPtr;
using DatCustom::Graphics::MeshManager;
MeshPtr pModel;

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

GLfloat cubeVertices[] = {
    // positions          // normals           // texture coords
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};



glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f, 2.0f, -12.0f),
	glm::vec3(0.0f, 0.0f, -3.0f),
}; 


GLuint indices[] = {
	1, 0, 3,
	2, 1, 3
};

GLuint screenWidth = 800;
GLuint screenHeight = 600;

GLuint quadVAO;
GLuint quadVBO;
GLuint quadEBO;

GLuint lampVAO;
GLuint VBO;
GLuint EBO;


GLuint cubeVAO;
GLuint cubeVBO;

Shader *shaderProgram, *lampProgram, *textureBufferProgram, *cubeProgram;


Camera theCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);

glm::mat4 projection;
glm::vec3 lightPos(1.7f, 1.0f, 4.0f);

float currentTime;
float nextTime;
float sumDeltaTime = 0.0f;
int gFrameCount = 0;

GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

/*
std::vector<std::string> cubeFaces{
	"../skyboxes/hw_alps/alps_rt.tga",
	"../skyboxes/hw_alps/alps_lf.tga",
	"../skyboxes/hw_alps/alps_up.jpg",
	"../skyboxes/hw_alps/alps_dn.jpg",
	"../skyboxes/hw_alps/alps_bk.tga",
	"../skyboxes/hw_alps/alps_ft.tga",
};

*/

std::vector<std::string> cubeFaces{
	"../skyboxes/mp_boulder/boulder-bay_rt.tga",
	"../skyboxes/mp_boulder/boulder-bay_lf.tga",
	"../skyboxes/mp_boulder/boulder-bay_up.jpg",
	"../skyboxes/mp_boulder/boulder-bay_dn.jpg",
	"../skyboxes/mp_boulder/boulder-bay_bk.tga",
	"../skyboxes/mp_boulder/boulder-bay_ft.tga",
};

GLuint cubemapTexture;

int main (int argc, char** argv)
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
	init(argv[1]);
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

// faces: right, left, top, bottom, back, front
GLuint loadCubemap(std::vector<std::string> faces){
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrchannels;
	for (unsigned int i = 0; i < faces.size(); i++){
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrchannels, 0);
		if (data) 
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}


void init(std::string modelPath){
	std::cout << "Loading model..." << std::endl;
	pModel.reset(&*MeshManager::instance().loadStaticMesh(modelPath.c_str()));
	std::cout << "Loading Shader..." << std::endl;
	cubemapTexture = loadCubemap(cubeFaces);
	shaderProgram = new Shader("vertex.glsl", "fragment.glsl");
	lampProgram = new Shader("vertex.glsl", "lamp_fragment.glsl");
	textureBufferProgram = new Shader("vertex_backup.glsl", "fragment_backup.glsl");
	cubeProgram = new Shader("vertex_cube.glsl", "fragment_cube.glsl");

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
	glBindVertexArray(0);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &lampVAO);


		// target, attachment, textarget, texture, level
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


	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// 2. Vertex Attribute pointer
	//location in vertex shader, vertex size, vertex type, normalize to 0-1?, stride - space
	//between consecutive vertex attribute set, (void*) offset
	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	printf("Finished initializing\n");
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
	sumDeltaTime += nextTime - currentTime;
	if (sumDeltaTime >= 1.0/30){
		cv::Mat imout = DatCustom::Graphics::RenderToImageManager::instance().getOutputFrame();
		cv::imwrite((std::string("data/frame_") + std::to_string(gFrameCount) + ".jpg").c_str(), imout);
		FILE* outCam = fopen((std::string("data/cam_") + std::to_string(gFrameCount) + ".txt").c_str(), "w");
		fprintf(outCam, "%f %f %f\n", theCamera.Position.x, theCamera.Position.y, theCamera.Position.z);
		fprintf(outCam, "%f %f %f\n", theCamera.Front.x, theCamera.Front.y, theCamera.Front.z);
		fprintf(outCam, "%f %f %f\n", theCamera.Up.x, theCamera.Up.y, theCamera.Up.z);
		fprintf(outCam, "%f %f %f\n", theCamera.Right.x, theCamera.Right.y, theCamera.Right.z);
		fclose(outCam);
		gFrameCount += 1;
		sumDeltaTime = 0;
	}
	currentTime = nextTime;
	// 4. draw
	// 4.1 Render with new framebuffer bound
	DatCustom::Graphics::RenderToImageManager::instance().use();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	printf("Camera pos - dir: %f, %f ,%f - %f, %f, %f\n",
		 	theCamera.Position.x, theCamera.Position.y, theCamera.Position.z,
			theCamera.Front.x, theCamera.Front.y, theCamera.Front.z);


	draw_model(theCamera);
	draw_lamp(theCamera);
	draw_cubemap(theCamera);
	
	// 4.2 Bind to default framebuffer
	
	DatCustom::Graphics::RenderToImageManager::instance().stop();
	draw_framebuffer();
	// 4.3 draw quad span fullcenter with texture as bound
	glBindVertexArray(0);
}

void draw_model(Camera& theCam){
	shaderProgram->use();
	shaderProgram->setFloat("material.shininess", 32.0f);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

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
	glm::mat4 view = theCam.GetViewMatrix();
	// pos, target, up
	
	projection = glm::perspective(glm::radians(theCam.Zoom), (float)screenWidth/screenHeight, 0.1f, 100.0f);
	shaderProgram->setMat4("view", glm::value_ptr(view));
	shaderProgram->setMat4("projection", glm::value_ptr(projection));
	shaderProgram->setVec3("viewPos", glm::value_ptr(theCam.Position));

	model = glm::rotate(model, glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	model = glm::rotate(model, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));	// it's a bit too big for our scene, so scale it down
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(-4.0f, -5.0f, 13.0f)); // translate it down so it's at the center of the scene
	shaderProgram->setMat4("model", glm::value_ptr(model));
	pModel->Draw(*shaderProgram);
}


void draw_lamp(Camera& theCam){
	glDisable(GL_CULL_FACE);
	lampProgram->use();
	glm::mat4 view = theCam.GetViewMatrix();
	// pos, target, up
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
	glEnable(GL_CULL_FACE);
}



void draw_cubemap(Camera& theCam){
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	cubeProgram->use();
	glm::mat4 view;
	// pos, target, up
	view = glm::mat4(glm::mat3(theCam.GetViewMatrix()));
	projection = glm::perspective(glm::radians(theCam.Zoom), (float)screenWidth/screenHeight, 0.1f, 100.0f);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	cubeProgram->setMat4("view", glm::value_ptr(view));
	cubeProgram->setMat4("projection", glm::value_ptr(projection));
	glBindVertexArray(cubeVAO);
	glm::mat4 model;
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
}


void draw_framebuffer(){
	textureBufferProgram->use();
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	textureBufferProgram->setInt("frameTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DatCustom::Graphics::RenderToImageManager::instance().getOutTexture());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
