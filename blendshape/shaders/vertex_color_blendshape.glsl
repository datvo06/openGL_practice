#version 330 core
layout (location = 0) in vec3 aPos0;
layout (location = 1) in vec3 aNormal0;
layout (location = 2) in vec4 aColor0;
layout (location = 3) in vec3 aPos1;
layout (location = 4) in vec3 aNormal1;
layout (location = 5) in vec4 aColor1;
layout (location = 6) in vec3 aPos2;
layout (location = 7) in vec3 aNormal2;
layout (location = 8) in vec4 aColor2;
layout (location = 9) in vec3 aPos3;
layout (location = 10) in vec3 aNormal3;
layout (location = 11) in vec4 aColor3;
layout (location = 12) in vec3 aPos4;
layout (location = 13) in vec3 aNormal4;
layout (location = 14) in vec4 aColor4;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

uniform int nBlendShape;
uniform int weights[5];
uniform bool hasTexture = false;

vec3 posArray[] = [aPos0, aPos1, aPos2, aPos3, aPos4];
vec4 colorArray[] = [aColor0, aColor1, aColor2, aColor3, aColor4];
vec3 normalArray[] = [aNormal0, aNormal1, aNormal2, aNormal3, aNormal4];

out vec3 Normal;
out vec3 FragPos;
out vec4 vertexColor;

void main (){
	// First, normalize weights
	int sumW = 0;
	/* // Uncomment if weights need to be normlalized
	for (int i = 0; i < nBlendShape; i++){
		sumW = sumW + weights[i];
	}
	for (int i = 0; i < nBlendShape; i++){
		weights[i] = weights[i]/sumW;
	}
	*/
	// The positions are just sumarization of the weighted
	vec3 aPos = vec3(0.0, 0.0, 0.0);
	vec3 aNormal = vec3(0.0, 0.0, 0.0);
	vec4 aColor = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < nBlendShape; i++){
		aPos += posArray[i]*weights[i];
		aColor += colorArray[i]*weights[i];
		aNormal += mat3(transpose(inverse(model))) * normalArray[i]*weights[i];
	}
	// Normal will be normalized in the next step so no need to do it now..

	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	Normal = aNormal;
	texCoord = aTexCoord;
	vertexColor = aColor;
}
