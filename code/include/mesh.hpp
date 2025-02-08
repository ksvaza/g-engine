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
        G_TRIANGLE = 0x13,
        G_RECTANGLE = 0x14,
        G_PENTAGON = 0x15,
        G_HEXAGON = 0x16,
        G_OCTAGON = 0x18,
        G_CIRCLE = 0x20,
        G_STAR = 0x21
    }Gshape;

    char PointInBounds(glm::vec2 point, AABox bounds);

    class Mesh
    {
    private:
        Vertex* Vertices = NULL;
        Index* Indices = NULL;
        AABox BoundingBox;
    public:
        int VertexCount = 0;
        int IndexCount = 0;

        Transform transform;
        glm::vec4 colour;

        static Mesh Empty();

        void Create(int vertexCount, int indicies);
        void Recreate(int vertexCount, int indicies);
        void Delete();
        void Fill(Vertex* verticies, Index* indicies);
        Vertex* GetVertices();
        Index* GetIndices();
        void FillColour(float r, float g, float b, float a);
        void SetColour(glm::vec4 colour);
        glm::vec4 GetColour();
        void Print();
        char Equals(Mesh mesh);

        void SetBoundingBox(AABox box);
        AABox GetBoundingBox();
        void SetTransform(Transform transform);
        void SetTransform();
        Transform GetTransform();
    };

    class MeshGenerator
    {
    public:
        static int HelloTriangle(Mesh* mesh);
        static int RegularShape(Mesh* mesh, Gshape shape);
        static int CalculateBounds(Mesh* mesh);
        static int CopyMesh(Mesh* destinaton, Mesh* source);
        static int StichMesh(Mesh* base, Mesh* add);
        static int AddMesh(Mesh* base, Mesh* add);
        static int TransformMesh(Mesh* mesh, Transform transform);
    };
}