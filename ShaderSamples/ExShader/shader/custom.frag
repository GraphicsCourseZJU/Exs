#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;


uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 spotLightPos;
uniform vec3 spotLightTar;
uniform float spotLightAngle;
uniform vec3 viewPos;

uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform int shiness;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// SpotLight
	vec3 spotLightDir = normalize(spotLightPos - FragPos);
	vec3 spotLightAxis = normalize(spotLightPos - spotLightTar);
	float angle = dot(spotLightDir, spotLightAxis);
	float innerAngle = cos(spotLightAngle*0.9/180.f*3.1415926);
	float outerAngle = cos(spotLightAngle/180.f*3.1415926);
	
	// Ambient
	vec3 ambient = ambientStrength*lightColor;
	
	// Diffuse 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	if (angle > outerAngle) {												// SpotLight
		float diff = max(dot(norm, spotLightDir), 0.0);
		if (angle < innerAngle) {
			diff = (angle - outerAngle)/(innerAngle-outerAngle)*diff;
		}
		diffuse = diffuse + diff * lightColor;
	}

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);

	vec3 specular = specularStrength * spec * lightColor;
	if (angle > outerAngle) {												// SpotLight
		vec3 reflectDir = reflect(-spotLightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
		if (angle < innerAngle) {
			diff = (angle - outerAngle)/(innerAngle-outerAngle)*spec;
		}
		specular = specular + specularStrength * spec * lightColor;
	}

	color = vec4( (ambient+diffuse+specular)*objectColor , 1.0f);
}
