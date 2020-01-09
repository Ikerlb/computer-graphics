#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <iostream>
#include <vector>
#include <glm/gtx/intersect.hpp>

#include "Mesh.h"
#include "Shader.h"

using namespace std;
using namespace glm;

class Plane{
    public:
        Plane();
        ~Plane();

        void startup(vec3 origin,vec3 normal,string id);
        void draw(Shader shader);
		bool intersects(vec3 rayOrigin,vec3 rayDirection,float result);
		string id;

    private:
        vec3 origin;
	  	vec3 normal;	
};
#endif
