#include "Window.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>


Window::Window(){
	//ctor
}

Window::~Window(){
	//dtor
}

//paths are the models paths.
void Window::startup(int window_w,int window_h,string title1,vector<string> models_paths,vector<string> shaders_paths){
	//useful control vars
	title=title1;
	width=window_w;
	height=window_h;
	firstMouse=true;
	lastX=window_w/2.0f;
	lastY=window_h/2.0f;

	//init glfw and glew
	cout<<"initializing GLFW..."<<endl;
	init_GLFW(window_w,window_h);
	cout<<"initializing GLEW..."<<endl;
	init_GLEW();

	//global opengl state
	glEnable(GL_DEPTH_TEST);
	//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//load models
	/*for(unsigned int i=0;i<models_paths.size();i++){
		cout<<"loading model in path: "<<models_paths[i]<<endl;
		Model model;
		model.startup(models_paths[i]);
		models.push_back(model);
	}*/

	//initialize terrain
	sceneSize=20;
	terrain.startup(string("textures/terrain.png"),256);
	fishManager.startup(string("obj/fish/Fish1.obj"),2,sceneSize);



	//start camera
	cout<<"initializing camera..."<<endl;
	camera.startup(vec3(0.0f, 0.0f, 0.0f));
	
	//initialize shaders
	cout<<"initializing shaders..."<<endl;
	for(int i=0;i<shaders_paths.size();i++){
		Shader shader;
		cout<<"shader: "<<shaders_paths[i]<<endl;
		shader.startup(shaders_paths[i]);
		shaders.push_back(shader);
	}

	//uniforms!
	shaders[0].use();
	//shaders[0].setUniform("lightPos",vec3(1.2f, 1.0f, 2.0f));
	shaders[0].setUniform("lightPos",vec3(0.0f,(float)sceneSize/-2.0,0.0f));
	shaders[0].setUniform("lightColor",vec3(1.0f,1.0f,1.0f));
	shaders[0].setUniform("objectColor",vec3(1.0f, 0.5f, 0.31f));

	//terrain uniforms
	shaders[1].use();
	//shaders[1].setUniform("lightPos",vec3(1.2f, 1.0f, 2.0f));
	shaders[1].setUniform("lightPos",vec3(0.0f,(float)sceneSize/-2,0.0f));
	shaders[1].setUniform("lightColor",vec3(1.0f,1.0f,1.0f));
	shaders[1].setUniform("objectColor",vec3(0.3f, 0.3f, 0.3f));


}

void Window::init_GLEW(){
	glewExperimental=GL_TRUE;
	glewInit();
}

void Window::init_GLFW(int window_w,int window_h){
	//glfw function to init
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window=glfwCreateWindow(window_w, window_h, title.c_str(), 0, 0);

	if(!window){
		cout<<"Failed to create GLFW window"<<endl;
		glfwTerminate();
		return;
	}


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetWindowUserPointer(window,this);
}

void Window::shutdown(){
	//shutdown program and maybe geometry
	glfwTerminate();
}

//TODO: Move uniforms settings to Terrain!!!
void Window::run(){
	float deltaTime=0.0f;
	float lastFrame=0.0f;
	float lastTime=glfwGetTime();
	float step=0.001f;
	float xmove=0;
	mat4 model;
	do{
		//frame logic
		float currentFrame=glfwGetTime();
		deltaTime=currentFrame-lastFrame;
		lastFrame=currentFrame;

		//input
		processInput(window,&camera,deltaTime);

		//render
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//camera matrices
		view=camera.viewMatrix();
        projection=perspective(
                radians(camera.getZoom()),
                (float)width/(float)height,
                0.1f,
                100.0f);
		
		//fish test!!
		//use shader 1
		shaders[0].use();
		shaders[0].setUniform("viewPos",camera.position);

		shaders[0].setUniform("projection",projection);
		shaders[0].setUniform("view",view);
		
		//test for fish
		shaders[0].setUniform("time",xmove);
		xmove+=step;
		if(xmove>=321768){
			xmove=0;
		}
		fishManager.update();
		fishManager.draw(shaders[0]);
		//model
		/*
		model=mat4(1.0f);
		model=translate(model,vec3(0.0f, 0.25f, 0.0f));
		model=rotate(model,3.14f,vec3(0.0,1.0,0.0));
		model=scale(model,vec3(0.05f, 0.05f, 0.05f));
		shaders[0].setUniform("model",model);
		models[0].draw(shaders[0]);*/

		//use shader2
		drawTerrain();

		//swap buffers and poll io events
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}while(!glfwWindowShouldClose(this->window));
}

void Window::drawTerrain(){
	shaders[1].use();
	mat4 model=mat4(1.0f);
	model=translate(model,vec3(0.0f,(float)sceneSize/-2,0.0f));
	model=scale(model,vec3(sceneSize,10,sceneSize));
	model=translate(model,vec3(-.5f,0.0f,0.5f));
	//cout<<to_string(model)<<endl;
	shaders[1].setUniform("viewPos",camera.position);
	shaders[1].setUniform("projection",projection);
	shaders[1].setUniform("view",view);
	shaders[1].setUniform("model",model);
	terrain.draw(shaders[1]);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Window* ww=(Window*)glfwGetWindowUserPointer(window);
	//if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_W:
				break;
			case GLFW_KEY_S:
				break;
			case GLFW_KEY_A:
				break;
			case GLFW_KEY_D:
				break;
			case GLFW_KEY_Q:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
		}
	//}
}

void processInput(GLFWwindow* window,Camera* camera,float deltaTime){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        (*camera).processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        (*camera).processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        (*camera).processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        (*camera).processKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	Window* ww=(Window*)glfwGetWindowUserPointer(window);
    if (ww->firstMouse){
        ww->lastX = xpos;
        ww->lastY = ypos;
        ww->firstMouse = false;
    }

    float xoffset = xpos - ww->lastX;
    float yoffset = ww->lastY - ypos; // reversed since y-coordinates go from bottom to top

    ww->lastX = xpos;
    ww->lastY = ypos;

    (ww->camera).processMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Window* ww=(Window*)glfwGetWindowUserPointer(window);

	(ww->camera).processMouseScroll(yoffset);
}
