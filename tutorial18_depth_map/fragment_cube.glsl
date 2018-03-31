#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main(){
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);//texture(skybox, TexCoords);
}
