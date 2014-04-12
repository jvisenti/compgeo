#version 330 core

const int BHGLLightTypeUnknown = 0;
const int BHGLLightTypeDirectional = 1;
const int BHGLLightTypePoint = 2;
const int BHGLLightTypeSpot = 3;

struct BHGLLightInfo
{
    int type;
    bool enabled;
    vec4 ambient;
    vec4 color;
    vec3 position;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    vec3 halfVector;
    vec3 coneDirection;
    float spotCosCutoff;
    float spotExponent;
};

struct BHGLMaterialInfo
{
    vec4 emission;
    vec4 surface;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

const int MaxLights = 1;
uniform BHGLLightInfo u_Lights[MaxLights];

uniform BHGLMaterialInfo u_Material;

uniform vec3 u_EyeDirection;

in vec3 v_normal;
in vec4 v_position;

out vec4 o_frag_color;

void main()
{
    vec4 scatteredLight = vec4(0.0);
    vec4 reflectedLight = vec4(0.0);
    
    vec3 nNormal = normalize(v_normal);
    
    BHGLLightInfo lightInfo = u_Lights[0];
    
    if (lightInfo.enabled == true)
    {
        vec3 halfVector;
        vec3 lightDirection = lightInfo.position;
        float attenuation = 1.0;
        
        if (lightInfo.type == BHGLLightTypeDirectional)
        {
            halfVector = lightInfo.halfVector;
        }
        else
        {
            lightDirection = lightDirection - vec3(v_position);
            float lightDistance = length(lightDirection);
            lightDirection = lightDirection / lightDistance;
            
            attenuation = 1.0 / (lightInfo.constantAttenuation + lightInfo.linearAttenuation * lightDistance + lightInfo.quadraticAttenuation * lightDistance * lightDistance);
            
            if (lightInfo.type == BHGLLightTypeSpot)
            {
                float spotCos = dot(lightDirection, -lightInfo.coneDirection);
                
                if (spotCos < lightInfo.spotCosCutoff)
                    attenuation = 0.0;
                else
                    attenuation *= pow(spotCos, lightInfo.spotExponent);
            }
            
            halfVector = normalize(lightDirection + u_EyeDirection);
        }
        
        float diffuse = max(0.0, dot(nNormal, lightDirection));
        float specular = max(0.0, dot(nNormal, halfVector));
        
        if (diffuse == 0.0)
            specular = 0.0;
        else
            specular = pow(specular, u_Material.shininess);
        
        scatteredLight += lightInfo.ambient * u_Material.ambient * attenuation + lightInfo.color * u_Material.diffuse * diffuse * attenuation;
        reflectedLight += lightInfo.color * u_Material.specular * specular * attenuation;
    }
    
    vec3 rgb = min(u_Material.emission.rgb + u_Material.surface.rgb * scatteredLight.rgb + reflectedLight.rgb, vec3(1.0));
    
    o_frag_color = vec4(rgb, u_Material.surface.a);
}