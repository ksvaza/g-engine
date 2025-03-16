#include "../include/meshloader.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <mesh.hpp>
#include <direct.h>

namespace Gengine
{
    int MeshLoader::LoadOBJ(Mesh* mesh, const char* filepath)
    {
        FILE* file = fopen(filepath, "r");
        if (!file)
        {
            printf("Failed to open file: %s\n", filepath);
            return -1;
        }
        printf("Loading OBJ file: %s\n", filepath);

        // First pass to count vertices, indices, faces, materials
        uint16_t vertexCount = 0;
        uint16_t texCoordCount = 0;
        uint16_t normalCount = 0;
        uint16_t faceCount = 0;
        uint16_t materialCount = 0;
        char line[128];
        memset(line, 0, 128);
        while (fgets(line, 128, file))
        {
            if (line[0] == 'v' && line[1] == ' ')
            {
                vertexCount++;
            }
            else if (line[0] == 'f' && line[1] == ' ')
            {
                faceCount++;
            }
            else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
            {
                texCoordCount++;
            }
            else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
            {
                normalCount++;
            }
            else if (line[0] == 'u' && line[1] == 's' && line[2] == 'e' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
            {
                materialCount++;
            }
        }
        fseek(file, 0, SEEK_SET);

        if (texCoordCount > vertexCount)
        {
            printf("Texture coordinate count exceeds vertex count\n");
        }
        else if (normalCount > vertexCount)
        {
            printf("Normal count exceeds vertex count\n");
        }

        mesh->Delete();
        mesh->Create(vertexCount, faceCount);
        mesh->materialNames = (char**)malloc(sizeof(char*) * materialCount);
        mesh->materialCount = materialCount;

        float* texCoords = (float*)malloc(sizeof(float) * texCoordCount * 2);
        memset(texCoords, 0, sizeof(float) * texCoordCount * 2);
        float* normals = (float*)malloc(sizeof(float) * normalCount * 3);
        memset(normals, 0, sizeof(float) * normalCount * 3);

        Vertex* vertices = mesh->GetVertices();
        Index* indices = mesh->GetIndices();

        // Second pass to fill vertices and indices
        uint16_t vertexIndex = 0;
        uint16_t texCoordIndex = 0;
        uint16_t normalIndex = 0;
        uint16_t faceIndex = 0;
        int mtlIndex = -1;
        char mtlPath[128];
        memset(mtlPath, 0, 128);
        while (fgets(line, 128, file))
        {
            //printf("Thus: %s", line);
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
                vertices[vertexIndex].nx = 0.0f;
                vertices[vertexIndex].ny = 0.0f;
                vertices[vertexIndex].nz = 0.0f;
                vertices[vertexIndex].tx = 0.0f;
                vertices[vertexIndex].ty = 0.0f;
                vertices[vertexIndex].tw = 0.0f;
                vertices[vertexIndex].th = 0.0f;
                vertices[vertexIndex].textureIndex = -1;
                vertexIndex++;
            }
            else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
            {
                float u, v;
                int res = sscanf(line, "vt %f %f", &u, &v);
                
                if (res < 2)
                {
                    printf("Failed to read texture coordinate\n");
                }
                else if (texCoordIndex < vertexCount)
                {
                    texCoords[texCoordIndex * 2] = u;
                    texCoords[texCoordIndex * 2 + 1] = v;
                    vertices[texCoordIndex].u = u;
                    vertices[texCoordIndex].v = v;
                }
                texCoordIndex++;
            }
            else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
            {
                float nx, ny, nz;
                int res = sscanf(line, "vn %f %f %f", &nx, &ny, &nz);
                if (res < 3)
                {
                    printf("Failed to read normal\n");
                }
                else if (normalIndex < vertexCount)
                {
                    normals[normalIndex * 3] = nx;
                    normals[normalIndex * 3 + 1] = ny;
                    normals[normalIndex * 3 + 2] = nz;
                    vertices[normalIndex].nx = nx;
                    vertices[normalIndex].ny = ny;
                    vertices[normalIndex].nz = nz;
                }
                normalIndex++;
            }
            else if (line[0] == 'f' && line[1] == ' ')
            {
                uint16_t v1, v2, v3, t1, t2, t3, n1, n2, n3;
                int res = sscanf(line, "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
                if (res < 9)
                {
                    res = sscanf(line, "f %hu//%hu %hu//%hu %hu//%hu", &v1, &n1, &v2, &n2, &v3, &n3);
                    if (res < 6)
                    {
                        res = sscanf(line, "f %hu/%hu %hu/%hu %hu/%hu", &v1, &t1, &v2, &t2, &v3, &t3);
                        if (res < 6)
                        {
                            res = sscanf(line, "f %hu %hu %hu", &v1, &v2, &v3);
                            if (res < 3) { printf("Failed to read face\n"); }
                            else
                            {
                                indices[faceIndex].I[0] = v1 - 1;
                                indices[faceIndex].I[1] = v2 - 1;
                                indices[faceIndex].I[2] = v3 - 1;
                                faceIndex++;
                                vertices[v1 - 1].textureIndex = mtlIndex;
                                vertices[v2 - 1].textureIndex = mtlIndex;
                                vertices[v3 - 1].textureIndex = mtlIndex;
                            }
                        }
                        else
                        {
                            vertices[v1 - 1].u = texCoords[t1 * 2];
                            vertices[v1 - 1].v = texCoords[t1 * 2 + 1];
                            vertices[v2 - 1].u = texCoords[t2 * 2];
                            vertices[v2 - 1].v = texCoords[t2 * 2 + 1];
                            vertices[v3 - 1].u = texCoords[t3 * 2];
                            vertices[v3 - 1].v = texCoords[t3 * 2 + 1];

                            vertices[v1 - 1].nx = 0.0;
                            vertices[v1 - 1].ny = 0.0;
                            vertices[v1 - 1].nz = 0.0;
                            vertices[v2 - 1].nx = 0.0;
                            vertices[v2 - 1].ny = 0.0;
                            vertices[v2 - 1].nz = 0.0;
                            vertices[v3 - 1].nx = 0.0;
                            vertices[v3 - 1].ny = 0.0;
                            vertices[v3 - 1].nz = 0.0;

                            indices[faceIndex].I[0] = v1 - 1;
                            indices[faceIndex].I[1] = v2 - 1;
                            indices[faceIndex].I[2] = v3 - 1;
                            faceIndex++;
                            vertices[v1 - 1].textureIndex = mtlIndex;
                            vertices[v2 - 1].textureIndex = mtlIndex;
                            vertices[v3 - 1].textureIndex = mtlIndex;
                        }
                    }
                    else
                    {
                        vertices[v1 - 1].u = 0.0f;
                        vertices[v1 - 1].v = 0.0f;
                        vertices[v2 - 1].u = 0.0f;
                        vertices[v2 - 1].v = 0.0f;
                        vertices[v3 - 1].u = 0.0f;
                        vertices[v3 - 1].v = 0.0f;

                        vertices[v1 - 1].nx = normals[n1 * 3];
                        vertices[v1 - 1].ny = normals[n1 * 3 + 1];
                        vertices[v1 - 1].nz = normals[n1 * 3 + 2];
                        vertices[v2 - 1].nx = normals[n2 * 3];
                        vertices[v2 - 1].ny = normals[n2 * 3 + 1];
                        vertices[v2 - 1].nz = normals[n2 * 3 + 2];
                        vertices[v3 - 1].nx = normals[n3 * 3];
                        vertices[v3 - 1].ny = normals[n3 * 3 + 1];
                        vertices[v3 - 1].nz = normals[n3 * 3 + 2];

                        indices[faceIndex].I[0] = v1 - 1;
                        indices[faceIndex].I[1] = v2 - 1;
                        indices[faceIndex].I[2] = v3 - 1;
                        faceIndex++;
                        vertices[v1 - 1].textureIndex = mtlIndex;
                        vertices[v2 - 1].textureIndex = mtlIndex;
                        vertices[v3 - 1].textureIndex = mtlIndex;
                    }
                }
                else
                {
                    vertices[v1 - 1].u = texCoords[t1 * 2];
                    vertices[v1 - 1].v = texCoords[t1 * 2 + 1];
                    vertices[v2 - 1].u = texCoords[t2 * 2];
                    vertices[v2 - 1].v = texCoords[t2 * 2 + 1];
                    vertices[v3 - 1].u = texCoords[t3 * 2];
                    vertices[v3 - 1].v = texCoords[t3 * 2 + 1];

                    vertices[v1 - 1].nx = normals[n1 * 3];
                    vertices[v1 - 1].ny = normals[n1 * 3 + 1];
                    vertices[v1 - 1].nz = normals[n1 * 3 + 2];
                    vertices[v2 - 1].nx = normals[n2 * 3];
                    vertices[v2 - 1].ny = normals[n2 * 3 + 1];
                    vertices[v2 - 1].nz = normals[n2 * 3 + 2];
                    vertices[v3 - 1].nx = normals[n3 * 3];
                    vertices[v3 - 1].ny = normals[n3 * 3 + 1];
                    vertices[v3 - 1].nz = normals[n3 * 3 + 2];

                    indices[faceIndex].I[0] = v1 - 1;
                    indices[faceIndex].I[1] = v2 - 1;
                    indices[faceIndex].I[2] = v3 - 1;
                    faceIndex++;
                    vertices[v1 - 1].textureIndex = mtlIndex;
                    vertices[v2 - 1].textureIndex = mtlIndex;
                    vertices[v3 - 1].textureIndex = mtlIndex;
                }
            }
            else if (line[0] == 'm' && line[1] == 't' && line[2] == 'l' && line[3] == 'l' && line[4] == 'i' && line[5] == 'b')
            {
                sscanf(line, "mtllib %128s", mtlPath);
            }
            else if (line[0] == 'u' && line[1] == 's' && line[2] == 'e' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
            {
                char name[128];
                sscanf(line, "usemtl %128s", name);
                mesh->materialNames[++mtlIndex] = (char*)malloc(sizeof(char) * 128);
                strcpy(mesh->materialNames[mtlIndex], name);
            }
        }
        _getcwd(line, 128);
        printf("Current working directory: %s\n", line);
        printf("Vertex count: %d\n", vertexCount);
        fclose(file);
        printf("Texture count: %d\n", texCoordCount);
        
        // Load MTL file
        if (mtlPath[0] != '\0')
        {
            char mtlFullPath[128];
            memset(mtlFullPath, 0, 128);
            strcpy(mtlFullPath, filepath);
            char* lastSlash = strrchr(mtlFullPath, '/');
            if (lastSlash)
            {
                lastSlash[1] = '\0';
                strcat(mtlFullPath, mtlPath);
            }
            LoadMTL(mesh, mtlFullPath);
        }

        return 0;
    }
    int MeshLoader::LoadMTL(Mesh* mesh, const char* filepath)
    {
        FILE* file = fopen(filepath, "r");
        if (!file)
        {
            printf("Failed to open file: %s\n", filepath);
            return -1;
        }
        printf("Loading MTL file: %s\n", filepath);

        // First pass to count materials, texture file associations
        uint16_t materialCount = 0;
        uint16_t textureCount = 0;
        char line[128];
        memset(line, 0, 128);
        printf("Reading MTL file\n");
        while (fgets(line, 128, file))
        {
            printf("test\n");
            if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
            {
                printf("Material name: %s\n", line);
                materialCount++;
            }
            else if (line[0] == 'm' && line[1] == 'a' && line[2] == 'p' && line[3] == '_' && line[4] == 'K' && line[5] == 'd')
            {
                printf("Texture path: %s\n", line);
                textureCount++;
            }
        }
        printf("Material count: %d\n", materialCount);
        printf("Texture count: %d\n", textureCount);
        fseek(file, 0, SEEK_SET);
        
        if (mesh->materialTextureAssociations)
        {
            free(mesh->materialTextureAssociations);
        }
        mesh->materialTextureAssociations = (int*)malloc(sizeof(int) * materialCount);
        for (uint16_t i = 0; i < materialCount; i++)
        {
            mesh->materialTextureAssociations[i] = -1;
        }
        printf("Associations allocated\n");

        // Second pass to fill materials, texture file associations
        uint16_t materialIndex = 0;
        uint8_t unusedMaterial = 0;
        while (fgets(line, 128, file))
        {
            if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
            {
                char name[128];
                sscanf(line, "newmtl %128s", name);
                unusedMaterial = 1;
                for (uint16_t i = 0; i < mesh->materialCount; i++)
                {
                    if (strcmp(mesh->materialNames[i], name) == 0)
                    {
                        materialIndex = i;
                        unusedMaterial = 0;
                        break;
                    }
                }
            }
            else if (line[0] == 'K' && line[1] == 'd' && line[2] == ' ')
            {
                if (!unusedMaterial)
                {
                    float r, g, b;
                    sscanf(line, "Kd %f %f %f", &r, &g, &b);
                    printf("aK ");
                    mesh->FillColourID(r, g, b, -1.0, materialIndex);
                    printf("bK\n");
                }
            }
            else if (line[0] == 'd' && line[1] == ' ')
            {
                if (!unusedMaterial)
                {
                    float a;
                    sscanf(line, "d %f", &a);
                    printf("aD ");
                    mesh->FillColourID(-1.0, -1.0, -1.0, a, materialIndex);
                    printf("bD\n");
                }
            }
            else if (line[0] == 'm' && line[1] == 'a' && line[2] == 'p' && line[3] == '_' && line[4] == 'K' && line[5] == 'd')
            {
                if (!unusedMaterial)
                {
                    char texturePath[128];
                    sscanf(line, "map_Kd %128s", texturePath);
                    
                    char fullTexturePath[128];
                    memset(fullTexturePath, 0, 128);
                    strcpy(fullTexturePath, filepath);
                    char* lastSlash = strrchr(fullTexturePath, '/');
                    if (lastSlash)
                    {
                        lastSlash[1] = '\0';
                        strcat(fullTexturePath, texturePath);
                    }

                    Texture* texture = (Texture*)malloc(sizeof(Texture));
                    if (!texture) { return -1; }
                    *texture = Texture::Empty();
                    texture->LoadData(fullTexturePath);
                    mesh->AddTexture(*texture);
                    mesh->materialTextureAssociations[materialIndex] = mesh->TextureCount - 1;
                }
            }
        }

        fclose(file);

        return 0;
    }
}