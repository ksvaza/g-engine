#pragma once
#include <mesh.hpp>
#include <transform.hpp>
#include <render.hpp>

namespace Gengine
{
    class GameObject
    {
    public:
        Mesh mesh;
        Transform transform;

        virtual void Update();

        uint8_t visible;

        virtual void Create();
        virtual void Delete();
        int BakeTexture();
        int BakeTextures();
    };

    class GameWorld
    {
    public:
        GameObject** gameObjects = NULL;
        uint16_t gameObjectCount = 0;
        Renderer* Render = NULL;
        void* ActiveCamera = NULL;

        void AddGameObject(GameObject* gameObject);
        void RemoveGameObject(GameObject* gameObject);
        void UpdateGameObjects();
        void DrawGameObjects();
    };
}