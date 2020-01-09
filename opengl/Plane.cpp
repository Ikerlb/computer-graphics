#include "Plane.h"

Plane::Plane(){
	//ctor
}

Plane::~Plane(){
	//dtor
}

void Plane::startup(vec3 origin,vec3 normal,string id){
	this->origin=origin;
	this->normal=normal;	
	this->id=id;
}

/*float Plane::intersects(vec3 rayOrigin,vec3 rayDirection){
	float result;
	if(intersectRayPlane(rayOrigin,rayDirection,this->origin,this->normal,result)){
		//cout<<result2<<endl;
		if(result>0)
			return result;
	}
	return -1.0f;
}
*/

bool Plane::intersects(vec3 rayOrigin,vec3 rayDirection,float result){
	float denom=dot(this->normal,rayDirection);
	if(denom>1e-6){
		vec3 sub=this->origin-rayOrigin;
		result=dot(sub,this->normal)/denom;
		return (result>=0);
	}
	return false;
}
