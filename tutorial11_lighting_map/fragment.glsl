#version 330 core
struct Material {
		sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

uniform Material material;


struct DirectionalLight {
	vec3 direction; // Note how we replace position with direction
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform DirectionalLight dirlight;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// Ambient
	vec3 ambient = vec3(texture(material.diffuse, texCoord))* dirlight.ambient;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, texCoord))* light.diffuse;
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (vec3(texture(material.specular, texCoord)) * spec) * light.specular;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
