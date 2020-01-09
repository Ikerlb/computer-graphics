#include "Shader.h"

Shader::Shader(){
	//ctor
}

Shader::~Shader(){
	//dtor
}

void Shader::startup(string path){
	//same name, but with vs extension for vertex shader and fs for fragment shader
	string vertexPath;
	string fragmentPath;
	vertexPath=path+".vs";
	fragmentPath=path+".fs";

	//first we get shaders source code
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);

	try{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode   = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}catch(ifstream::failure e){
		cout<<"Error reading shader."<<endl;
	}
	//convert to c string
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	//we then compile
	unsigned int vertex, fragment;
	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	cout<<"succesfully created program shader"<<endl;
}

void Shader::checkCompileErrors(unsigned int shader,string type){
	int success;
	char infoLog[1024];
	if (type != "PROGRAM"){
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout<<"Error compilando "<<type<<" shader\n"<<infoLog<<"\n"<<endl;
		}
	}
	else{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout<<"Error enlazando "<<type<<" shader\n"<<infoLog<<"\n"<<endl;
		}
	}
}

void Shader::use(){
	glUseProgram(ID);
}


void Shader::setUniform(const string &name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setUniform(const string &name, const vec2 &v) {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), v.x, v.y);
}

void Shader::setUniform(const string &name, const vec3 &v) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
}

void Shader::setUniform(const string &name, const vec4 &v) {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const string &name, const mat4 &m) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const string &name, const mat3 &m) {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()),1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const string &name, float val) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setUniform(const string &name, int val) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setUniform(const string &name, bool val) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setUniform(const string &name, GLuint val) {
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), val);
}

//TODO: Delete
void Shader::print_id(){
	cout<<"id "<<ID<<endl;
}
