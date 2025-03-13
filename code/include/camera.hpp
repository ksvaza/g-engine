#pragma once
#include <game.hpp>
#include <shader.hpp>
#include <glm/glm.hpp>

namespace Gengine
{
    class Camera : public GameObject
    {
    public:
        void Create() override;
        void Update() override;
        void UpdateView();
        void UpdateProjection();
        
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        float fov = 90.0;
        float aspectRatio = 16.0 / 9.0;
        float nearClip = 0.1;
        float farClip = 100.0;
        Shader shader;
    };
}