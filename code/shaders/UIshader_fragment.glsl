#version 330 core
out vec4 FragColor;
in vec4 oColour;

void main()
{
    FragColor = oColour;
}