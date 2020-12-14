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

in vec3 v_Normal;
in vec3 v_FragPos;
 
uniform vec3 viewPos; 
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform DirLight directionalLight;

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

float getFogFactor(float d)
{
    const float FogMax = 300.0;
    const float FogMin = 10.0;

    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;

    return 1 - (FogMax - d) / (FogMax - FogMin);
}

void main()
{  	
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(viewPos - v_FragPos);
    vec4 result = CalcDirLight(directionalLight, norm, viewDir);

    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, v_FragPos, viewDir);
    }    
    
    FragColor = result;

    float alpha = 1.f;
    float d = distance(viewPos, v_FragPos);
    alpha = getFogFactor(d);
    
    vec3 res=mix(vec3(result.x,result.y,result.z), vec3(result.x,result.y,result.z)*0.4, alpha);
    FragColor = vec4(res,1.0);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * light.color;
  	
    // diffuse
    vec3 lightDir = normalize(light.position - v_FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color;
    
    // specular
    float specularStrength = 0.1;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1);

    float attenuation = (1 / (light.linear * length(light.position - v_FragPos)));
    
    vec4 result = (vec4(ambient,1.0) + vec4(diffuse,1.0) + vec4(specular,1.0)) * vec4(0.31, 0.82, 0.96, 1.f) * attenuation;
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
    vec3 diffuse = diff * light.color;;
    vec3 specular = specularStrength * spec * vec3(1);;
    return (vec4(ambient, 1) + vec4(diffuse, 1) + vec4(specular, 1)) * vec4(0.31, 0.82, 0.96, 1.f);
}