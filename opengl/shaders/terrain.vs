#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat3 normalMatrix;

uniform sampler2D texture_height;

const vec2 size=vec2(1.0,0.0);
const ivec3 off=ivec3(-1,0,1);

void main(){
	float height=texture(texture_height,aTexCoords).r-0.5;
	float s11=height;
	float s01=textureOffset(texture_height,aTexCoords,off.xy).x-0.5;
	float s21=textureOffset(texture_height,aTexCoords,off.zy).x-0.5;
	float s10=textureOffset(texture_height,aTexCoords,off.yx).x-0.5;
	float s12=textureOffset(texture_height,aTexCoords,off.yz).x-0.5;

	vec3 va=normalize(vec3(size.xy,s21-s01));
	vec3 vb = normalize(vec3(size.yx,s12-s10));

	vec3 normal=cross(va,vb);

	Normal = mat3(transpose(inverse(model))) * normal;	

	vec4 pos=vec4(aPos.x,height,aPos.z,1.0);
	FragPos=vec3(model*pos);

	gl_Position=projection*view*model*pos;
}
