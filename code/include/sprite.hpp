#pragma once

namespace Gengine
{
    // Structures
    typedef struct {
        float x, y, z;
        float r, g, b, a;
    }Vertex;
    typedef struct {
        Vertex V[3];
    }Triangle;
    Triangle NullTriangle();
    typedef struct {
        int I[3];
    }Index;

    class Sprite
    {
    private:
        int VertexCount;
        Vertex* Verticies;
        int IndexCount;
        Index* Indicies;
    public:
        Triangle GetTriangle(int index);
        void Default();
        Sprite(int vertexCount, Vertex* verticies, int indicies, Index* indexCount);
        Sprite();
        ~Sprite();
    };
}