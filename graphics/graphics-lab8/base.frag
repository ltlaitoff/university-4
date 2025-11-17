#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

in vec2 TextureCoord;
in vec3 FragmentPos;
in vec3 Normal;

out vec4 FragColor;

#define NR_POINT_LIGHTS 4

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir); 
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragmentPos);

	vec3 result = calculateDirectionalLight(dirLight, normal, viewDir);
	
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += calculatePointLight(pointLights[i], normal, FragmentPos, viewDir);
	}

	result += calculateSpotLight(spotLight, normal, FragmentPos, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoord));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoord));

	ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoord));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoord));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoord));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoord));

	return ambient + diffuse + specular;
}