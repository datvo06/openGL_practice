#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 texCoord;


uniform sampler2D wallTexture;
uniform sampler2D faceTexture;

void main(){
	vec4 faceColor = texture(faceTexture, texCoord);
	vec4 wallColor = texture(wallTexture, texCoord);
	FragColor = wallColor;
}
