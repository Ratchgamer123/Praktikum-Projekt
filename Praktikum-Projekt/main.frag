#version 460 core
out vec4 fragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 lightColor;

uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	float ambientLight = 0.2;

	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - crntPos);

	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularLight = 0.5;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 16);
	float specular = specularAmount * specularLight;

	fragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambientLight) + texture(tex1, texCoord).r * specular;
}