#include "Fish.h"

Fish::Fish(){
	//ctor
}

Fish::~Fish(){

}

void Fish::startup(unsigned int worldSize,string id){
	this->id=id;
	velocity=randVelocity()*0.005f;
	front=vec3(0.0f,0.0f,1.0f);
	//velocity=vec3(1.0f,0.0f,0.0f)*0.005f;
	//position=randPosition(worldSize);
	position=vec3(0.0f,0.0f,0.0f);
}

bool Fish::isBlocked(vector<Plane> planes){
	for(int i=0;i<planes.size();i++){
		float t;
		if(planes[i].intersects(position,normalize(velocity),t)){
			cout<<planes[i].id<<" plane intersects with fish "<<id<<". distance: "<<t<<endl;
		}

	}
	return false;
}


int flipCoin(){
    if(rand()%2==0)
        return 1;
    return -1;
}

int randInt(int until){
    return rand()%until;
}

float randFloat(){
    return (float)rand()/(float)(RAND_MAX);
}


vec3 randPosition(int max){
    float x=flipCoin()*randFloat()*max;
    float y=flipCoin()*randFloat()*max;
    float z=flipCoin()*randFloat()*max;
    return vec3(x,y,z);
}

vec3 randVelocity(){
    float x=flipCoin()*randFloat();
    float y=flipCoin()*randFloat();
    float z=flipCoin()*randFloat();
    return vec3(x,y,z);
}
