#ifndef __CAMERA_OGL_H
#define __CAMERA_ORL_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera{
	public:
		// Camera Attribute
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		// Euler Angles
		float Yaw;
		float Pitch;
		// Camera Options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		// Constructor with vectors;
		/**
		 * @brief initialize camera
		 * @param position : vec3 start position
		 * @param up : vec3 up position
		 * @param yaw : float init yaw
		 * @param pitch: float init pitch
		 */
		Camera(glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3 up=glm::vec3(0.0f, 1.0f, 0.0f),
				float yaw = YAW, float pitch = PITCH):
			Front(glm::vec3(0.0f,0.0f, -1.0f)), MovementSpeed(SPEED),
			MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
				Position = position;
				WorldUp = up;
				Yaw = yaw;
				Pitch = pitch;
				UpdateCameraVectors();
			}
		// Constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY,
				float upZ, float yaw, float pitch): Front(glm::vec3(0.0f,0.0f, -1.0f)), MovementSpeed(SPEED),
			MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
				Position = glm::vec3(posX, posY, posZ);
				WorldUp = glm::vec3(upX, upY, upZ);
				Yaw = yaw;
				Pitch = pitch;
				UpdateCameraVectors();
			}
		glm::mat4 GetViewMatrix(){
			return glm::lookAt(Position, Position + Front, Up);
		}
		void ProcessKeyboard(CameraMovement direction, float deltaTime){
			float velocity = MovementSpeed*deltaTime;
			if(direction == FORWARD)
				Position += Front * velocity;
			if(direction == BACKWARD)
				Position -= Front * velocity;
			if(direction == LEFT)
				Position -= Right * velocity;
			if(direction == RIGHT)
				Position += Right * velocity;
		}
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true){
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;
			Yaw += xoffset;
			Pitch += yoffset;
			if (constrainPitch){
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}
			UpdateCameraVectors();
		}
	void ProcessMouseScroll(float yoffset){
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom =1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}
private:
	void UpdateCameraVectors(){
		glm::vec3 front;
		front.y = sin(glm::radians(Pitch));
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif
