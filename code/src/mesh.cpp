#include "../include/mesh.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Gengine
{
    // Mesh functions
    Mesh Mesh::Empty()
    {
        Mesh mesh;
        mesh.Vertices = NULL;
        mesh.Indices = NULL;
        mesh.VertexCount = 0;
        mesh.IndexCount = 0;
        mesh.BoundingBox.x = 0.0f;
        mesh.BoundingBox.y = 0.0f;
        mesh.BoundingBox.z = 0.0f;
        mesh.BoundingBox.width = 0.0f;
        mesh.BoundingBox.height = 0.0f;
        mesh.BoundingBox.depth = 0.0f;
        mesh.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        mesh.transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        mesh.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        mesh.colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        return mesh;
    }
    void Mesh::Create(int vertexCount, int indexCount)
    {
        if (Vertices) { free(Vertices); }
        VertexCount = vertexCount;
        Vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
        
        if (Indices) { free(Indices); }
        IndexCount = indexCount;
        Indices = (Index*)malloc(sizeof(Index) * indexCount);

        BoundingBox = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        transform = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
    }
    void Mesh::Recreate(int vertexCount, int indexCount)
    {
        if (Vertices && Indices)
        {
            VertexCount = vertexCount;
            Vertices = (Vertex*)realloc(Vertices, sizeof(Vertex) * vertexCount);

            IndexCount = indexCount;
            Indices = (Index*)realloc(Indices, sizeof(Index) * indexCount);
        }
        else
        {
            Create(vertexCount, indexCount);
        }
    }
    void Mesh::Delete()
    {
        if (Vertices) { free(Vertices); }
        if (Indices) { free(Indices); }
    }
    void Mesh::Fill(Vertex* vertices, Index* indices)
    {
        if (Vertices)
        {
            for (int i = 0; i < VertexCount; i++)
            {
                Vertices[i] = vertices[i];
            }
        }

        if (Indices)
        {
            for (int i = 0; i < IndexCount; i++)
            {
                Indices[i] = indices[i];
            }
        }
    }
    Vertex* Mesh::GetVertices()
    {
        return Vertices;
    }
    Index* Mesh::GetIndices()
    {
        return Indices;
    }
    void Mesh::FillColour(float r, float g, float b, float a)
    {
        for (int i = 0; i < VertexCount; i++)
        {
            Vertices[i].r = r;
            Vertices[i].g = g;
            Vertices[i].b = b;
            Vertices[i].a = a;
        }
    }
    void Mesh::SetColour(glm::vec4 colour)
    {
        this->colour = colour;
    }
    glm::vec4 Mesh::GetColour()
    {
        return colour;
    }
    void Mesh::Print()
    {
        printf("Transform:\n");
        printf("{\n\tPosition: (%f, %f, %f)\n", transform.position.x, transform.position.y, transform.position.z);
        printf("\tRotation: (%f, %f, %f)\n", transform.rotation.x, transform.rotation.y, transform.rotation.z);
        printf("\tScale: (%f, %f, %f)\n}\n", transform.scale.x, transform.scale.y, transform.scale.z);
        printf("Bounding box:\n");
        printf("{\n\tPosition: (%f, %f, %f)\n", BoundingBox.x, BoundingBox.y, BoundingBox.z);
        printf("\tWidth: %f\n", BoundingBox.width);
        printf("\tHeight: %f\n", BoundingBox.height);
        printf("\tDepth: %f\n}\n", BoundingBox.depth);
        printf("Mesh vertices: %d\n", VertexCount);
        for (int i = 0; i < VertexCount; i++)
        {
            printf("{\n\tPosition: (%f, %f, %f)\n", Vertices[i].x, Vertices[i].y, Vertices[i].z);
            printf("\tColor: (%f, %f, %f, %f)\n}", Vertices[i].r, Vertices[i].g, Vertices[i].b, Vertices[i].a);
            printf("\n");
        }
        printf("Mesh indicies: %d\n", IndexCount);
        for (int i = 0; i < IndexCount; i++)
        {
            printf("{%d, %d, %d}\n", Indices[i].I[0], Indices[i].I[1], Indices[i].I[2]);
        }
    }
    char Mesh::Equals(Mesh mesh)
    {
        if (VertexCount != mesh.VertexCount || IndexCount != mesh.IndexCount)
        {
            return 0;
        }

        for (int i = 0; i < VertexCount; i++)
        {
            if (Vertices[i].x != mesh.Vertices[i].x || Vertices[i].y != mesh.Vertices[i].y || Vertices[i].z != mesh.Vertices[i].z)
            {
                return 0;
            }
        }

        for (int i = 0; i < IndexCount; i++)
        {
            if (Indices[i].I[0] != mesh.Indices[i].I[0] || Indices[i].I[1] != mesh.Indices[i].I[1] || Indices[i].I[2] != mesh.Indices[i].I[2])
            {
                return 0;
            }
        }

        return 1;
    }
    void Mesh::SetBoundingBox(AABox box)
    {
        BoundingBox = box;
    }
    AABox Mesh::GetBoundingBox()
    {
        return BoundingBox;
    }
    void Mesh::SetTransform(Transform transform)
    {
        this->transform = transform;
    }
    void Mesh::SetTransform()
    {
        this->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        this->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    Transform Mesh::GetTransform()
    {
        return transform;
    }

    // MeshGenerator functions
    int MeshGenerator::HelloTriangle(Mesh* mesh)
    {
        mesh->Create(3, 1);
        Vertex Vertices[3];
        Index Indices[1];

        Vertices[0].x = -0.5f;
        Vertices[0].y = -0.5f;
        Vertices[0].z = 0.0f;

        Vertices[1].x = 0.5f;
        Vertices[1].y = -0.5f;
        Vertices[1].z = 0.0f;

        Vertices[2].x = 0.0f;
        Vertices[2].y = 0.5f;
        Vertices[2].z = 0.0f;

        Indices[0].I[0] = 0;
        Indices[0].I[1] = 2;
        Indices[0].I[2] = 1;

        for (int i = 0; i < mesh->VertexCount; i++)
        {
            Vertices[i].r = 1.0f;
            Vertices[i].g = 1.0f;
            Vertices[i].b = 1.0f;
            Vertices[i].a = 1.0f;
        }

        mesh->Fill(Vertices, Indices);
        mesh->FillColour(1.0f, 0.5f, 0.2f, 1.0f);

        return 0;
    }
    int MeshGenerator::RegularShape(Mesh* mesh, Gshape shape)
    {
        switch (shape) // creates correct amount of vertices and indicies for the shape and also sets the correct positions to create a regular unit shape of width, heigth 1
        {
        case G_TRIANGLE: // regular triangle side length 1
        {
            mesh->Create(3, 1);
            Vertex Vertices[3];
            Index Indices[1];

            float sqrtFP = glm::sqrt(0.25f / 5.0f);

            // Verticies
            Vertices[0].x = -2.0f * sqrtFP;
            Vertices[0].y = -1.0f * sqrtFP;
            Vertices[0].z = 0.0f;

            Vertices[1].x = 2.0f * sqrtFP;
            Vertices[1].y = -1.0f * sqrtFP;
            Vertices[1].z = 0.0f;

            Vertices[2].x = 0.0f;
            Vertices[2].y = 0.5f;
            Vertices[2].z = 0.0f;

            // Indicies
            Indices[0].I[0] = 0;
            Indices[0].I[1] = 1;
            Indices[0].I[2] = 2;

            mesh->Fill(Vertices, Indices);
            break;
        }
        case G_RECTANGLE: // regular square
        {
            mesh->Create(4, 2);
            Vertex Vertices[4];
            Index Indices[2];

            // Verticies
            Vertices[0].x = -0.5f;
            Vertices[0].y = -0.5f;
            Vertices[0].z = 0.0f;
            
            Vertices[1].x = 0.5f;
            Vertices[1].y = -0.5f;
            Vertices[1].z = 0.0f;

            Vertices[2].x = 0.5f;
            Vertices[2].y = 0.5f;
            Vertices[2].z = 0.0f;

            Vertices[3].x = -0.5f;
            Vertices[3].y = 0.5f;
            Vertices[3].z = 0.0f;

            // Indicies
            Indices[0].I[0] = 0;
            Indices[0].I[1] = 2;
            Indices[0].I[2] = 1;

            Indices[1].I[0] = 0;
            Indices[1].I[1] = 3;
            Indices[1].I[2] = 2;

            mesh->Fill(Vertices, Indices);
            break;
        }
        case G_HEXAGON:
        {
            mesh->Create(6, 4);
            Vertex Vertices[6];
            Index Indices[4];

            float sqrtFP = glm::sqrt(0.25f / 5.0f);

            // Verticies
            Vertices[0].x = -2.0f * sqrtFP;
            Vertices[0].y = -1.0f * sqrtFP;
            Vertices[0].z = 0.0f;

            Vertices[1].x = 0;
            Vertices[1].y = -0.5f;
            Vertices[1].z = 0.0f;

            Vertices[2].x = 2.0f * sqrtFP;
            Vertices[2].y = -1.0f * sqrtFP;
            Vertices[2].z = 0.0f;

            Vertices[3].x = 2.0f * sqrtFP;
            Vertices[3].y = 1.0f * sqrtFP;
            Vertices[3].z = 0.0f;

            Vertices[4].x = 0.0f;
            Vertices[4].y = 0.5f;
            Vertices[4].z = 0.0f;

            Vertices[5].x = -2.0f * sqrtFP;
            Vertices[5].y = 1.0f * sqrtFP;
            Vertices[5].z = 0.0f;

            // Indicies
            Indices[0].I[0] = 0;
            Indices[0].I[1] = 2;
            Indices[0].I[2] = 4;
            
            Indices[1].I[0] = 0;
            Indices[1].I[1] = 1;
            Indices[1].I[2] = 2;
            
            Indices[2].I[0] = 2;
            Indices[2].I[1] = 3;
            Indices[2].I[2] = 4;
            
            Indices[3].I[0] = 4;
            Indices[3].I[1] = 5;
            Indices[3].I[2] = 0;

            mesh->Fill(Vertices, Indices);
            break;
        }
        default:
            break;
        }

        for (int i = 0; i < mesh->VertexCount; i++)
        {
            mesh->GetVertices()[i].r = 1.0f;
            mesh->GetVertices()[i].g = 1.0f;
            mesh->GetVertices()[i].b = 1.0f;
            mesh->GetVertices()[i].a = 1.0f;
        }

        return 0;
    }
    int MeshGenerator::CalculateBounds(Mesh* mesh)
    {
        Vertex* verts = mesh->GetVertices();
        AABox box = mesh->GetBoundingBox();
        glm::mat4 m = TransformToMatrix(mesh->GetTransform());

        float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
        float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

        for (int i = 0; i < mesh->VertexCount; i++)
        {
            glm::vec4 p = m * glm::vec4(verts[i].x, verts[i].y, verts[i].z, 1.0f);
            if (p.x < minX) { minX = p.x; } if (p.x > maxX) { maxX = p.x; }
            if (p.y < minY) { minY = p.y; } if (p.y > maxY) { maxY = p.y; }
            if (p.z < minZ) { minZ = p.z; } if (p.z > maxZ) { maxZ = p.z; }
        }

        box.x = minX; box.y = minY; box.z = minZ;
        box.width = maxX - minX; box.height = maxY - minY; box.depth = maxZ - minZ;
        mesh->SetBoundingBox(box);
        return 0;
    }
    int MeshGenerator::StichMesh(Mesh* base, Mesh* add)
    {
        Vertex* addVertices = add->GetVertices();
        Index* addIndices = add->GetIndices();

        int baseVertexCount = base->VertexCount;
        int baseIndexCount = base->IndexCount;
        base->Recreate(base->VertexCount + add->VertexCount, base->IndexCount + add->IndexCount);
        Vertex* newVertices = base->GetVertices();
        Index* newIndices = base->GetIndices();

        for (int i = 0; i < add->VertexCount; i++)
        {
            newVertices[baseVertexCount + i] = addVertices[i];
        }

        for (int i = 0; i < add->IndexCount; i++)
        {
            newIndices[baseIndexCount + i] = addIndices[i];
            for (int j = 0; j < 3; j++)
            {
                newIndices[baseIndexCount + i].I[j] += baseVertexCount;
            }
        }

        return 0;
    }
    int MeshGenerator::AddMesh(Mesh* base, Mesh* add)
    {
        Vertex* addVertices = add->GetVertices();
        Index* addIndices = add->GetIndices();

        int baseVertexCount = base->VertexCount;
        int baseIndexCount = base->IndexCount;
        base->Recreate(base->VertexCount + add->VertexCount, base->IndexCount + add->IndexCount);
        Vertex* newVertices = base->GetVertices();
        Index* newIndices = base->GetIndices();

        for (int i = 0; i < add->VertexCount; i++)
        {
            // Tranform
            glm::vec4 vertex = glm::vec4(addVertices[i].x, addVertices[i].y, addVertices[i].z, 1.0f);
            glm::mat4 transform = Gengine::TransformToMatrix(add->GetTransform());
            glm::mat4 invBaseTransform = glm::inverse(Gengine::TransformToMatrix(base->GetTransform()));
            vertex = invBaseTransform * transform * vertex;
            glm::vec4 colour = add->GetColour();

            // Add
            Vertex newVertex = { vertex.x, vertex.y, vertex.z, addVertices[i].r * colour.r, addVertices[i].g * colour.g, addVertices[i].b * colour.b, addVertices[i].a * colour.a };
            newVertices[baseVertexCount + i] = newVertex;
        }

        for (int i = 0; i < add->IndexCount; i++)
        {
            newIndices[baseIndexCount + i] = addIndices[i];
            for (int j = 0; j < 3; j++)
            {
                newIndices[baseIndexCount + i].I[j] += baseVertexCount;
            }
        }

        return 0;
    }
    int MeshGenerator::TransformMesh(Mesh* mesh, Transform transform)
    {
        Vertex* verts = mesh->GetVertices();
        glm::mat4 m = TransformToMatrix(transform);

        for (int i = 0; i < mesh->VertexCount; i++)
        {
            glm::vec4 p = m * glm::vec4(verts[i].x, verts[i].y, verts[i].z, 1.0f);
            verts[i].x = p.x; verts[i].y = p.y; verts[i].z = p.z;
        }

        return 0;
    }
}        
