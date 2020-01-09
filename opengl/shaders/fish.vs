#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoords;
layout (location=3) in vec3 aTangent;
layout (location=4) in vec3 aBitangent;
layout (location=5) in mat4 aInstanceMatrix; 

out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;
uniform mat4 projection;

//????
uniform float time;

const float pivot=2.0;
const float wave=2.0;

vec3 side2side(vec3 v){
	v.x=v.x+cos(time);
	return v;
}

vec3 rotatexz(vec3 v){
	float pivot_angle = cos(time)*0.1*pivot;
	mat2 rotation_matrix = mat2(vec2(cos(pivot_angle), -sin(pivot_angle)), vec2(sin(pivot_angle), cos(pivot_angle)));
	v.xz=rotation_matrix*v.xz;
	return v;
}

vec3 pandownxy(vec3 v){
	float body = (v.z + 1.0) / 2.0;
	v.x=v.x+cos(time + body) * wave;
	return v;
}

void main(){
	vec3 nPos=vec3(aPos);
	nPos=rotatexz(nPos);
	nPos=side2side(nPos);
	nPos=pandownxy(nPos);

	FragPos=vec3(aInstanceMatrix*vec4(nPos,1.0));
	//FragPos=vec3(aInstanceMatrix*vec4(aPos,1.0));
	Normal=mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
	gl_Position=projection*view*aInstanceMatrix*vec4(nPos,1.0);
	//gl_Position=projection*view*aInstanceMatrix*vec4(aPos,1.0);
}
