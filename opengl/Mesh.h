#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

//TODO: delete
#include "glm/gtx/string_cast.hpp"

#include "Shader.h"

using namespace std;
using namespace glm;

struct Vertex {
    // position
    vec3 position;
    // normal
    vec3 normal;
    // texCoords
    vec2 texCoords;
    // tangent
    vec3 tangent;
    // bitangent
    vec3 bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

};

class Mesh{
	public:
		Mesh();
	    virtual ~Mesh();

		unsigned int VAO;

	    void startup(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures,Material mat,GLenum mode=GL_TRIANGLES);
		void draw(Shader shader,unsigned int instances=1);

	private:
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
		Material material;
		
		void initMesh();

		GLenum mode;

		unsigned int VBO;
		unsigned int EBO;
};
#endif
