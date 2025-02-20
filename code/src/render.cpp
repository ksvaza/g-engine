#include "../include/render.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mesh.hpp>
#include <shader.hpp>

namespace Gengine
{
    int Renderer::DrawMesh(Mesh mesh, char wireFrame, Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Texture font)
    {
        float* vertexData = (float*)mesh.GetVertices();
        unsigned int* indices = (unsigned int*)mesh.GetIndices();

        shader.Use();
        shader.SetUniformMat4("uModel", TransformToMatrix(mesh.GetTransform()));
        shader.SetUniformMat4("uView", viewMatrix);
        shader.SetUniformMat4("uProjection", projectionMatrix);
        shader.SetUniformVec4("uColour", mesh.GetColour());

        // Draw mesh
        // --------------------------------
        unsigned int VBO, VAO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);;
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.VertexCount * 10, vertexData, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.IndexCount * 3, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));
        glEnableVertexAttribArray(3);

        GLint samplers[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

        if (mesh.TextureCount > 16) { shader.SetUniform1iv("uTextures", 16, samplers); }
        else { shader.SetUniform1iv("uTextures", mesh.TextureCount, samplers); }

        for (int i = 0; i < mesh.TextureCount && i < 16; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            if (!mesh.GetTexture(i).textureID)
            {
                //printf("Empty Texture!\t");
                continue;
            }
            glBindTexture(GL_TEXTURE_2D, mesh.GetTexture(i).textureID);
        }
        /*for (int i = mesh.TextureCount; i < 32; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0); // Or bind a default texture.
        }*/

        shader.SetUniform1i("uFontTexture", 16);
        glActiveTexture(GL_TEXTURE0 + 16);
        glBindTexture(GL_TEXTURE_2D, font.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // set wireframe mode if needed
        if (wireFrame)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glDrawElements(GL_TRIANGLES, mesh.IndexCount * 3, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);

        return 0;
    }
    int Renderer::DrawBoundingBox(Mesh mesh, Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        AABox box = mesh.GetBoundingBox();
        float vertices[] = {
            box.x, box.y, box.z, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,                                        // bottom front left
            box.x + box.width, box.y, box.z, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,                            // bottom front right
            box.x + box.width, box.y + box.height, box.z, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,               // top front right
            box.x, box.y + box.height, box.z, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,                           // top front left
            box.x, box.y, box.z + box.depth, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,                            // bottom back left
            box.x + box.width, box.y, box.z + box.depth, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,                // bottom back right
            box.x + box.width, box.y + box.height, box.z + box.depth, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,   // top back right
            box.x, box.y + box.height, box.z + box.depth, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0                // top back left
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
            1, 5, 6,
            6, 2, 1,
            7, 6, 5,
            5, 4, 7,
            4, 0, 3,
            3, 7, 4,
            4, 5, 1,
            1, 0, 4,
            3, 2, 6,
            6, 7, 3
        };

        shader.Use();
        shader.SetUniformMat4("uModel", TransformToMatrix(mesh.GetTransform()));
        shader.SetUniformMat4("uView", viewMatrix);
        shader.SetUniformMat4("uProjection", projectionMatrix);
        shader.SetUniformVec4("uColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        // Draw bounding box
        // --------------------------------
        unsigned int VBO, VAO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);

        return 0;
    }
    void Renderer::Clear(glm::vec4 colour)
    {
        glClearColor(colour.r, colour.g, colour.b, colour.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
