#include "../include/render.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    int Renderer::DrawSprite(Sprite sprite)
    {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };
        unsigned int VBO, VAO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }
}
