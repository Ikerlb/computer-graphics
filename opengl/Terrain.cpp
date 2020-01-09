#include "Terrain.h"

Terrain::Terrain(){
	//ctor
}

Terrain::~Terrain(){
	//dtor
}

void Terrain::startup(string heightmap,unsigned int size){
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		float step=1.0f/size;
		for(float x=0.0f;x<=1.0f;x+=step){
			for(float z=0.0f;z>=-1.0f;z-=step){
				Vertex v;
				v.position=vec3(x,0.0f,z);
				v.texCoords=vec2(x,-z);
				vertices.push_back(v);
			}
		}
		for(int i=0;i<size;i++){
			//we get to row start with i*(size+1)
			for(int j=i*(size+1);j<(i+1)*(size+1);j++){
				indices.push_back(j);
				indices.push_back(j+size+1);
			}
			//degenerate triangle
			if(i!=size){
				indices.push_back((i+1)*(size+1)+size);
				indices.push_back((i+1)*(size+1));
			}
		}

		Texture texture;
		texture.id=TextureFromFile(heightmap.c_str());
		texture.type="texture_height";
		texture.path=heightmap.c_str();
		textures.push_back(texture);

		Material material;
		material.diffuse=vec3(0.69f,0.66f,0.52f);
		material.ambient=vec3(0.69f,0.66f,0.52f);
		material.specular=vec3(0.5f,0.5f,0.5f);
		material.shininess=96.078431;

		mesh.startup(vertices,indices,textures,material,GL_TRIANGLE_STRIP);
}

void Terrain::draw(Shader shader){
	
	mesh.draw(shader);
}


