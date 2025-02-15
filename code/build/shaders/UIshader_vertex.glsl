#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in float aTexID;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec4 uColour;

out vec4 oColour;
out vec2 oTexCoords;
flat out int oTexID;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    oColour = aColor * uColour;
    oTexCoords = aTexCoords;
    oTexID = int(aTexID);
}