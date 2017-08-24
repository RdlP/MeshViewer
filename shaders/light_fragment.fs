#version 410 core
//in vec2 UV;
in vec3 E, N, P, H;
out vec4 finalColor;
//uniform sampler2D sampler;

struct AmbientLight{
	vec4 color;
	float intensity;
};

struct DirectionalLight{
	vec4 color;
	float intensity;
	vec3 position;
};

struct PointLight{
	vec4 color;
	float intensity;
	vec3 position;
	float linearAttenuation;
	float quadraticAttenuation;
	float constantAttenuation;
};

struct SpotLight{
	vec4 color;
	float intensity;
	vec3 positon;
	vec3 coneDirection;
	float spotCosCutOff;
	float spotExponent;
	float linearAttenuation;
	float quadraticAttenuation;
	float constantAttenuation;
};

struct DiffuseMaterial{
	vec4 diffuseColor;
};

struct PhongMaterial{
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
};

//int MAX_AMBIENT_LIGHT = 1;
AmbientLight ambientLights[1];
//uniform AmbientLight ambientLights[1];

//int MAX_DIRECTIONAL_LIGHT = 1;
DirectionalLight directionalLights[1];
//uniform DirectionalLight directionalLights[1];

//const int MAX_POINT_LIGHT = 1;
//uniform PointLight pointLights[1];
//uniform PointLight pointLights[1];

//const int MAX_SPOT_LIGHT = 1;
//uniform SpotLight spotLights[1];
//uniform SpotLight spotLights[1];

DiffuseMaterial material;

vec4 calculePointLight(int index, PointLight light, vec3 N){
	vec3 L;
	vec4 diffuse;
	float Kd;
	float dist;
	float att;
	dist = distance(P,light.position);
	att = 1.0/(light.constantAttenuation + light.linearAttenuation * dist + light.quadraticAttenuation * dist * dist);
	L = normalize(light.position.xyz - P);
	Kd = max(dot(L,N),0.0);
	diffuse = Kd * light.color * light.intensity * material.diffuseColor;
	return diffuse * att;
}

vec4 calculeDirectionalLight(int index, DirectionalLight light, vec3 N){
	vec3 L;
	vec4 diffuse;
	float Kd;
	L = normalize(light.position.xyz);
	Kd = max(dot(L,N),0.0);
	diffuse = Kd * light.color * light.intensity * material.diffuseColor;
	return diffuse;
}



void main(){
	//AmbientLight ambientLights[1];
	//DirectionalLight directionalLights[1];
	material.diffuseColor = vec4(0.5,0.5,0.5,1);
	ambientLights[0].color = vec4(0.0,0.0,0.0,1);
	ambientLights[0].intensity = 1.0;

	directionalLights[0].color = vec4(0.6,0.6,0.6,1);
	directionalLights[0].position = vec3(0,0,-1);
	directionalLights[0].intensity = 1.0;
	vec3 PN = N;
	vec4 fColor = vec4(0.0,0.0,0.0,0.0);
	for (int i = 0; i < 1; i++){
		fColor += ambientLights[i].color;
	}

	for (int i = 0; i < 1; i++){
		fColor += calculeDirectionalLight(i, directionalLights[i], PN);
	}

	/*for (int i = 0; i < MAX_POINT_LIGHT; i++){
		fColor += calculePointLight(i, pointLights[i], PN);
	}

	for (int i = 0; i < MAX_SPOT_LIGHT; i++){
		fColor += calculeSpotLight(i, spotLights[i], PN);
	}*/
	finalColor = fColor;
  //color = vec3(UV,1);
  //color = vec3(1,0,0);
  //color = texture(sampler,UV).rgb;
}