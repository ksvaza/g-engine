#pragma once
#include <glm/glm.hpp>

namespace Gengine {
    typedef struct {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    }Transform;

    glm::mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
    glm::mat4 TransformToMatrix(const Transform& t);
    Transform CombineTransforms(const Transform& parent, const Transform& child);
}