#version 400 core
out vec4 FragColor;

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

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{  	
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(viewPos - v_FragPos);
    vec4 result = vec4(0);
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, v_FragPos, viewDir);
    }    
    
    FragColor = result;
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