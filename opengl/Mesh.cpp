#include "Mesh.h"

Mesh::Mesh(){
	//ctor
}

Mesh::~Mesh(){
	//dtor
}


//TODO: 
void Mesh::startup(vector<Vertex> vtx, vector<unsigned int> idx, vector<Texture> tex,Material material,GLenum mode){
	this->vertices=vtx;
	this->indices=idx;
	this->textures=tex;
	this->material=material;
	this->mode=mode;

	initMesh();
}


void Mesh::draw(Shader shader,unsigned int instances){
	//bind textures
    unsigned int diffuseNr=1;
    unsigned int specularNr=1;
    for(unsigned int i=0;i<textures.size();i++){
        glActiveTexture(GL_TEXTURE0+i);
        string number;
        string name=textures[i].type;
        if(name=="texture_diffuse")
            number=to_string(diffuseNr++);
        else if(name=="texture_specular")
            number=to_string(specularNr++);
        shader.setUniform(("material."+name+number).c_str(),i);
        //shader.setUniform((name+number).c_str(),i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

	//set material uniforms
	shader.setUniform("ambientColor",material.ambient);
	//cout<<"ambient "<<to_string(material.ambient)<<endl;
	shader.setUniform("diffuseColor",material.diffuse);
	//cout<<"diffuse "<<to_string(material.diffuse)<<endl;
	shader.setUniform("specularColor",material.specular);
	//cout<<"specular "<<to_string(material.specular)<<endl;
	shader.setUniform("shininess",material.shininess);

	//draw mesh
	glBindVertexArray(VAO);
	//draw EBO
	if(instances>1)
		glDrawElementsInstanced(mode,indices.size(),GL_UNSIGNED_INT,0,instances);
	else
		glDrawElements(mode,indices.size(),GL_UNSIGNED_INT,0);
	//unbind VAO
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

//load and initialize all vertex data
void Mesh::initMesh(){
	
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);

	glBindVertexArray(VAO);
	//load data
	//first vertex data
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
	
	//then index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
	
	//then we set vertex attribute pointers
	//first vertex pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position));
	
	//then vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
	
	//then texture coordinates SHOULD BE 2!!!!!
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoords));
	
	//then vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,tangent));
	//then vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,bitangent));

	//unbind VAO
	glBindVertexArray(0);
}
