#include "../include/sprite.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gengine
{
    Triangle NullTriangle()
    {
        Triangle triangle;
        for (int i = 0; i < 3; ++i)
        {
            triangle.V[i].x = 0.0f;
            triangle.V[i].y = 0.0f;
            triangle.V[i].z = 0.0f;
            triangle.V[i].r = 0.0f;
            triangle.V[i].g = 0.0f;
            triangle.V[i].b = 0.0f;
            triangle.V[i].a = 0.0f;
        }
        return triangle;
    }
    Triangle Sprite::GetTriangle(int index)
    {
        Triangle triangle;
        if (index >= IndexCount)
        {
            return NullTriangle();
        }
        for (int i = 0; i < 3; i++)
        {
            if (Indicies[index].I[i] >= VertexCount)
            {
                return NullTriangle();
            }
            triangle.V[i] = Verticies[Indicies[index].I[i]];
        }
        return triangle;
    }
    Sprite::Sprite(int vertexCount, Vertex* verticies, int indexCount, Index* indicies)
    {
        VertexCount = vertexCount;
        Verticies = new Vertex[vertexCount];
        for (int i = 0; i < vertexCount; i++)
        {
            Verticies[i] = verticies[i];
        }
        IndexCount = indexCount;
        Indicies = new Index[indexCount];
        for (int i = 0; i < indexCount; i++)
        {
            Indicies[i] = indicies[i];
        }
    }
    Sprite::Sprite()
    {
        VertexCount = 0;
        Verticies = NULL;
        IndexCount = 0;
        Indicies = NULL;
    }
    Sprite::~Sprite()
    {
        delete[] Verticies;
        delete[] Indicies;
    }
    void Sprite::Default()
    {
        VertexCount = 3;
        Verticies = new Vertex[VertexCount];
        Verticies[0].x = 0.0f;
        Verticies[0].y = 0.5f;
        Verticies[0].z = 0.0f;
        Verticies[0].r = 1.0f;
        Verticies[0].g = 0.0f;
        Verticies[0].b = 0.0f;
        Verticies[0].a = 1.0f;
        Verticies[1].x = -0.5f;
        Verticies[1].y = -0.5f;
        Verticies[1].z = 0.0f;
        Verticies[1].r = 0.0f;
        Verticies[1].g = 1.0f;
        Verticies[1].b = 0.0f;
        Verticies[1].a = 1.0f;
        Verticies[2].x = 0.5f;
        Verticies[2].y = -0.5f;
        Verticies[2].z = 0.0f;
        Verticies[2].r = 0.0f;
        Verticies[2].g = 0.0f;
        Verticies[2].b = 1.0f;
        Verticies[2].a = 1.0f;
        IndexCount = 1;
        Indicies = new Index[IndexCount];
        Indicies[0].I[0] = 0;
        Indicies[0].I[1] = 1;
        Indicies[0].I[2] = 2;
    }
}        
