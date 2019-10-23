#version 330 core
layout (location = 0) in vec3 aPos0;
layout (location = 1) in vec3 aNormal0;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aPos1;
layout (location = 4) in vec3 aNormal1;
layout (location = 5) in vec3 aPos2;
layout (location = 6) in vec3 aNormal2;
layout (location = 7) in vec3 aPos3;
layout (location = 8) in vec3 aNormal3;
layout (location = 9) in vec3 aPos4;
layout (location = 10) in vec3 aNormal4;
layout (location = 11) in vec3 aPos5;
layout (location = 12) in vec3 aNormal5;
layout (location = 13) in vec3 aPos6;
layout (location = 14) in vec3 aNormal6;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

uniform int nBlendShape;
uniform float weights[7];
uniform bool hasTexture = false;

vec3 posArray[] = vec3[7](aPos0, aPos1, aPos2, aPos3, aPos4, aPos5, aPos6);
vec3 normalArray[] = vec3[7](aNormal0, aNormal1, aNormal2, aNormal3, aNormal4, aNormal5, aNormal6);

/*
vec3 posArray[] = vec3[5](
			vec3(0.0, 0.0, 0.0),
			vec3(0.0, 0.0, 0.0),
			vec3(0.0, 0.0, 0.0),
			vec3(0.0, 0.0, 0.0),
			vec3(0.0, 0.0, 0.0));
*/


out vec2 texCoord;
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
	for (int i = 0; i < nBlendShape; i++){
		aPos += posArray[i]*weights[i];
		aNormal += mat3(transpose(inverse(model))) * normalArray[i]*weights[i];
	}
	// aPos = aPos4;
	// aNormal = aNormal4;
	// aColor = aColor4;

	// Normal will be normalized in the next step so no need to do it now..

	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	Normal = aNormal;
	vertexColor = vec4(0.0, 0.0, 0.0, 1.0f);
	texCoord = aTexCoord;
 }
