#pragma once
#include <mesh.hpp>
#include <transform.hpp>

namespace Gengine
{
    class GameObject
    {
    public:
        Mesh mesh;
        Transform transform;

        void Create();
        void Delete();
    };

    class Game
    {
    public:
        GameObject** gameObjects;
        uint16_t gameObjectCount;

        void AddGameObject(GameObject* gameObject);
        void RemoveGameObject(GameObject* gameObject);
        void DrawGameObjects();
    };
}