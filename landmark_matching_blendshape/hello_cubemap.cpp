#include <iostream>
#include <Dat/string_utils.h>
#include <Shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <CustomizedUtils/RenderToImageManager.hpp>
#include <CustomizedModelLoading/MeshManager.hpp>
#include <CustomizedModelLoading/Texture.h>
#include <CustomizedModelLoading/BasicGeometry.hpp>
#include <eigen3/Eigen/Eigen>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.h>
#include <thread>
#include <mutex>

#include <glm/gtx/string_cast.hpp>
#include <opencv2/opencv.hpp>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>


using DatCustom::Graphics::MeshManager;
using DatCustom::Graphics::MeshPtr;
using DatCustom::Graphics::BlendShapeMeshPtr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void draw_model(Camera& theCam);
void draw_lamp(Camera& theCam);
void draw_framebuffer();
void draw_cubemap(Camera& theCam);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void init(const char* videoPath);
void render();
void terminate();
void processCalculation();
std::vector<float> estimateModelParameters(const dlib::full_object_detection& shape, cv::Mat& image);


BlendShapeMeshPtr theBlendShape;

std::vector<unsigned int> blendShapeindices = {0, 1, 2, 3, 4, 5, 6};
std::vector<float> weights = {0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14};
std::vector<unsigned int> kptIndices = {
	21874,22150,21654,21037,43237,44919,46167,47136,47915,48696,49668,50925,52614,33679,33006,32470,32710,38696,39393,39783,39988,40155,40894,
	41060,41268,41662,42368,8162,8178,8188,8193,6516,7244,8205,9164,9884,2216,3887,4921,5829,4802,3641,10456,11354,12384,14067,12654,11493,5523,
	6026,7496,8216,8936,10396,10796,9556,8837,8237,7637,6916,5910,7385,8224,9065,10538,8830,8230,7630
};

GLfloat** kptPoints;


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
	glm::vec3(3.0f, 0.0f, -3.0f),
}; 



GLuint screenWidth = 800;
GLuint screenHeight = 600;

GLuint quadVAO;
GLuint quadVBO;
GLuint quadEBO;

GLuint lampVAO;
GLuint lampVBO;
GLuint lampEBO;


GLuint cubeVAO;
GLuint cubeVBO;

Shader *blendShapeShaderProgram, *modelShaderProgram, *lampProgram, *textureBufferProgram, *cubeProgram;


// The original config
// Camera theCamera(glm::vec3(0, 1, 0), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);
Camera theCamera(glm::vec3(0, 1, 0), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);

glm::mat4 projection;
glm::mat4 parameterizedRotation;
glm::vec3 lightPos(1.7f, 1.0f, 4.0f);

float currentTime;
float nextTime;
float sumDeltaTime = 0.0f;
int gFrameCount = 0;
int totalFrame = 0;

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
cv::VideoCapture* pCap;
dlib::frontal_face_detector g_faceDetector;
dlib::shape_predictor g_sp;


bool g_IsQuitting = false;
std::mutex g_captureMutex;
std::mutex g_weightMutex;
std::mutex g_rotationMatMutex;


int main (int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //On Mac only
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	const char* videoPath = argc < 2 ? 0 : argv[1];
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
	init(videoPath);
	std::thread calculateThread(processCalculation);
	while(!glfwWindowShouldClose(window)){
		nextTime = glfwGetTime();
		// Input
		processInput(window);
		// Update the model parameters
		// Rendering
		render();
		// Check poll events and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	{
		std::lock_guard<std::mutex> lock(g_captureMutex);
		g_IsQuitting = true;
	}
	calculateThread.join();
	terminate();
	return 0;
}


void processCalculation(){
	cv::Mat frame, smallFrame;
	const int iSkipFrame = 2;
	int iframeCount = 0;
	while(!g_IsQuitting){
		// First, get the frame
		*pCap >> frame;
		if (iframeCount++ % iSkipFrame == 0){
			printf("Skipping frame %d\n", iframeCount);
			continue;
		}
		dlib::cv_image<dlib::bgr_pixel> srcImageDLIB(frame);
		float fFaceDownSample = std::max(frame.size().width, frame.size().height) > 1280 ? 6.0 : 4.0f;
		cv::resize(frame, smallFrame, 
				cv::Size(), 1.0/fFaceDownSample, 1.0/fFaceDownSample);

		dlib::cv_image<dlib::bgr_pixel> srcImageDLIBSmall(smallFrame);
		std::vector<dlib::rectangle> dets = g_faceDetector(srcImageDLIBSmall);
		printf("Number of faces detected: %d\n", (int)dets.size());
		if (dets.size() > 0){
			dlib::rectangle r(
                   (long)(dets[0].left() * fFaceDownSample),
                   (long)(dets[0].top() * fFaceDownSample),
                   (long)(dets[0].right() * fFaceDownSample),
                   (long)(dets[0].bottom() * fFaceDownSample)
      );
			dlib::full_object_detection shape = g_sp(srcImageDLIB, r);
			// this is the landmark
			// Set the weights
			estimateModelParameters(shape, frame);
		}
		std::lock_guard<std::mutex> lock(g_captureMutex);
	}
	std::cout << "Quitting calculation thread.. " << std::endl;
}


void dlib_point2cv_Point(const dlib::full_object_detection& S, std::vector<cv::Point>& L,double& scale) {
	for(unsigned int i = 0; i<S.num_parts();++i)
		L.push_back(cv::Point(S.part(i).x()*(1/scale),S.part(i).y()*(1/scale)));
}

std::vector<float> estimateModelParameters(const dlib::full_object_detection& shape, cv::Mat& image){
	// Match for each key points...
	std::vector<float> results;
	{
		std::lock_guard<std::mutex> lock(g_weightMutex);
		results = weights;
	}
	// to get pixel 0: shape.part(0), 
	if (shape.num_parts() != 68){
		printf("ERROR: wrong number of shape\n");
	}
	// first, estimate the camera pose
	// Just use the neutral mesh for now
	cv::Mat objectPoints(3, 68, CV_32F, kptPoints[0]);
	std::vector<cv::Point> landmarks,R_Eyebrow,L_Eyebrow,L_Eye,R_Eye,Mouth,Jaw_Line,Nose;
	double scale = 1.0;
	dlib_point2cv_Point(shape, landmarks, scale);

  // 2D image points. If you change the image, you need to change vector
	std::vector<cv::Point2d> imagePoints;
  imagePoints.push_back(landmarks[30]);    // Nose tip
  imagePoints.push_back(landmarks[8]);    // Chin
  imagePoints.push_back(landmarks[45]);     // Left eye left corner
  imagePoints.push_back(landmarks[36]);    // Right eye right corner
  imagePoints.push_back(landmarks[54]);    // Left Mouth corner
  imagePoints.push_back(landmarks[48]);    // Right mouth corner


	// Get corresponding 3D points
	std::vector<cv::Point3d> modelPoints;
	modelPoints.push_back(cv::Point3d(kptPoints[0][30*3], kptPoints[0][30*3+1], kptPoints[0][30*3+2]));
	modelPoints.push_back(cv::Point3d(kptPoints[0][8*3], kptPoints[0][8*3+1], kptPoints[0][8*3+2]));
	modelPoints.push_back(cv::Point3d(kptPoints[0][45*3], kptPoints[0][45*3+1], kptPoints[0][45*3+2]));
	modelPoints.push_back(cv::Point3d(kptPoints[0][36*3], kptPoints[0][36*3+1], kptPoints[0][36*3+2]));
	modelPoints.push_back(cv::Point3d(kptPoints[0][54*3], kptPoints[0][54*3+1], kptPoints[0][54*3+2]));
	modelPoints.push_back(cv::Point3d(kptPoints[0][48*3], kptPoints[0][48*3+1], kptPoints[0][48*3+2]));

	// Camera internals
	double focal_length = image.cols; // Approximate focal length.
	cv::Point2d center = cv::Point2d(image.cols/2,image.rows/2);
	cv::Mat camera_matrix = (cv::Mat_<double>(3,3) << focal_length, 0, center.x, 0 , focal_length, center.y, 0, 0, 1);
	cv::Mat dist_coeffs = cv::Mat::zeros(4,1,cv::DataType<double>::type); // Assuming no lens distortion

	// Output rotation and translation
	cv::Mat rotation_vector; // Rotation in axis-angle form
	cv::Mat translation_vector;

	// Solve for pose
	cv::solvePnP(modelPoints, imagePoints, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
	cv::Mat rotationMatrix;
	cv::Rodrigues(rotation_vector, rotationMatrix);
	rotationMatrix.convertTo(rotationMatrix, CV_32FC1);
	cv::Mat flipmatrix;
	/*
	flipmatrix	= (cv::Mat_<float>(3, 3) << 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	flipmatrix.convertTo(flipmatrix, CV_32FC1);
	std::cout << rotationMatrix.type() << " " << flipmatrix.type() << std::endl;
	rotationMatrix *= flipmatrix;
	*/
	// Check if the pose is flipped
	if (0 < rotationMatrix.at<float>(1, 1) && rotationMatrix.at<float>(1, 1) < 1){
		flipmatrix = (cv::Mat_<float>(3, 3) << 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f);
		flipmatrix.convertTo(flipmatrix, CV_32FC1);
		rotationMatrix *= flipmatrix;
	}
	{
		std::lock_guard<std::mutex> lock(g_rotationMatMutex);
		rotationMatrix.convertTo(rotationMatrix, CV_32F);
		parameterizedRotation = glm::mat4(glm::make_mat3((GLfloat*)rotationMatrix.data));
	}
	return results;
}


void initKptPoints(){
	using TBSMesh = DatCustom::Graphics::TexturedBlendShapeMesh;
	kptPoints = (GLfloat**) malloc(((TBSMesh*)theBlendShape.get())->vertices.size()*sizeof(GLfloat*));
	for (size_t i = 0; i < ((TBSMesh*)theBlendShape.get())->vertices.size(); i++){
		kptPoints[i] = (GLfloat*) malloc(68*3*sizeof(GLfloat));
		for (size_t j = 0; j < kptIndices.size(); j++){
			kptPoints[i][3*j] = ((TBSMesh*)theBlendShape.get())->vertices[i][kptIndices[j]].Position.x;
			kptPoints[i][3*j+1] = ((TBSMesh*)theBlendShape.get())->vertices[i][kptIndices[j]].Position.y;
			kptPoints[i][3*j+2] = ((TBSMesh*)theBlendShape.get())->vertices[i][kptIndices[j]].Position.z;
		}
	}
}


void init(const char* videoPath){
	if (videoPath != 0) pCap = new cv::VideoCapture(videoPath);
	else pCap = new cv::VideoCapture(0);
	if (!pCap->isOpened()){
		throw std::logic_error("Failed to open the camera\n");
	}

	std::cout << "Initializing Face detector... " << std::endl;
	g_faceDetector = dlib::get_frontal_face_detector();
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> g_sp;
	std::cout << "Opening the capture..." << std::endl;
	std::cout << "Loading BlendShape..." << std::endl;
	// Loading blendshape
	theBlendShape = MeshManager::instance().loadStaticBlendShape({"neutral_0.obj", "1_0.obj", "2_0.obj", "3_0.obj", "4_0.obj", "5_0.obj", "6_0.obj"});

	// Let's preload all keypoints
	// We sure they are textured blendshape now..
	initKptPoints();
	
	std::cout << "Loading Shader..." << std::endl;
	// Let's get the total number of frames... for now
	cubemapTexture = DatCustom::Graphics::loadCubemap(cubeFaces);
	modelShaderProgram = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
	blendShapeShaderProgram = new Shader("shaders/vertex_texture_blendshape.glsl", "shaders/fragment_texture_blendshape.glsl");
	lampProgram = new Shader("shaders/vertex.glsl", "shaders/lamp_fragment.glsl");
	textureBufferProgram = new Shader("shaders/vertex_backup.glsl", "shaders/fragment_backup.glsl");
	cubeProgram = new Shader("shaders/vertex_cube.glsl", "shaders/fragment_cube.glsl");

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER,
		 	DatCustom::Graphics::BasicGeometry::getQuadVerticesNumFloat()*sizeof(GLfloat),
		 	DatCustom::Graphics::BasicGeometry::getQuadVertices(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 	DatCustom::Graphics::BasicGeometry::getQuadIndicesNumUInt()*sizeof(GLuint),
		 	DatCustom::Graphics::BasicGeometry::getQuadIndices(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glGenBuffers(1, &lampVBO);
	glGenBuffers(1, &lampEBO);
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	// target, attachment, textarget, texture, level
	// 0. Bind VAO
	// 1. Setup buffer
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lampEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			DatCustom::Graphics::BasicGeometry::getQuadIndicesNumUInt()*sizeof(GLuint),
		 	DatCustom::Graphics::BasicGeometry::getQuadIndices(), GL_STATIC_DRAW);
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


void framebuffer_size_callback(GLFWwindow*, int width, int height){
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

int gTempModelCount = 1;


void render(){
	 sumDeltaTime += nextTime - currentTime;
	 // Let's load the ith model
		//printf("Here0\n");

	 // printf("Current Temp Model Count: %d\n", gTempModelCount);
	 // printf("Here1\n");
	 if (sumDeltaTime >= 1.0/30){
		 // reset the weights;
		 /*
		 {
			std::lock_guard<std::mutex> lock(g_weightMutex);
			 Eigen::VectorXf newWeights = Eigen::VectorXf::Random(weights.size()).array().abs();
			 newWeights /= newWeights.sum();
			 weights.assign(newWeights.data(), newWeights.data() + newWeights.size());
		 }
		 */
		 /*
	 cv::Mat imout = DatCustom::Graphics::RenderToImageManager::instance().getOutputFrame();
	 cv::imwrite((std::string("data/frame_") + std::to_string(gFrameCount) + ".jpg").c_str(), imout);
	 FILE* outCam = fopen((std::string("data/cam_") + std::to_string(gFrameCount) + ".txt").c_str(), "w");
	 fprintf(outCam, "%f %f %f\n", theCamera.Position.x, theCamera.Position.y, theCamera.Position.z);
	 fprintf(outCam, "%f %f %f\n", theCamera.Front.x, theCamera.Front.y, theCamera.Front.z);
	 fprintf(outCam, "%f %f %f\n", theCamera.Up.x, theCamera.Up.y, theCamera.Up.z);
	 fprintf(outCam, "%f %f %f\n", theCamera.Right.x, theCamera.Right.y, theCamera.Right.z);
	 fclose(outCam);
	 */
		 /*
		 printf("Updating..\n");
		 DatCustom::FaceModel::PCAFaceModelManager& tempInstance = DatCustom::FaceModel::PCAFaceModelManager::instance();
		 // Gen vectors in range 0, 1
		 srand((unsigned int) time(0));
		 Eigen::VectorXf shapeParams = (Eigen::VectorXf::Random(tempInstance.getNShapeParams(), 1) + Eigen::VectorXf::Constant(tempInstance.getNShapeParams(), 1, 1.));
		 // Eigen::VectorXf exprParams = (Eigen::VectorXf::Random(tempInstance.getNExpParams(), 1) + Eigen::VectorXf::Constant(tempInstance.getNExpParams(), 1, 1.))/1.25;
		 srand((unsigned int) time(0));
		 Eigen::VectorXf exprParams = (Eigen::VectorXf::Random(tempInstance.getNExpParams(), 1) + Eigen::VectorXf::Constant(tempInstance.getNExpParams(), 1, 1.))/80;
		 srand((unsigned int) time(0));
		 Eigen::VectorXf texParams = (Eigen::VectorXf::Random(tempInstance.getNTextureParams(), 1) + Eigen::VectorXf::Constant(tempInstance.getNTextureParams(), 1, 1.));

		 gFrameCount += 1;
		 if (gFrameCount % 120 == 0){
			 tempInstance.updateModel(shapeParams, exprParams, texParams);
		 }
		 */
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

	/*
	printf("Camera pos - dir: %f, %f ,%f - %f, %f, %f\n",
			theCamera.Position.x, theCamera.Position.y, theCamera.Position.z,
			theCamera.Front.x, theCamera.Front.y, theCamera.Front.z);
			*/

	// printf("Here1\n");
	glDisable(GL_CULL_FACE);
	draw_model(theCamera);
	glEnable(GL_CULL_FACE);
	// printf("Here2\n");
	draw_lamp(theCamera);
	draw_cubemap(theCamera);

	// 4.2 Bind to default framebuffer

	DatCustom::Graphics::RenderToImageManager::instance().stop();
	draw_framebuffer();
	// 4.3 draw quad span fullcenter with texture as bound
	glBindVertexArray(0);

	// printf("Here3\n");
	cv::Mat imout = DatCustom::Graphics::RenderToImageManager::instance().getOutputFrame();
	// printf("Here4\n");
	cv::imwrite((std::string("data/frame_") + std::to_string((gTempModelCount-1)%1000+1) + ".jpg").c_str(), imout);
	 gTempModelCount += 1;
}


void setupDirectionalLight(Shader* pProgram, GLfloat shininess, glm::vec3 dirDirection,
	 	glm::vec3 dirAmbientColor, glm::vec3 dirDiffuseColor, glm::vec3 dirSpecularColor){
	pProgram->use();
	pProgram->setFloat("material.shininess", shininess);
	pProgram->setVec3("dirlight.direction", glm::value_ptr(dirDirection));
	pProgram->setVec3("dirlight.ambient", glm::value_ptr(dirAmbientColor));
	pProgram->setVec3("dirlight.diffuse", glm::value_ptr(dirDiffuseColor));
	pProgram->setVec3("dirlight.specular", glm::value_ptr(dirSpecularColor));
}



void draw_model(Camera& theCam){
	blendShapeShaderProgram->use();
	// shaderProgram->setFloat("material.shininess", 32.0f);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	glm::vec3 lightColor;
	lightColor.x = 1.0f;
	lightColor.y = 1.0f;
	lightColor.z = 1.0f;
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.7f); // 0.5
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.3f); //0.2
	glm::vec3 lightDir(0.0f, -1.0f, -2.0f);

	setupDirectionalLight(blendShapeShaderProgram, 32.0f,
		 	lightDir, diffuseColor, ambientColor, glm::vec3(1.0f, 1.0f, 1.0f));

	for (int i = 0; i < 4; i++){
		std::string pointString = "pointLights";
		pointString  += "[" + std::to_string(i)+ "].";
		blendShapeShaderProgram->setVec3((pointString+"position"), glm::value_ptr(pointLightPositions[i]));
		blendShapeShaderProgram->setVec3((pointString+"ambient"), glm::value_ptr(ambientColor));
		blendShapeShaderProgram->setVec3((pointString+"diffuse"), glm::value_ptr(diffuseColor));
		blendShapeShaderProgram->setVec3((pointString+"specular"), 1.0f, 1.0f, 1.0f);
		blendShapeShaderProgram->setFloat((pointString+"constant"), 1.0f);
		blendShapeShaderProgram->setFloat((pointString+"linear"), 0.09f);
		blendShapeShaderProgram->setFloat((pointString+"quadratic"), 0.032f);
	}
	
	glm::mat4 model;
	glm::mat4 view = theCam.GetViewMatrix();
	// pos, target, up
	
	projection = glm::perspective(glm::radians(theCam.Zoom), (float)screenWidth/screenHeight, 0.1f, 100.0f);
	blendShapeShaderProgram->setMat4("view", glm::value_ptr(view));
	blendShapeShaderProgram->setMat4("projection", glm::value_ptr(projection));
	blendShapeShaderProgram->setVec3("viewPos", glm::value_ptr(theCam.Position));

	/*
	model = glm::rotate(model, glm::pi<float>()*3/2, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(-5.0f, -5.0f, -20.0f)); // translate it down so it's at the center of the scene
	*/
	// GLM expects the transformation to be performed bottom up (right hand side?)
	// For 3DDFA
	model = glm::translate(model, glm::vec3(-10.0f, -5.0f, -24.0f)); // translate it down so it's at the center of the scene
	/*
	model = glm::rotate(model, glm::pi<float>()/2, glm::vec3(0.0f, 0.0f, 1.0f));	
	model = glm::rotate(model, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));	
	model = glm::rotate(model, glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));	
	*/
	{
		std::lock_guard<std::mutex> lock(g_rotationMatMutex);
		model = model*parameterizedRotation;
	}
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
	/*
	// For RingNet
	model = glm::rotate(model, glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));	

	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));	// it's a bit too big for our scene, so scale it down
	// Note: the y axis is inverted...and z point in the screen
	model = glm::translate(model, glm::vec3(-4.5f, -7.5f, 14.0f)); 
	*/

	// model = glm::rotate(model, glm::pi<float>()/6, glm::vec3(0.0f, 1.0f, 0.0f));
	blendShapeShaderProgram->setMat4("model", glm::value_ptr(model));
	// DatCustom::FaceModel::PCAFaceModelManager::instance().Draw(*shaderProgram);
	std::vector<float> newWeights;
	{
		std::lock_guard<std::mutex> lock_guard(g_weightMutex);
		newWeights = weights;
	}
	theBlendShape->Draw(*blendShapeShaderProgram, blendShapeindices, newWeights);
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
		// Scale first, translate later
	 	model	= glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.1f));
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


void mouse_callback(GLFWwindow*, double xpos, double ypos){
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


void scroll_callback(GLFWwindow*, double, double yoffset){
	theCamera.ProcessMouseScroll(yoffset);
}


void terminate(){
	glDeleteBuffers(1, &lampVAO);
	glDeleteBuffers(1, &lampEBO);
	glDeleteBuffers(1, &lampVBO);
	glfwTerminate();
}
