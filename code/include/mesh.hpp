#pragma once
#include <stdlib.h>
#include <glm/glm.hpp>
#include <transform.hpp>

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
    static const Triangle NullTriangle = {
        {
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
        }
    };
    typedef struct {
        unsigned int I[3];
    }Index;

    typedef struct {
        float x, y, z;
        float width, height, depth;
    }AABox; // Axis aligned box

    typedef enum {
        G_TRIANGLE = 0x10,
        G_RECTANGLE = 0x11,
        G_HEXAGON = 0x12
    }Gshape;

    class Mesh
    {
    private:
        Vertex* Vertices = NULL;
        Index* Indices = NULL;
        AABox BoundingBox;
        Transform transform;
    public:
        int VertexCount = 0;
        int IndexCount = 0;

        void Create(int vertexCount, int indicies);
        void Delete();
        void Fill(Vertex* verticies, Index* indicies);
        Vertex* GetVertices();
        Index* GetIndices();
        void FillColour(float r, float g, float b, float a);
        void Print();

        void SetBoundingBox(AABox box);
        AABox GetBoundingBox();
        void SetTransform(Transform transform);
        void SetTransform();
        Transform GetTransform();
    };

    class MeshGenerator
    {
    public:
        int HelloTriangle(Mesh* mesh);
        int RegularShape(Mesh* mesh, Gshape shape);
        int CalculateBounds(Mesh* mesh);
    };
}