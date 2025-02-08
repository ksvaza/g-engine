#pragma once
#include <mesh.hpp>
#include <shader.hpp>
#include <glm/glm.hpp>

namespace Gengine
{
    class Renderer
    {
    private:
        
    public:
        int DrawMesh(Mesh mesh, char wireFrame, Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
        int DrawBoundingBox(Mesh mesh, Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
        void Clear();
    };
}
