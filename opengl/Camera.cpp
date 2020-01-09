#include "Camera.h"

Camera::Camera(){
	//ctor
}

Camera::~Camera(){
	//dtor
}

void Camera::startup(vec3 pos,vec3 upv,vec3 front){
	this->position=pos;
	this->worldUp=upv;
	this->up=upv;
	this->front=front;
	this->yaw=YAW;
	this->pitch=PITCH;
	this->mouseSensitivity=SENSITIVITY;
	this->zoom=ZOOM;
	this->movementSpeed=SPEED;


	updateCameraVectors();

}

//hypothesis:
//position+front would yield same result as 
//front?????
mat4 Camera::viewMatrix(){
	//return lookAt(position,position+front,up);
	return lookAt(position,position+front,up);
}

float Camera::getZoom(){
	return zoom;
}

void Camera::updateCameraVectors(){
	vec3 ph;
	ph.x=cos(radians(yaw))*cos(radians(pitch));
	ph.y=sin(radians(pitch));
	ph.z=sin(radians(yaw))*cos(radians(pitch));
	front=normalize(ph);
	//recalculate right and up vectors
	right=normalize(cross(front,worldUp));
	up=normalize(cross(right,front));
}

void Camera::processKeyboard(Camera_Movement direction,float deltaTime){
	float velocity=movementSpeed*deltaTime;
	if(direction==FORWARD)
		position+=front*velocity;
	if(direction==BACKWARD)
		position-=front*velocity;
	if(direction==LEFT)
		position-=right*velocity;
	if(direction==RIGHT)
		position+=right*velocity;
	//TODO: maybe delete
	//updateCameraVectors();
}

void Camera::processMouse(float xoffset, float yoffset, bool constrainPitch){
	xoffset*=mouseSensitivity;
	yoffset*=mouseSensitivity;

	this->yaw=this->yaw+xoffset;
	this->pitch=this->pitch+yoffset;

	//constrains
	if(constrainPitch){
		if(pitch>89.0f)
			pitch=89.0f;
		if(pitch<-89.0f)
			pitch=-89.0f;
	}

	//update
	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset){
	if(zoom>=1.0f && zoom<=45.0f)
		zoom-=yoffset;
	if(zoom<=1.0f)
		zoom=1.0f;
	if(zoom>=45.0f)
		zoom=45.0f;
}
