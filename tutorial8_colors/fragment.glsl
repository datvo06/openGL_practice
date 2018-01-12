#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;


uniform sampler2D wallTexture;
uniform sampler2D faceTexture;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	/*
	vec4 wallColor = texture(wallTexture, texCoord);
	vec4 faceColor = texture(faceTexture, texCoord);
	FragColor = mix(wallColor, faceColor, 0.2);//*vec4(ourColor, 1.0f);
	*/
	FragColor = vec4(lightColor * objectColor, 1.0);
}
