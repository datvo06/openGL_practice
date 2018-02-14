#version 330 core
out vec4 FragColor;
in vec4 ourColor;
//in vec2 texCoord;


uniform sampler2D wallTexture;
uniform sampler2D faceTexture;


void main(){
	//FragColor = mix(texture(wallTexture, texCoord), texture(faceTexture, texCoord), 0.2);
	FragColor = ourColor;
	//FragColor = vec4(0.5f, 0.3f, 0.6f, 1.0f);
}
