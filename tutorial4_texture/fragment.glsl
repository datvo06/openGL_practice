#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

// texture samplers
uniform sampler2D wallTexture;
uniform sampler2D faceTexture;

void main()
{
	// Some comment
	vec4 wallColor = texture(wallTexture, texCoord);
	FragColor = wallColor;
}
