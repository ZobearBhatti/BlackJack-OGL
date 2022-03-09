#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 fragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	
	float specStrength = 0.3;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specStrength * spec * lightColor;
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}