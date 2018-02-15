#version 330 core
out vec4 FragColor;
in vec2 texCoord;


uniform sampler2D frameTexture;


void main(){
	//FragColor = mix(texture(wallTexture, texCoord), texture(faceTexture, texCoord), 0.2);
	FragColor = texture(frameTexture, texCoord);
}
