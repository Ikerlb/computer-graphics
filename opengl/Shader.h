#ifndef PROGRAMSHADER_H
#define PROGRAMSHADER_H

//#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>


using namespace std;
using namespace glm;

class Shader
{
public:
	Shader();
    virtual ~Shader();
    
	void startup(string path);
	// use/activate the shader
    void use();
    // utility uniform functions
    void setUniform(const string &name, float x, float y, float z);
    void setUniform(const string &name, const vec2 &v);
    void setUniform(const string &name, const vec3 &v);
    void setUniform(const string &name, const vec4 &v);
    void setUniform(const string &name, const mat4 &m);
    void setUniform(const string &name, const mat3 &m);
    void setUniform(const string &name, float val);
    void setUniform(const string &name, int val);
    void setUniform(const string &name, bool val);
    void setUniform(const string &name, GLuint val);
	void print_id();

private:
    // the program ID
    unsigned int ID;
    //uniform and its id
    map<string, GLint> uniforms;
    void checkCompileErrors(unsigned int shader, string type);

};
#endif
