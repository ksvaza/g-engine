#version 330 core

in vec4 oColour;
in vec2 oTexCoords;
in vec4 oNormal;
flat in vec4 oViewDir;
flat in int oTextured;

out vec4 FragColor;

uniform sampler2D uAtlasTexture;
uniform vec3 uLightDir;

void main()
{
    //vec3 uLightDir = normalize(vec3(0.0, 0.0, 1.0));
    float lightAmbient = 0.3;
    float shine = 0.5;
    float shineMultiplier = 0.0;
    if (!isnan(normalize(oNormal).x) && !isnan(normalize(oNormal).y) && !isnan(normalize(oNormal).z)) {
        vec3 normal = normalize(vec3(oNormal));
        vec3 viewDir = normalize(vec3(oViewDir));
        vec3 lightDir = normalize(uLightDir);
        
        // Calculate reflection vector
        vec3 reflectDir = reflect(-lightDir, normal);
        
        // Calculate specular component
        float specular = max(dot(viewDir, reflectDir), 0.0);
        shineMultiplier = pow(specular, 32.0) * shine;
    }
    float lightMultiplier = (dot(normalize(vec3(oNormal)), normalize(uLightDir)) + 1.0) / 2.0;
    //lightMultiplier += shineMultiplier; // Add the specular component
    if (isnan(normalize(oNormal).x) || isnan(normalize(oNormal).y) || isnan(normalize(oNormal).z))
    {
        lightMultiplier = 0.5;
    }
    if (isnan(normalize(uLightDir).x) || isnan(normalize(uLightDir).y) || isnan(normalize(uLightDir).z))
    {
        lightMultiplier = 1.0;
    }
    float lightIntensity = lightMultiplier * (1.0 - lightAmbient) + lightAmbient;
    if (oTextured == 1)
    {
        //FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        FragColor = oColour * texture(uAtlasTexture, oTexCoords) * lightIntensity;
    }
    //else if (isnan(normalize(uLightDir).x) || isnan(normalize(uLightDir).y) || isnan(normalize(uLightDir).z))
    else if (sqrt(dot(uLightDir, uLightDir)) == 0.0)
    {
        FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
    else
    {
        //FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        FragColor = oColour * lightIntensity;
    }
}