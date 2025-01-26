#include "../include/render.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    int Renderer::DrawSprite()//Sprite sprite);
    {
        float vertices[9] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };
        unsigned int VBO, VAO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);

        return 0;
    }

    void Renderer::Clear()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
