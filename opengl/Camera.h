#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Initial camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  5.0f;
const float SENSITIVITY =  0.12f;
const float ZOOM        =  45.0f;

using namespace std;
using namespace glm;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera{
	public:
		Camera();
		~Camera();
	
		// Camera Attributes
		vec3 position;
		vec3 front;
		vec3 up;
		vec3 right;
		vec3 worldUp;

		void startup(vec3 pos=vec3(0.0f,0.0f,0.0f),vec3 upv=vec3(0.0f,1.0f,0.0f),vec3 front=vec3(0.0f,0.0f,-1.0f));
		float getZoom();
		mat4 viewMatrix();
		void processKeyboard(Camera_Movement,float);
		void processMouse(float xoffset, float yoffset,bool constrainPitch = true);
		void processMouseScroll(float yoffset);

	private:
		void updateCameraVectors();

		// Euler Angles
		float yaw;
		float pitch;
		// Camera options
		float movementSpeed;
		float mouseSensitivity;
		float zoom;
};
#endif
