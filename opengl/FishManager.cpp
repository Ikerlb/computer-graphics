#include "FishManager.h"

FishManager::FishManager(){
	//ctor
}

FishManager::~FishManager(){
	//dtor
}

vector<vec3> FishManager::createDirections(unsigned int num,float turnFraction){
    vector<vec3> dirs;
    for(unsigned int i=0;i<num;i++){
        float dst=(float)i/((float)num-1.0f);
        float inclination=acos(1-2*dst);
        float azimuth=2.0f*M_PI;

        float x=sin(inclination)*cos(azimuth);
        float y=sin(inclination)*sin(azimuth);
        float z=cos(inclination);

        dirs.push_back(vec3(x,y,z));
    }
    return dirs;
}

void FishManager::startup(string path,unsigned int instances,unsigned int length){
	fishModel.startup(path,instances);
	this->length=length;
	//yz first
	Plane yz1;
	yz1.startup(vec3(-length,0,0),vec3(1,0,0),"yz1");
	Plane yz2;
	yz2.startup(vec3(length,0,0),vec3(-1,0,0),"yz2");

	//xy
	Plane xy1;
	xy1.startup(vec3(0,0,-length),vec3(0,0,1),"xy1");
	Plane xy2;
	xy2.startup(vec3(0,0,length),vec3(0,0,-1),"xy2");

	//xz
	Plane xz1;
	xz1.startup(vec3(0,-length,0),vec3(0,0,1),"xz");
	Plane xz2;
	xz2.startup(vec3(0,length,0),vec3(0,0,-1),"xz");
	
	planes.push_back(yz1);
	planes.push_back(yz2);
	planes.push_back(xy1);
	planes.push_back(xy2);
	planes.push_back(xz1);
	planes.push_back(xz2);

	for(int i=0;i<instances;i++){
		Fish nFish;
		nFish.startup(length,to_string(i));
		fish.push_back(nFish);
		
		mat4 rotation=lookAt(nFish.position,nFish.position+nFish.velocity,nFish.front);
				
		nFish.front=vec3(rotation*vec4(nFish.front,1.0f));

		//WORKS!!
		//mat4 rotation=lookAtVec(nFish.velocity,vec3(0.0f,0.0f,1.0f),vec3(0.0f,1.0f,0.f));
		models.push_back(rotation);
	}
}

void FishManager::update(){
	for(int i=0;i<models.size();i++){
		vec3 acceleration=vec3(0.0f,0.0f,0.0f);
		
		mat4 model=models[i];

		//cast ray fowrard and see if it is blocked
		
		fish[i].position+=fish[i].velocity;
		models[i]=translate(models[i],vec3(0.0f,0.0f,0.001f));
		
	}
	fishModel.updateInstancingModels(models);
}

void FishManager::draw(Shader shader){
	fishModel.draw(shader);
}
