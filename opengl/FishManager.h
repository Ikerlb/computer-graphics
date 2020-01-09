#ifndef FISHMANAGER_H
#define FISHMANAGER_H

#include <string>
#include <iostream>
#include <vector>
#include <glm/gtx/quaternion.hpp>

#include "Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Fish.h"
#include "Plane.h"

using namespace std;
using namespace glm;

//vector<vec3> DIRS=createDirections(30,1.61803398875);

class FishManager{
	public:
		FishManager();
		~FishManager();

		const vector<vec3> DIRS=createDirections(30,1.61803398875);

		static vector<vec3> createDirections(unsigned int num,float turnFraction);
		void startup(string path,unsigned int instances,unsigned int length);
		void draw(Shader shader);
		void update();
	private:
		unsigned int length;
		vector<mat4> models;
		vector<Fish> fish;
		vector<Plane> planes;
		Model fishModel;
};
#endif
