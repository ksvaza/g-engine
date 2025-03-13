#include "../include/mesh.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <textureatlas.hpp>
#include <string.h>

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
        mesh.TextureCount = 0;
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
        mesh.textures = NULL;
        mesh.atlas = NULL;
        mesh.atlasBounds = NULL;
        mesh.textureNames = NULL;
        mesh.textureNameAssociations = NULL;
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

        TextureCount = 0;

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
        if (textures) { free(textures); }
        if (atlas) { free(atlas); }
        if (textureNames && textureNameCount > 0)
        {
            for (int i = 0; i < textureNameCount; i++)
            {
                free(textureNames[i]);
            }
            free(textureNames);
        }
        *this = Empty();
    }
    void Mesh::Clear()
    {
        if (Vertices) { free(Vertices); }
        if (Indices) { free(Indices); }
        if (textures) { free(textures); }
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
    void Mesh::FillColourID(float r, float g, float b, float a, int textureID)
    {
        for (int i = 0; i < VertexCount; i++)
        {
            if (Vertices[i].textureIndex == (float)textureID)
            {
                if (r >= 0.0) { Vertices[i].r = r; }
                if (g >= 0.0) { Vertices[i].g = g; }
                if (b >= 0.0) { Vertices[i].b = b; }
                if (a >= 0.0) { Vertices[i].a = a; }
            }
        }
    }
    void Mesh::FillTextureID(int textureID) // deprecated
    {
        for (int i = 0; i < VertexCount; i++)
        {
            Vertices[i].textureIndex = (float)textureID;
        }
    }
    void Mesh::FillTextureTransform(float tx, float ty, float tw, float th)
    {
        for (int i = 0; i < VertexCount; i++)
        {
            Vertices[i].tx = tx;
            Vertices[i].ty = ty;
            Vertices[i].tw = tw;
            Vertices[i].th = th;
        }
    }
    void Mesh::FillTextureTransformID(float tx, float ty, float tw, float th, int textureID)
    {
        for (int i = 0; i < VertexCount; i++)
        {
            if (Vertices[i].textureIndex == (float)textureID)
            {
                Vertices[i].tx = tx;
                Vertices[i].ty = ty;
                Vertices[i].tw = tw;
                Vertices[i].th = th;
            }
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
            printf("\tColor: (%f, %f, %f, %f)\n", Vertices[i].r, Vertices[i].g, Vertices[i].b, Vertices[i].a);
            printf("\tTexture coordinates: (%f, %f)\n", Vertices[i].u, Vertices[i].v);
            printf("\tTexture transformation: (%f, %f, %f, %f)\n", Vertices[i].tx, Vertices[i].ty, Vertices[i].tw, Vertices[i].th);
            printf("\tTexture index: %d\n}", (int)Vertices[i].textureIndex);
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
    void Mesh::AddTexture(Texture texture)
    {
        if (TextureCount == 0 && textures)
        {
            printf("Texture count is 0 but textures is not NULL\n");
            textures = NULL;
        }
        TextureCount++;
        textures = (Texture*)realloc(textures, sizeof(Texture) * TextureCount);
        textures[TextureCount - 1] = texture;
    }
    Texture Mesh::GetTexture(int index)
    {
        if (!textures)
        {
            printf("Textures is NULL\n");
            return textures[0];
        }
        return textures[index];
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

    char PointInBounds(glm::vec2 point, AABox bounds)
    {
        if (point.x >= bounds.x && point.x <= bounds.x + bounds.width &&
            point.y >= bounds.y && point.y <= bounds.y + bounds.height)
        {
            return 1;
        }
        return 0;
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
            Vertices[i].u = 0.0f;
            Vertices[i].v = 0.0f;
            Vertices[i].tx = 0.0f;
            Vertices[i].ty = 0.0f;
            Vertices[i].tw = 0.0f;
            Vertices[i].th = 0.0f;
            Vertices[i].textureIndex = -1.0;
        }

        mesh->Fill(Vertices, Indices);
        mesh->SetColour(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));

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
            mesh->GetVertices()[i].tx = 0.0f;
            mesh->GetVertices()[i].ty = 0.0f;
            mesh->GetVertices()[i].tw = 0.0f;
            mesh->GetVertices()[i].th = 0.0f;
            mesh->GetVertices()[i].textureIndex = -1.0;
        }
        MeshGenerator::CalculateTextureCoordinates(mesh);

        return 0;
    }
    int MeshGenerator::Rectangle(Mesh* mesh, glm::vec2 size)
    {
        mesh->Create(4, 2);
        Vertex* Vertices = mesh->GetVertices();
        Index* Indices = mesh->GetIndices();

        // Verticies
        Vertices[0].x = -size.x / 2.0f;
        Vertices[0].y = -size.y / 2.0f;
        Vertices[0].z = 0.0f;
        Vertices[0].u = 0.0f;
        Vertices[0].v = 0.0f;

        Vertices[1].x = size.x / 2.0f;
        Vertices[1].y = -size.y / 2.0f;
        Vertices[1].z = 0.0f;
        Vertices[1].u = 1.0f;
        Vertices[1].v = 0.0f;

        Vertices[2].x = size.x / 2.0f;
        Vertices[2].y = size.y / 2.0f;
        Vertices[2].z = 0.0f;
        Vertices[2].u = 1.0f;
        Vertices[2].v = 1.0f;

        Vertices[3].x = -size.x / 2.0f;
        Vertices[3].y = size.y / 2.0f;
        Vertices[3].z = 0.0f;
        Vertices[3].u = 0.0f;
        Vertices[3].v = 1.0f;

        // Indicies
        Indices[0].I[0] = 0;
        Indices[0].I[1] = 2;
        Indices[0].I[2] = 1;

        Indices[1].I[0] = 0;
        Indices[1].I[1] = 3;
        Indices[1].I[2] = 2;

        for (int i = 0; i < mesh->VertexCount; i++)
        {
            Vertices[i].r = 1.0f;
            Vertices[i].g = 1.0f;
            Vertices[i].b = 1.0f;
            Vertices[i].a = 1.0f;
            Vertices[i].tx = 0.0f;
            Vertices[i].ty = 0.0f;
            Vertices[i].tw = 0.0f;
            Vertices[i].th = 0.0f;
            Vertices[i].textureIndex = -1.0;
        }

        return 0;
    }
    int MeshGenerator::Cube(Mesh* mesh, glm::vec3 size)
    {
        mesh->Create(24, 12);

        float unx = size.x / 2.0f;
        float uny = size.y / 2.0f;
        float unz = size.z / 2.0f;

        // Verticies
        float vertices[] = {
            -unx, -uny, -unz,  1.0, 1.0, 1.0, 1.0,  0.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx, -uny, -unz,  1.0, 1.0, 1.0, 1.0,  1.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx,  uny, -unz,  1.0, 1.0, 1.0, 1.0,  1.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
            -unx,  uny, -unz,  1.0, 1.0, 1.0, 1.0,  0.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,

            -unx, -uny,  unz,  1.0, 1.0, 1.0, 1.0,  0.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx, -uny,  unz,  1.0, 1.0, 1.0, 1.0,  1.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx,  uny,  unz,  1.0, 1.0, 1.0, 1.0,  1.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
            -unx,  uny,  unz,  1.0, 1.0, 1.0, 1.0,  0.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,

            -unx,  uny,  unz,  1.0, 1.0, 1.0, 1.0,  1.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
            -unx,  uny, -unz,  1.0, 1.0, 1.0, 1.0,  1.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
            -unx, -uny, -unz,  1.0, 1.0, 1.0, 1.0,  0.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
            -unx, -uny,  unz,  1.0, 1.0, 1.0, 1.0,  0.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,

             unx,  uny,  unz,  1.0, 1.0, 1.0, 1.0,  1.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx,  uny, -unz,  1.0, 1.0, 1.0, 1.0,  1.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx, -uny, -unz,  1.0, 1.0, 1.0, 1.0,  0.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx, -uny,  unz,  1.0, 1.0, 1.0, 1.0,  0.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,

            -unx, -uny, -unz,  1.0, 1.0, 1.0, 1.0,  0.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx, -uny, -unz,  1.0, 1.0, 1.0, 1.0,  1.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx, -uny,  unz,  1.0, 1.0, 1.0, 1.0,  1.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
            -unx, -uny,  unz,  1.0, 1.0, 1.0, 1.0,  0.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,

            -unx,  uny, -unz,  1.0, 1.0, 1.0, 1.0,  0.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx,  uny, -unz,  1.0, 1.0, 1.0, 1.0,  1.0, 1.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
             unx,  uny,  unz,  1.0, 1.0, 1.0, 1.0,  1.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0,
            -unx,  uny,  unz,  1.0, 1.0, 1.0, 1.0,  0.0, 0.0,  0.0, 0.0, 0.0, 0.0,  -1.0
        };
        // Indicies
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            8, 9, 10,
            10, 11, 8,
            12, 13, 14,
            14, 15, 12,
            16, 17, 18,
            18, 19, 16,
            20, 21, 22,
            22, 23, 20
        };

        mesh->Fill((Vertex*)vertices, (Index*)indices);
        
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
    int MeshGenerator::CalculateTextureCoordinates(Mesh* mesh)
    {
        Vertex* verts = mesh->GetVertices();
        
        float minX = FLT_MAX, minY = FLT_MAX;
        float maxX = -FLT_MAX, maxY = -FLT_MAX;

        // Calculate bounds
        for (int i = 0; i < mesh->VertexCount; i++)
        {
            if (verts[i].x < minX) { minX = verts[i].x; } if (verts[i].x > maxX) { maxX = verts[i].x; }
            if (verts[i].y < minY) { minY = verts[i].y; } if (verts[i].y > maxY) { maxY = verts[i].y; }
        }

        // Apply texture coordinates
        for (int i = 0; i < mesh->VertexCount; i++)
        {
            verts[i].u = (verts[i].x - minX) / (maxX - minX);
            verts[i].v = (maxY - minY) - (verts[i].y - minY) / (maxY - minY);
        }

        return 0;
    }
    int MeshGenerator::CopyMesh(Mesh* destination, Mesh* source)
    {
        destination->Create(source->VertexCount, source->IndexCount);
        Vertex* destVerts = destination->GetVertices();
        Vertex* sourceVerts = source->GetVertices();
        Index* destIndices = destination->GetIndices();
        Index* sourceIndices = source->GetIndices();

        for (int i = 0; i < source->VertexCount; i++)
        {
            destVerts[i] = sourceVerts[i];
        }

        for (int i = 0; i < source->IndexCount; i++)
        {
            destIndices[i] = sourceIndices[i];
        }

        destination->SetColour(source->GetColour());
        destination->SetBoundingBox(source->GetBoundingBox());
        destination->SetTransform(source->GetTransform());
        for (int i = 0; i < source->TextureCount; i++)
        {
            Texture t = source->GetTexture(i);
            destination->AddTexture(t);
            //destination->AddTexture(source->GetTexture(i));
        }
        destination->atlas = (TextureAtlas*)malloc(sizeof(TextureAtlas));
        if (!source->atlas || !destination->atlas) { return -1; }
        *((TextureAtlas*)destination->atlas) = *((TextureAtlas*)source->atlas);
        return 0;
    }
    int MeshGenerator::StichMesh(Mesh* base, Mesh* add)
    {
        Vertex* addVertices = add->GetVertices();
        Index* addIndices = add->GetIndices();
        Texture* addTextures = add->textures;

        int baseVertexCount = base->VertexCount;
        int baseIndexCount = base->IndexCount;
        int baseTextureCount = base->TextureCount;
        base->Recreate(base->VertexCount + add->VertexCount, base->IndexCount + add->IndexCount);
        Vertex* newVertices = base->GetVertices();
        Index* newIndices = base->GetIndices();

        for (int i = 0; i < add->VertexCount; i++)
        {
            newVertices[baseVertexCount + i] = addVertices[i];
            if (newVertices[baseVertexCount + i].textureIndex >= 0.0 && newVertices[baseVertexCount + i].textureIndex < 16.0)
            {
                newVertices[baseVertexCount + i].textureIndex += (float)baseTextureCount;
            }
        }

        for (int i = 0; i < add->IndexCount; i++)
        {
            newIndices[baseIndexCount + i] = addIndices[i];
            for (int j = 0; j < 3; j++)
            {
                newIndices[baseIndexCount + i].I[j] += baseVertexCount;
            }
        }

        for (int i = 0; i < add->TextureCount; i++)
        {
            base->AddTexture(addTextures[i]);
        }

        return 0;
    }
    int MeshGenerator::AddMesh(Mesh* base, Mesh* add)
    {
        Vertex* addVertices = add->GetVertices();
        Index* addIndices = add->GetIndices();
        Texture* addTextures = add->textures;

        int baseVertexCount = base->VertexCount;
        int baseIndexCount = base->IndexCount;
        int baseTextureCount = base->TextureCount;
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
            glm::vec4 baseColour = base->GetColour();

            // Add
            Vertex newVertex = {
                vertex.x, vertex.y, vertex.z, 
                addVertices[i].r * colour.r / baseColour.r, 
                addVertices[i].g * colour.g / baseColour.g, 
                addVertices[i].b * colour.b / baseColour.b, 
                addVertices[i].a * colour.a / baseColour.a,
                addVertices[i].u, addVertices[i].v, 
                addVertices[i].tx, addVertices[i].ty, addVertices[i].tw, addVertices[i].th,
                addVertices[i].textureIndex
            };
            newVertices[baseVertexCount + i] = newVertex;

            // Texture index correction
            if (newVertices[baseVertexCount + i].textureIndex >= 0.0 && newVertices[baseVertexCount + i].textureIndex < 16.0)
            {
                newVertices[baseVertexCount + i].textureIndex += (float)baseTextureCount;
            }
        }

        for (int i = 0; i < add->IndexCount; i++)
        {
            newIndices[baseIndexCount + i] = addIndices[i];
            for (int j = 0; j < 3; j++)
            {
                newIndices[baseIndexCount + i].I[j] += baseVertexCount;
            }
        }

        for (int i = 0; i < add->TextureCount; i++)
        {
            base->AddTexture(addTextures[i]);
        }
        //printf("AddMesh texture count: %d\n", baseTextureCount);

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
    int MeshGenerator::LoadOBJ(Mesh* mesh, const char* path)
    {
        FILE* file = fopen(path, "r");
        if (!file)
        {
            printf("Failed to open file: %s\n", path);
            return -1;
        }

        // First pass to count vertices and indices
        int vertexCount = 0;
        int indexCount = 0;
        int textureCount = 0;
        char line[128];
        while (fgets(line, 128, file))
        {
            if (line[0] == 'v' && line[1] == ' ')
            {
                vertexCount++;
            }
            else if (line[0] == 'f' && line[1] == ' ')
            {
                indexCount++;
            }
            else if (line[0] == 'u' && line[1] == 's' && line[2] == 'e' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
            {
                textureCount++;
            }
        }
        fseek(file, 0, SEEK_SET);

        mesh->Delete();
        mesh->Create(vertexCount, indexCount);
        mesh->textureNames = (char**)malloc(sizeof(char*) * textureCount);
        mesh->textureNameCount = textureCount;
        Vertex* vertices = mesh->GetVertices();
        Index* indices = mesh->GetIndices();

        // Second pass to fill vertices and indices
        int vertexIndex = 0;
        int uvIndex = 0;
        int indexIndex = 0;
        int mtlIndex = -1;
        char mtlPath[128];
        memset(mtlPath, 0, 128);
        while (fgets(line, 128, file))
        {
            if (line[0] == 'v' && line[1] == ' ')
            {
                float x, y, z;
                int res = sscanf(line, "v %f %f %f", &x, &y, &z);
                if (res < 3) { printf("Failed to read vertex\n"); }
                vertices[vertexIndex].x = x;
                vertices[vertexIndex].y = y;
                vertices[vertexIndex].z = z;
                vertices[vertexIndex].r = 1.0f;
                vertices[vertexIndex].g = 1.0f;
                vertices[vertexIndex].b = 1.0f;
                vertices[vertexIndex].a = 1.0f;
                vertices[vertexIndex].u = 0.0f;
                vertices[vertexIndex].v = 0.0f;
                vertices[vertexIndex].tx = 0.0f;
                vertices[vertexIndex].ty = 0.0f;
                vertices[vertexIndex].tw = 0.0f;
                vertices[vertexIndex].th = 0.0f;
                vertices[vertexIndex].textureIndex = -1.0;
                vertexIndex++;
            }
            else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
            {
                float u, v;
                int res = sscanf(line, "vt %f %f", &u, &v);
                vertices[uvIndex].u = u;
                vertices[uvIndex].v = 0.0;
                if (res > 1) { vertices[uvIndex].v = v; }
                else if (res < 1) { printf("Failed to read texture coordinates\n"); }
                uvIndex++;
            }
            else if (line[0] == 'f' && line[1] == ' ')
            {
                int i0, i1, i2;
                int res = sscanf(line, "f %d %d %d", &i0, &i1, &i2);
                if (res < 3) { printf("Failed to read face\n"); }
                indices[indexIndex].I[0] = i0 - 1;
                indices[indexIndex].I[1] = i1 - 1;
                indices[indexIndex].I[2] = i2 - 1;
                indexIndex++;
                vertices[i0 - 1].textureIndex = mtlIndex;
                vertices[i1 - 1].textureIndex = mtlIndex;
                vertices[i2 - 1].textureIndex = mtlIndex;
            }
            else if (line[0] == 'm' && line[1] == 't' && line[2] == 'l' && line[3] == 'l' && line[4] == 'i' && line[5] == 'b')
            {
                sscanf(line, "mtllib %s", mtlPath);
            }
            else if (line[0] == 'u' && line[1] == 's' && line[2] == 'e' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
            {
                mtlIndex++;
                mesh->textureNames[mtlIndex] = (char*)malloc(sizeof(char) * 128);
                sscanf(line, "usemtl %128s", mesh->textureNames[mtlIndex]);
                //printf("Texture name: %s\n", mesh->textureNames[mtlIndex]);
            }
        }
        
        // Load MTL file
        if (mtlPath[0] != '\0')
        {
            char mtlFullPath[128];
            memset(mtlFullPath, 0, 128);
            strcpy(mtlFullPath, path);
            char* lastSlash = strrchr(mtlFullPath, '/');
            if (lastSlash)
            {
                lastSlash[1] = '\0';
                strcat(mtlFullPath, mtlPath);
            }
            LoadMTL(mesh, mtlFullPath);
        }

        fclose(file);

        return 0;
    }
    int MeshGenerator::LoadMTL(Mesh* mesh, const char* path)
    {
        //printf("Loading MTL file: %s\n", path);
        FILE* file = fopen(path, "r");
        if (!file)
        {
            printf("Failed to open file: %s\n", path);
            return -1;
        }

        // First pass to count color texture file associations
        int textureNameCount = 0;
        int textureCount = 0;
        char line[128];
        while (fgets(line, 128, file))
        {
            if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
            {
                textureNameCount++;
            }
            else if (line[0] == 'm' && line[1] == 'a' && line[2] == 'p' && line[3] == '_')
            {
                textureCount++;
            }
        }
        fseek(file, 0, SEEK_SET);
        mesh->textureNameAssociations = (int*)malloc(sizeof(int) * textureNameCount);
        for (int i = 0; i < textureNameCount; i++)
        {
            mesh->textureNameAssociations[i] = -1;
        }
        // Second pass to load each texture file and associate it with a name
        int currentName = 0;
        while (fgets(line, 128, file))
        {
            if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
            {
                char name[128];
                sscanf(line, "newmtl %128s", name);
                for (int i = 0; i < mesh->textureNameCount; i++)
                {
                    if (strcmp(mesh->textureNames[i], name) == 0)
                    {
                        currentName = i;
                        break;
                    }
                }
            }
            else if (line[0] == 'K' && line[1] == 'd')
            {
                float r, g, b;
                sscanf(line, "Kd %f %f %f", &r, &g, &b);
                mesh->FillColourID(r, g, b, -1.0, currentName);
            }
            else if (line[0] == 'd' && line[1] == ' ')
            {
                float a;
                sscanf(line, "d %f", &a);
                mesh->FillColourID(-1.0, -1.0, -1.0, a, currentName);
            }
            else if (line[0] == 'm' && line[1] == 'a' && line[2] == 'p' && line[3] == '_')
            {
                char texturePath[128];
                sscanf(line, "map_Kd %128s", texturePath);
                
                Texture* texture = (Texture*)malloc(sizeof(Texture));
                if (!texture) { return -1; }
                texture->LoadData(texturePath);
                mesh->AddTexture(*texture);
                mesh->textureNameAssociations[currentName] = mesh->TextureCount - 1;
            }
        }

        fclose(file);

        return 0;
    }
}        
