#version 450 core
out vec4 fragColor;

in vec3 crntPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;

uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	float ambient = 0.20f;

	vec3 normals = normalize(normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normals, lightDirection), 0.0f);

	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normals);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	fragColor = (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}