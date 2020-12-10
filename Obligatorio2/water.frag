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
uniform vec3 minScene;
uniform vec3 maxScene;
uniform samplerCube skybox;

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
bool inScene(float x, float z);
vec3 ClampToUnit(vec3 point);
vec3 CalcBoxIntersection(vec3 dir, vec3 pos);
vec4 CalcResultFromLights();

void main()
{  	
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(viewPos - v_FragPos);
    vec3 reflectedViewDir = normalize(reflect(viewDir, norm));
    
    //Componente de las luces
    vec4 result = CalcResultFromLights();
    
    //Reflexion
    vec4 reflectComponent;
    //float alpha = (maxScene.y - v_FragPos.y) / reflectedViewDir.y;
    //vec3 inter = v_FragPos + reflectedViewDir * alpha;
    
    //if (inScene(inter.x, inter.z)) inter = ClampToUnit(inter);
    //else inter = CalcBoxIntersection(reflectedViewDir, v_FragPos);
    
    vec3 I = normalize(v_FragPos - viewPos);
    vec3 R = reflect(I, norm);
    reflectComponent = vec4(texture(skybox, R).rgb, 1.0);

    float fresnel = 0.15f + (1.0 - 0.15f) * pow(max(0.0, 1.0 - dot(I, norm)), 5.0);

    //inter = ClampToUnit(inter);
    //reflectComponent = texture(skybox, inter);
    //result = vec4(0.f, 0.66, 0.89, 1.f);
    
    FragColor = mix(reflectComponent, result, fresnel);
}

bool inScene(float x, float z)
{
    if (x < minScene.x) return false;
    if (z < minScene.z) return false;
    if (x > maxScene.x) return false;
    if (x > maxScene.z) return false;
    return true;
}

vec3 ClampToUnit(vec3 point)
{
    float delta_x = maxScene.x - minScene.x;
    float delta_y = maxScene.y - minScene.y;
    float delta_z = maxScene.z - minScene.z;

    //to [0, 1]
    point.x = point.x / delta_x;
    point.y = point.y / delta_y;
    point.z = point.z / delta_z;

    //to [-1, 1]
    point = (2 * point) - vec3(1.f, 1.f, 1.f);

    //Asegurarse
    if (point.x > 1) point.x = 1.f;
    if (point.x < -1) point.x = -1.f;
    if (point.y > 1) point.y = 1.f;
    if (point.y < -1) point.y = -1.f;
    if (point.z > 1) point.z = 1.f;
    if (point.z < -1) point.z = -1.f;

    return point;
}

vec3 CalcBoxIntersection(vec3 dir, vec3 pos)
{
    float alpha;
    if (abs(dir.x) >= abs(dir.z))
    {
        //derecha o izquierda
        if (dir.x >= 0) alpha = (maxScene.x - pos.x) / dir.x;
        else alpha = (minScene.x - pos.x) / dir.x;
    }
    else
    {
        //front o back
        if (dir.z >= 0) alpha = (maxScene.z - pos.z) / dir.z;
        else alpha = (minScene.z - pos.z) / dir.z;
    }

    return vec3(pos.x + alpha * dir.x, pos.y + alpha * dir.y, pos.z + alpha * dir.z);
}

vec4 CalcResultFromLights()
{
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(viewPos - v_FragPos);
    vec4 result = CalcDirLight(directionalLight, norm, viewDir);

    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, v_FragPos, viewDir);
    }

    return result;
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
    
    vec4 result = (vec4(ambient,1.0) + vec4(diffuse,1.0) + vec4(specular,1.0)) * vec4(0.1, 0.1, 0.1, 1.f) * attenuation;
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