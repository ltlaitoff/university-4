#version 330 core
out vec4 FragColor;

in vec3 outColor;
in vec2 TextureCoord;
in vec3 FragmentPos;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

float specularStrength = 0.5;
float ambientStrenght = 0.3;

void main() {
	vec3 objectColor = vec3(texture(ourTexture, TextureCoord));

	vec3 ambient = ambientStrenght * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragmentPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragmentPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}