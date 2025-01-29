#include "../include/render.hpp"
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mesh.hpp>

namespace Gengine
{
    int Renderer::DrawMesh(Mesh mesh, char wireFrame, char boundingBox)
    {

        /*float* vertexData = (float*)malloc(sizeof(float) * mesh.VertexCount * 7);
        for (int i = 0; i < mesh.VertexCount; i++)
        {
            vertexData[i * 7 + 0] = mesh.Vertices[i].x;
            vertexData[i * 7 + 1] = mesh.Vertices[i].y;
            vertexData[i * 7 + 2] = mesh.Vertices[i].z;
            vertexData[i * 7 + 3] = mesh.Vertices[i].r;
            vertexData[i * 7 + 4] = mesh.Vertices[i].g;
            vertexData[i * 7 + 5] = mesh.Vertices[i].b;
            vertexData[i * 7 + 6] = mesh.Vertices[i].a;
        }
        unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * mesh.IndexCount * 3);
        for (int i = 0; i < mesh.IndexCount; i++)
        {
            indices[i * 3 + 0] = mesh.Indices[i].I[0];
            indices[i * 3 + 1] = mesh.Indices[i].I[1];
            indices[i * 3 + 2] = mesh.Indices[i].I[2];
        }
        */
        float* vertexData = (float*)malloc(sizeof(float) * mesh.VertexCount * 7);
        unsigned int* indices = (unsigned int*)mesh.GetIndices();
        Transform transform = mesh.GetTransform();

        for (int i = 0; i < mesh.VertexCount; i++)
        {
            vertexData[i * 7 + 0] = mesh.GetVertices()[i].x * transform.scale.x + transform.position.x;
            vertexData[i * 7 + 1] = mesh.GetVertices()[i].y * transform.scale.y + transform.position.y;
            vertexData[i * 7 + 2] = mesh.GetVertices()[i].z * transform.scale.z + transform.position.z;
            vertexData[i * 7 + 3] = mesh.GetVertices()[i].r;
            vertexData[i * 7 + 4] = mesh.GetVertices()[i].g;
            vertexData[i * 7 + 5] = mesh.GetVertices()[i].b;
            vertexData[i * 7 + 6] = mesh.GetVertices()[i].a;
        }

        // Draw mesh
        // --------------------------------
        unsigned int VBO, VAO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);;
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.VertexCount * 7, vertexData, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.IndexCount * 3, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

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

        // draw bounding box
        // --------------------------------
        if (boundingBox)
        {
            AABox box = mesh.GetBoundingBox();
            float vertices[] = {
                box.x, box.y, box.z,                                        // bottom front left
                box.x + box.width, box.y, box.z,                            // bottom front right
                box.x + box.width, box.y + box.height, box.z,               // top front right
                box.x, box.y + box.height, box.z,                           // top front left
                box.x, box.y, box.z + box.depth,                            // bottom back left
                box.x + box.width, box.y, box.z + box.depth,                // bottom back right
                box.x + box.width, box.y + box.height, box.z + box.depth,   // top back right
                box.x, box.y + box.height, box.z + box.depth                // top back left
            };
            // scale and offset bounding box also
            for (int i = 0; i < 8; i++)
            {
                vertices[i * 3 + 0] = vertices[i * 3 + 0] * mesh.GetTransform().scale.x + mesh.GetTransform().position.x;
                vertices[i * 3 + 1] = vertices[i * 3 + 1] * mesh.GetTransform().scale.y + mesh.GetTransform().position.y;
                vertices[i * 3 + 2] = vertices[i * 3 + 2] * mesh.GetTransform().scale.z + mesh.GetTransform().position.z;
            }
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

            unsigned int VBO, VAO, EBO;

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glBindVertexArray(0);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        }

        return 0;
    }

    void Renderer::Clear()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
