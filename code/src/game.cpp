#include "../include/game.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <mesh.hpp>
#include <camera.hpp>

namespace Gengine
{
    // GameObject Functions
    // ----------------------------------------------------------------

    void GameObject::Create()
    {
        mesh = Mesh::Empty();
        transform = NewTransform();
        visible = 1;
    }
    void GameObject::Delete()
    {
        mesh.Delete();
    }
    void GameObject::Update()
    {
        //printf("GameObject updated\n");
    }
    int GameObject::BakeTexture() // Deprecated
    {
        // Atlas Stiching
        free(mesh.atlas);
        
        mesh.atlas = (TextureAtlas*)malloc(sizeof(TextureAtlas));
        if (!mesh.atlas) { return -1; }
        *((TextureAtlas*)mesh.atlas) = TextureAtlas::Empty();
        if (mesh.TextureCount == 0) { return 0; }

        ((TextureAtlas*)mesh.atlas)->AddTexture(&mesh.textures[0]);
        ((TextureAtlas*)mesh.atlas)->elementReference = (void**)realloc(((TextureAtlas*)mesh.atlas)->elementReference, sizeof(GameObject*));
        ((GameObject**)((TextureAtlas*)mesh.atlas)->elementReference)[0] = this;
        
        char* name = (char*)malloc(256);
        sprintf(name, "textures/temp/GameObject_%p.bmp", (void*)this);
        ((TextureAtlas*)mesh.atlas)->Bake(name);

        // Atlas Mapping
        TextureAtlas* atlas = (TextureAtlas*)mesh.atlas;
        AABox bounds = ((AABox*)atlas->textureBounds)[0];
        mesh.FillTextureTransform(bounds.x, bounds.y, bounds.width, bounds.height);
        mesh.atlasBounds = (AABox*)malloc(sizeof(AABox));
        *(mesh.atlasBounds) = bounds;

        return 0;
    }
    int GameObject::BakeTextures()
    {
        // Atlas Stiching
        free(mesh.atlas);
        
        mesh.atlas = (TextureAtlas*)malloc(sizeof(TextureAtlas));
        if (!mesh.atlas) { return -1; }
        *((TextureAtlas*)mesh.atlas) = TextureAtlas::Empty();
        if (mesh.TextureCount == 0) { return 0; }

        for (int i = 0; i < mesh.TextureCount; i++)
        {
            ((TextureAtlas*)mesh.atlas)->AddTexture(&mesh.textures[i]);
        }
        
        char* name = (char*)malloc(256);
        sprintf(name, "textures/temp/GameObject_%p.bmp", (void*)this);
        ((TextureAtlas*)mesh.atlas)->Bake(name);

        // Atlas Mapping
        TextureAtlas* atlas = (TextureAtlas*)mesh.atlas;
        mesh.FillTextureTransform(0.0, 0.0, 0.0, 0.0);
        for (int i = 0; i < mesh.materialCount; i++)
        {
            int current = mesh.materialTextureAssociations[i];
            if (current == -1) { continue; }
            AABox bounds = ((AABox*)atlas->textureBounds)[current];
            mesh.FillTextureTransformID(bounds.x, bounds.y, bounds.width, bounds.height, i);
        }

        return 0;
    }

    // Game Functions
    // ----------------------------------------------------------------

    void GameWorld::AddGameObject(GameObject* gameObject)
    {
        gameObjects = (GameObject**)realloc(gameObjects, sizeof(GameObject*) * (gameObjectCount + 1));
        gameObjects[gameObjectCount] = gameObject;
        gameObjectCount++;
    }
    void GameWorld::RemoveGameObject(GameObject* gameObject)
    {
        for (int i = 0; i < gameObjectCount; i++)
        {
            if (gameObjects[i] == gameObject)
            {
                gameObjects[i] = NULL;
                break;
            }
        }

        uint16_t offset = 0;
        for (int i = 0; i < gameObjectCount; i++)
        {
            if (gameObjects[i] == NULL)
            {
                offset++;
            }
            else
            {
                gameObjects[i - offset] = gameObjects[i];
            }
        }
        gameObjectCount -= offset;

        gameObjects = (GameObject**)realloc(gameObjects, sizeof(GameObject*) * gameObjectCount);
    }
    void GameWorld::UpdateGameObjects()
    {
        for (uint16_t i = 0; i < gameObjectCount; i++)
        {
            gameObjects[i]->Update();
        }
    }
    void GameWorld::DrawGameObjects()
    {
        if (ActiveCamera)
        {
            if (((Camera*)ActiveCamera)->visible)
            {
                Shader shader = ((Camera*)ActiveCamera)->shader;
                glm::mat4 viewMatrix = ((Camera*)ActiveCamera)->viewMatrix;
                glm::mat4 projectionMatrix = ((Camera*)ActiveCamera)->projectionMatrix;
                for (uint16_t i = 0; i < gameObjectCount; i++)
                {
                    if (gameObjects[i]->visible)
                    {
                        Mesh mesh = Mesh::Empty();
                        MeshGenerator::CopyMesh(&mesh, &gameObjects[i]->mesh);
                        MeshGenerator::TransformMesh(&mesh, gameObjects[i]->mesh.GetTransform());
                        mesh.SetTransform(gameObjects[i]->transform);
                        Render->DrawMesh(mesh, 0, shader, viewMatrix, projectionMatrix);
                        mesh.Delete();
                    }
                }
            }
        }
    }
}
