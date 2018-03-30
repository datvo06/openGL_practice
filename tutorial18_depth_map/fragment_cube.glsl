#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main(){
	FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);//texture(skybox, TexCoords);
}
