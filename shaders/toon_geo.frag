#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    //vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int ToonSwitch;
uniform int ShadowSwitch;
float ShadowCalculation(vec4 fragPosLightSpace, float dotLightNormal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0){
        projCoords.z = 1.0;
    }
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    float bias = max(0.01*(1-dotLightNormal), 0.005);
    // check whether current frag pos is in shadow
    //float shadow = currentDepth > (bias+closestDepth)  ? 1.0 : 0.0;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{
   
    vec3 result;
    vec3 color = vec3(0.5,1,0.5);
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, dot(lightDir, normal));
    float edge_detection = (dot(viewDir, normal) > 0.05) ? 1 : 0;
    if(ShadowSwitch == 1){
        result = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    }
    else{
        result = (ambient + diffuse + specular) * color;
    }
    //vec3 result = (ambient + (diffuse + specular)) * color;
    // Edge Detection
    //float edge_detection = (dot(viewDir, normal) > 0.1) ? 1 : 0;
    if(ToonSwitch == 1){
        result = result * edge_detection;
        
        // Toon Shading
        
        float intensity = dot(lightDir, normal);
        if (intensity > 0.95)
            result = vec3(1.0, 1.0, 1.0) * result;
        else if (intensity > 0.5)
            result = vec3(0.7, 0.7, 0.7) * result;
        else if (intensity > 0.05 && intensity < 0.5)
            result = vec3(0.35, 0.35, 0.35) * result;
        else if (intensity < 0.05)
            result = vec3(0.1, 0.1, 0.1) * result;
    }
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(lighting, 1.0);
    
}
