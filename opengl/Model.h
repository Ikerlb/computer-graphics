#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "stb_image.h"

using namespace std;
using namespace glm;

unsigned int TextureFromFile(const char* path, const string &directory=".", bool gamma = false);

class Model{
	public:
		Model();
		~Model();
		void startup(string path,unsigned int instances=1);
		void draw(Shader shader);
		void updateInstancingModels(vector<mat4> models);

	private:
		vector<Mesh> meshes;
		string directory;		
		vector<Texture> textures_loaded;
		unsigned int instancingVBO;
		unsigned int instances;

		void processNode(aiNode* node,const aiScene* scene);
		Mesh processMesh(aiMesh* mesh,const aiScene* scene);
		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
		Material loadMaterial(aiMaterial* mat);
};
#endif
