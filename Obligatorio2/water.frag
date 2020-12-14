#version 400 core

const float Eta = 0.15;

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
in vec2 v_TexCoords;
 
uniform vec3 viewPos; 
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform DirLight directionalLight;
uniform samplerCube skybox;
uniform mat4 worldToUnitMatrix;

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 CalcReflectionComponent(vec3 normal, vec3 viewPos, vec3 point);

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
    vec4 lightComponent = CalcDirLight(directionalLight, norm, viewDir);

    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        lightComponent += CalcPointLight(pointLights[i], norm, v_FragPos, viewDir) / float(NUM_POINT_LIGHTS + 1);
    }

    vec4 reflectionComponent = CalcReflectionComponent(norm, viewPos, v_FragPos);
    
    float fresnel = Eta + (1.0 - Eta) * pow(max(0.0, 1.0 - dot(viewDir, norm)), 5.0);

    FragColor = mix(lightComponent, reflectionComponent*0.6, fresnel);
}

vec4 CalcReflectionComponent(vec3 normal, vec3 viewPos, vec3 point)
{
    vec3 incident = point - viewPos;
    vec3 reflectionDir = normalize(reflect(incident, normal));

    vec3 unitReflectedDir = vec3(worldToUnitMatrix * vec4(reflectionDir, 1.f));
    vec3 unitPoint = vec3(worldToUnitMatrix * vec4(point, 1.f));
    float alpha = 1000000000.f;

    //punto = unitPoint + unitReflectedDir * alpha
    //alpha = (punto - unitPoint) / unitReflectedDir

    //x = 1
    float alpha_1 = (1 - unitPoint.x) / unitReflectedDir.x;
    if (alpha_1 > 0 && alpha_1 < alpha) alpha = alpha_1;

    //x = -1
    float alpha_2 = (-1 - unitPoint.x) / unitReflectedDir.x;
    if (alpha_2 > 0 && alpha_2 < alpha) alpha = alpha_2;

    //y = 1
    float alpha_3 = (1 - unitPoint.y) / unitReflectedDir.y;
    if (alpha_3 > 0 && alpha_3 < alpha) alpha = alpha_3;

    //y = -1
    float alpha_4 = (-1 - unitPoint.y) / unitReflectedDir.y;
    if (alpha_4 > 0 && alpha_4 < alpha) alpha = alpha_4;

    //z = 1
    float alpha_5 = (1 - unitPoint.z) / unitReflectedDir.z;
    if (alpha_5 > 0 && alpha_5 < alpha) alpha = alpha_5;

    //z = -1
    float alpha_6 = (-1 - unitPoint.z) / unitReflectedDir.z;
    if (alpha_6 > 0 && alpha_6 < alpha) alpha = alpha_6;

    if (alpha == 1000000000.f) alpha = 0.f;

    vec3 skyboxCoords = unitPoint + alpha * unitReflectedDir;

    return texture(skybox, skyboxCoords);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * light.color;
  	
    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color;
    
    // specular
    float specularStrength = 0;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1);

    float attenuation = (1 / (light.linear * length(light.position - fragPos)));
    
    vec4 result = (vec4(ambient,1.0) + vec4(diffuse,1.0) + vec4(specular,1.0)) * vec4(0.023f, 0.258f, 0.450f, 1.f) * attenuation;
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
    return (vec4(ambient, 1) + vec4(diffuse, 1) + vec4(specular, 1)) * vec4(0.023f, 0.258f, 0.450f, 1.f);
}