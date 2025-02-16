#version 330 core

in vec4 oColour;
in vec2 oTexCoords;
flat in int oTexID;

out vec4 FragColor;

uniform sampler2D uTextures[32]; // sure buddy, 32 textures, why not - copilot
uniform sampler2D uFontTexture;

void main()
{
    if (oTexID >= 0)
    {
        if (oTexID == 32)
        {
            FragColor = oColour * texture(uFontTexture, oTexCoords);
        }
        else
        {
            FragColor = oColour * texture(uTextures[oTexID], oTexCoords);
        }
    }
    else
    {
        FragColor = oColour;
    }
    if (FragColor.a < 0.01)
    {
        discard;
    }
}