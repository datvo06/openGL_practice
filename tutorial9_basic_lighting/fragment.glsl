#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform float ambientStrength = 0.5;
uniform float specularStrength = 0.5;
uniform sampler2D wallTexture;
uniform sampler2D faceTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	// Ambient
	vec3 ambient = ambientStrength * lightColor;
	// Diffuse
	float diff = pow(max(dot(norm, lightDir), 0.0), 2);
	vec3 diffuse = diff* lightColor;
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 12);
	vec3 specular = specularStrength * spec * lightColor;
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
