#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Terrain.h"
#include "FishManager.h"

#define GLEW_STATIC
using namespace std;
using namespace glm;

// Función para procesar los eventos del teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window,Camera* camera,float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//do I really need a struct for these? it seems to me that they make a lot of sense as 
//attributes of Window.
struct Matrix{
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 MVP;
    mat3 normal;
};

class Window{
    public:
		Window();
        virtual ~Window();
        void startup(int,int,string title,vector<string> models_paths,vector<string> shaders_path);
		void run();
        void shutdown();
		float lastX;
		float lastY;
		bool firstMouse;		
		Camera camera;
		mat4 view;
		mat4 projection;
		Terrain terrain;
		FishManager fishManager;
		unsigned int sceneSize;
    private:
        void init_GLEW();
        void init_GLFW(int,int);
		void drawTerrain();
        GLFWwindow *window;
        string title;
		int width;
		int height;
        Matrix matrices;
		//for now as i don't know how complex this things can get
		vector<Model> models;
		vector<Shader> shaders;
		//map<string,Shader> shaders; 
};

#endif // WINDOW_H
