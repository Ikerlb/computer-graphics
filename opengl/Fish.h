#ifndef FISH_H
#define FISH_H

#include <string>
#include <iostream>
#include <vector>

#include "Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Plane.h"

using namespace std;
using namespace glm;

int flipCoin();
int randInt(int until);
float randFloat();
vec3 randPosition(int max);
vec3 randVelocity();


class Fish{
    public:
        Fish();
        ~Fish();

		vec3 velocity;
		vec3 position;
		vec3 front;
		
		string id;

        void startup(unsigned int worldSize,string id);
		bool isBlocked(vector<Plane> planes);

    //private:
};
#endif
