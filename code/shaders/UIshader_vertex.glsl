#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
uniform mat4 uTransform;
uniform vec4 uColour;
out vec4 oColour;

void main()
{
    gl_Position = uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    oColour = aColor * uColour;
}