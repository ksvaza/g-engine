#version 330 core

in vec4 oColour;
in vec2 oTexCoords;
flat in int oTexID;

out vec4 FragColor;

uniform sampler2D uTextures[32]; // sure buddy, 32 textures, why not - copilot

void main()
{
    if (oTexID < 0)
    {
        FragColor = oColour;
    }
    else
    {
        FragColor = oColour * texture(uTextures[oTexID], oTexCoords);
    }
    if (FragColor.a < 0.1)
        discard;
}