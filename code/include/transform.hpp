#pragma once
#include <glm/glm.hpp>

namespace Gengine {
    typedef struct {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    }Transform;
}