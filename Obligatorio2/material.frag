#version 400 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;
	vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

#define NUM_POINT_LIGHTS 3

flat in vec3 Normal;  
in vec3 FragPos; 
in vec2 TexCoords;

uniform vec3 viewPos;
uniform float diffuser;
uniform float diffuseg;
uniform float diffuseb; 
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform DirLight directionalLight;
uniform sampler2D texture_diffuse1;

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

float getFogFactor(float d)
{
    const float FogMax = 120.0;
    const float FogMin = 10.0;

    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;

    return 1 - (FogMax - d) / (FogMax - FogMin);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec4 result = CalcDirLight(directionalLight, norm, viewDir);
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    
    float alpha = 1.f;
    float d = distance(viewPos, FragPos);
    alpha = getFogFactor(d);
    
    vec3 res=mix(vec3(result.x,result.y,result.z), vec3(0.42,0.45,0.5), alpha);
    FragColor = vec4(res,1.0);
} 


vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * light.color;
  	
    // diffuse
    vec3 lightDir = normalize(light.position - FragPos);
   // float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(diffuser,diffuseg,diffuseb);
    
    // specular
    float specularStrength = 0.1;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1);

    float attenuation = (1 / (light.linear * length(light.position - FragPos)));
    
    vec4 result = (vec4(diffuse,0.1)) * attenuation*0.7;
    return result;
}

// calculates the color when using a directional light.
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ambient shading
    float ambientStrength = 0.2;    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specularStrength = 0.1;

    // combine results
    vec3 ambient =  ambientStrength * light.color;
    vec3 diffuse = vec3(diffuser,diffuseg,diffuseb)*0.7;
    vec3 specular = specularStrength * spec * vec3(1);;
    return (vec4(diffuse, 1));
}
