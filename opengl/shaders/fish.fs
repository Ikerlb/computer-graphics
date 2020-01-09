#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

const float ambientCoefficient=0.1;

void main(){
    // ambient
    vec3 ambient = ambientCoefficient*ambientColor;

    // diffuse
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir=normalize(-lightPos);
	vec3 diff = diffuseColor *max(dot(norm, lightDir), 0.1);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 spec = specularColor*pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 result = (ambient + diff + spec);
    FragColor = vec4(result, 1.0);
}
