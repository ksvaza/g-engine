#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec2 aTexOffset;
layout (location = 4) in vec2 aTexScale;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec4 uColour;
uniform vec2 uAtlasScale;

out vec4 oColour;
out vec2 oTexCoords;
flat out int oTextured;
flat out vec4 aColour;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    oColour = aColor * uColour;
    aColour = aColor;
    oTexCoords = (aTexCoords * aTexScale + aTexOffset) / uAtlasScale;
    if (aTexScale.x > 0.0 && aTexScale.y > 0.0)
    {
        oTextured = 1;
    }
    else
    {
        oTextured = 0;
    }
}