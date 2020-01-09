#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model(){
	//ctor
}

Model::~Model(){
	//dtor
}

void Model::startup(string path,unsigned int instances){
	this->instances=instances;
	vector<mat4> instancingModels;
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);	
	//const aiScene* scene = importer.ReadFile(path,0);	
	// check for errors
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		cout<<"ERROR::ASSIMP::"<<importer.GetErrorString()<<endl;
		return;
	}

	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);	

	if(instances>1){
		for(int i=0;i<instances;i++)
			instancingModels.push_back(mat4(1));
		//we initialize model matrices on identity;
		glGenBuffers(1, &instancingVBO);
		glBindBuffer(GL_ARRAY_BUFFER,instancingVBO);
		glBufferData(GL_ARRAY_BUFFER,instances*sizeof(glm::mat4),&instancingModels[0],GL_DYNAMIC_DRAW);
		for(int i=0;i<meshes.size();i++){
			unsigned int VAO = meshes[i].VAO;
			glBindVertexArray(VAO);

			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glVertexAttribDivisor(5,1);
			
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glVertexAttribDivisor(6,1);

			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2*sizeof(glm::vec4)));
			glVertexAttribDivisor(7,1);
		
			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3*sizeof(glm::vec4)));
			glVertexAttribDivisor(8,1);


			glBindVertexArray(0);
	
		}
	}
}

//THEORY: i don't need to bind vao after updating vbo
void Model::updateInstancingModels(vector<mat4> models){
	//not sure if it causes problem if not but, check if its the same size
	if(instances==models.size()){
		//instancingModels=models;
		glBindBuffer(GL_ARRAY_BUFFER,instancingVBO);
		//glBufferData(GL_ARRAY_BUFFER,instances*sizeof(glm::mat4),&instancingModels[0],GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(mat4),&models[0]);
		glBufferData(GL_ARRAY_BUFFER,instances*sizeof(glm::mat4),&models[0],GL_DYNAMIC_DRAW);
	}

}


void Model::processNode(aiNode* node,const aiScene* scene){
	//process each node's meshes
	for(unsigned int i=0;i<node->mNumMeshes;i++){
		aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh,scene));
	}
	//after we've processed all of the meshes we then recursively process each child
	for(unsigned int i=0;i<node->mNumChildren;i++){
		processNode(node->mChildren[i],scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
	//data to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh myMesh;

	//iterate each vertex
	for(unsigned int i=0;i<mesh->mNumVertices;i++){
		Vertex vertex;
		vec3 placeholder;

		//position
		placeholder.x=mesh->mVertices[i].x;
		placeholder.y=mesh->mVertices[i].y;
		placeholder.z=mesh->mVertices[i].z;
		vertex.position=placeholder;

		//normals
		//TODO: this sucks? do better
		if((*mesh).HasNormals()){
			placeholder.x=mesh->mNormals[i].x;
			placeholder.y=mesh->mNormals[i].y;
			placeholder.z=mesh->mNormals[i].z;
			vertex.normal=placeholder;
		}

		//texture coords
		//does the mesh have any tcoords?
		if(mesh->mTextureCoords[0]){	
			vec2 vec;
			//a vertex can contain up to 8 different tcoords. 
			//this, however, only handles one pair of coords
			vec.x=mesh->mTextureCoords[0][i].x;
			vec.y=mesh->mTextureCoords[0][i].y;
			vertex.texCoords=vec;
		}
		else
			vertex.texCoords=vec2(0.0f,0.0f);

		if((*mesh).HasTangentsAndBitangents()){
			//tangent
			placeholder.x=mesh->mTangents[i].x;
			placeholder.y=mesh->mTangents[i].y;
			placeholder.z=mesh->mTangents[i].z;
			vertex.tangent=placeholder;

			//bitangets
			placeholder.x=mesh->mBitangents[i].x;
			placeholder.y=mesh->mBitangents[i].y;
			placeholder.z=mesh->mBitangents[i].z;
			vertex.bitangent=placeholder;
		}
		//cout<<"v "<<vertex.position.x<<" "<<vertex.position.y<<" "<<vertex.position.z<<endl;
		//cout<<"vn "<<vertex.normal.x<<" "<<vertex.normal.y<<" "<<vertex.normal.z<<endl;
		vertices.push_back(vertex);
	}
	//now iterate each mesh face and get vertex indices
	for(unsigned int i=0;i<mesh->mNumFaces;i++){
		aiFace face=mesh->mFaces[i];
		for(unsigned int j=0;j<face.mNumIndices;j++)
			indices.push_back(face.mIndices[j]);
	}


	//process material
	aiMaterial* material=scene->mMaterials[mesh->mMaterialIndex];

	Material nMaterial;
	nMaterial=loadMaterial(material);

	//difuse: texture_diffuseN
	//specular: texture_specularN
	//normal: texture_normalN

	//1. diffuse maps
	vector<Texture> diffuseMaps=loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
	textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());

	//2. specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	//3. normal maps
	vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	
	//4. height maps
	vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	myMesh.startup(vertices,indices,textures,nMaterial);
	/*
    for(unsigned int i=0;i<vertices.size();i++){
        Vertex v;
        vec3 pos=v.position;
        cout<<"v "<<pos.x<<" "<<pos.y<<" "<<pos.z<<endl;
    }
	*/

	return myMesh;
}

Material Model::loadMaterial(aiMaterial* mat){
	Material material;
	aiColor3D color(0.f,0.f,0.f);
	float shininess;
	//diffuse component
	mat->Get(AI_MATKEY_COLOR_DIFFUSE,color);
	material.diffuse=vec3(color.r,color.b,color.g);
	//ambient component
	mat->Get(AI_MATKEY_COLOR_AMBIENT,color);
	material.ambient=vec3(color.r,color.b,color.g);
	//specular
	mat->Get(AI_MATKEY_COLOR_SPECULAR,color);
	material.specular=vec3(color.r,color.b,color.g);
	//shininess
	mat->Get(AI_MATKEY_SHININESS, shininess);
	material.shininess=shininess;
	
	return material;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat,aiTextureType type,string typeName){
	vector<Texture> textures;
	for(unsigned int i=0;i<mat->GetTextureCount(type);i++){
		aiString str;
		mat->GetTexture(type,i,&str);
		//check if texture was loaded before and if so, continue to next iteration
		bool skip=false;
		for(unsigned int j=0;j<textures_loaded.size();j++){
			if(strcmp(textures_loaded[j].path.data(),str.C_Str())==0){
				textures.push_back(textures_loaded[j]);
				skip=true;
				break;
			}
		}
		if(!skip){
			Texture texture;
			texture.id=TextureFromFile(str.C_Str(),this->directory);
			texture.type=typeName;
			texture.path=str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

//TODO: Triangle fan
void Model::draw(Shader shader){
	//cout<<instances<<endl;
	//cout<<to_string(instancingModels[0])<<endl;
	for(unsigned int i=0;i<meshes.size();i++)
		meshes[i].draw(shader,instances);
}

unsigned int TextureFromFile(const char* path, const string &directory, bool gamma){
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data){
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else{
		cout << "Texture failed to load at path: " << path << endl;
		stbi_image_free(data);
	}

	return textureID;
}
