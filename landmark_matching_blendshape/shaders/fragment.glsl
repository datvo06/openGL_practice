#version 330 core


struct Material {
		sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

uniform Material material;
uniform samplerCube skybox;

uniform sampler2D blend_texture_diffuse[7];
uniform sampler2D blend_texture_specular[7];
uniform float weights[7];


struct DirectionalLight {
	vec3 direction; // Note how we replace position with direction
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


uniform PointLight light;
uniform DirectionalLight dirlight;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;
in vec4 vertexColor;

uniform vec3 viewPos;
uniform bool hasTexture;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];


vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 ambient, diffuse, specular;
	if (hasTexture){
		ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
		diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));
		specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
	}
	else{
		ambient = light.ambient * vertexColor.rgb;
		diffuse = light.diffuse * diff;
		specular = light.specular * spec;
	}
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);
	vec3 ambient, diffuse, specular;
	if (hasTexture){
		ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
		diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));
		specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
	}
	else {
		ambient = light.ambient * vertexColor.rgb;
		diffuse = light.diffuse * diff;
		specular = light.specular*spec;
	}

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	// vec3 R = reflect(-viewDir, norm); for reflect
	float ratio = 1.00/1.52;
	vec3 R = refract(-viewDir, norm, ratio);
	// vec3 result = CalcDirLight(dirlight, norm, viewDir);
	vec3 result = vertexColor.rgb;
	// vec3 result = vec3(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < NR_POINT_LIGHTS; i++){
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	FragColor = vec4(result, 1.0);
	// FragColor = vec4(texture(skybox, R).rgb, 1.0f);
	// FragColor = vec4(mix(texture(skybox, R).rgb,  result, 0.2f), 1.0f);
}
