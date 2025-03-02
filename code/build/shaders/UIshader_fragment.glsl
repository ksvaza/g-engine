#version 330 core

in vec4 oColour;
in vec2 oTexCoords;
flat in int oTextured;
flat in vec4 aColour;

out vec4 FragColor;

uniform sampler2D uAtlasTexture;

void main()
{
    if (oTextured == 1)
    {
        //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        // Check if the surrounding pixels have very different alpha values, to then take the average colour of the surrounding pixels with greater alpha values

        // vec2 texelSize = 1.0 / textureSize(uAtlasTexture, 0);
        // vec4 centre = texture(uAtlasTexture, oTexCoords);
        // vec4 top = texture(uAtlasTexture, oTexCoords + vec2(0.0, texelSize.y));
        // vec4 bottom = texture(uAtlasTexture, oTexCoords - vec2(0.0, texelSize.y));
        // vec4 left = texture(uAtlasTexture, oTexCoords - vec2(texelSize.x, 0.0));
        // vec4 right = texture(uAtlasTexture, oTexCoords + vec2(texelSize.x, 0.0));
        // float edgeThreshold = 0.7;
        // if (abs(centre.a - top.a) > edgeThreshold ||
        //     abs(centre.a - bottom.a) > edgeThreshold ||
        //     abs(centre.a - left.a) > edgeThreshold ||
        //     abs(centre.a - right.a) > edgeThreshold)
        // {
        //     centre = (centre + top + bottom + left + right) / 5.0;
        //     vec4 weightedSum = centre;
        //     float totalWeight = 1.0;
        //     if (top.a > centre.a) {
        //         weightedSum += top;
        //         totalWeight += 1.0;
        //     }
        //     if (bottom.a > centre.a) {
        //         weightedSum += bottom;
        //         totalWeight += 1.0;
        //     }
        //     if (left.a > centre.a) {
        //         weightedSum += left;
        //         totalWeight += 1.0;
        //     }
        //     if (right.a > centre.a) {
        //         weightedSum += right;
        //         totalWeight += 1.0;
        //     }
        //     centre = weightedSum / totalWeight;
        // }
        // FragColor = oColour * centre;
    
        FragColor = oColour * texture(uAtlasTexture, oTexCoords);
    }
    else
    {
        //FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        FragColor = oColour;
    }
    if (oTexCoords.x < 0.0)
    {
        FragColor.r = 1.0;
    }
    if (oTexCoords.y < 0.0)
    {
        FragColor.g = 1.0;
    }
    if (FragColor.a < 0.25)
    {
        discard;
    }
}