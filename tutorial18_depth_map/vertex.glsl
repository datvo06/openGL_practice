#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;


out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 CamSpacePos;

void main (){
	FragPos = vec3(model * vec4(aPos, 1.0));
	CamSpacePos = view * vec4(FragPos, 1.0);
	gl_Position = projection * view * vec4(FragPos, 1.0);
	CamSpacePos = gl_Position;
	Normal = mat3(transpose(inverse(model))) * aNormal;
	texCoord = aTexCoord;
}
