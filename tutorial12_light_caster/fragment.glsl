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
	vec3 direction; // Note how we replace position with direction
	float cutOff;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Light light;
uniform DirectionalLight dirlight;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

uniform vec3 viewPos;

void main()
{
	float distance = length(FragPos - light.position);
	float positional_attentuation = 1.0f/(light.constant + light.linear*distance + light.quadratic * distance * distance);
	vec3 positional_lightDir = normalize(FragPos - light.position);
	float theta = dot(positional_lightDir, normalize(light.direction));
	if (theta < light.cutOff){
		positional_attentuation = 0.0f;
	}
	// Ambient
	vec3 directional_ambient = vec3(texture(material.diffuse, texCoord))* dirlight.ambient;
	vec3 positional_ambient = vec3(texture(material.diffuse, texCoord))* light.ambient * positional_attentuation;
	vec3 ambient = directional_ambient + positional_ambient;

	// Diffuse
	vec3 norm = normalize(Normal);
	// Directional Diffuse
	vec3 directional_lightDir = normalize(-dirlight.direction);
	float diff = max(dot(norm, directional_lightDir), 0.0);
	vec3 directional_diffuse = diff * vec3(texture(material.diffuse, texCoord))* dirlight.diffuse;
	// Positional Diffuse
	float positional_diff = max(dot(norm, positional_lightDir), 0.0);
	vec3 positional_diffuse = diff * vec3(texture(material.diffuse, texCoord))* light.diffuse * positional_attentuation;
	vec3 diffuse = directional_diffuse + positional_diffuse;

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);

	// Directional Specular
	vec3 directional_reflectDir = reflect(-directional_lightDir, norm);
	float directional_spec = pow(max(dot(viewDir, directional_reflectDir), 0.0), material.shininess);
	vec3 directional_specular = (vec3(texture(material.specular, texCoord)) * directional_spec) * dirlight.specular;

	// Positional Specular
	vec3 positional_reflectDir = reflect(-positional_lightDir, norm);
	float positional_spec = pow(max(dot(viewDir, positional_reflectDir), 0.0), material.shininess);
	vec3 positional_specular = (vec3(texture(material.specular, texCoord)) * positional_spec) * light.specular * positional_attentuation;

	vec3 specular = directional_specular + positional_specular;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
