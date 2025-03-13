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
        FragColor = oColour * texture(uAtlasTexture, oTexCoords);
    }
    else
    {
        //FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        FragColor = oColour;
    }
}