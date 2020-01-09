#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include <iostream>
#include <vector>

#include "Model.h"
#include "Mesh.h"
#include "Shader.h"

using namespace std;
using namespace glm;

class Terrain{
	public:
		Terrain();
		~Terrain();

		void startup(string heightmap,unsigned int size);
		void draw(Shader shader);

	private:
		mat4 model;
		Mesh mesh;
};
#endif
