#include "../include/transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Gengine {
    glm::mat4 TransformToMatrix(const Transform& t)
    {
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), t.scale);
        glm::quat rotQuat  = glm::quat(glm::radians(t.rotation));
        glm::mat4 rotMat   = glm::mat4_cast(rotQuat);
        glm::mat4 transMat = glm::translate(glm::mat4(1.0f), t.position);

        return transMat * rotMat * scaleMat;
    }

    Transform CombineTransforms(const Transform& parent, const Transform& child)
    {
        glm::mat4 parentMat = TransformToMatrix(parent);
        glm::mat4 childMat = TransformToMatrix(child);
        glm::mat4 combinedMat = parentMat * childMat;

        // Extract position
        glm::vec3 position = glm::vec3(combinedMat[3]);

        // Extract scale
        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(combinedMat[0]));
        scale.y = glm::length(glm::vec3(combinedMat[1]));
        scale.z = glm::length(glm::vec3(combinedMat[2]));

        // Extract rotation
        glm::mat3 rotationMat = glm::mat3(combinedMat);
        rotationMat[0] /= scale.x;
        rotationMat[1] /= scale.y;
        rotationMat[2] /= scale.z;
        glm::quat rotation = glm::quat_cast(rotationMat);
        glm::vec3 eulerRot = glm::degrees(glm::eulerAngles(rotation));

        Transform result;
        result.position = position;
        result.rotation = eulerRot;
        result.scale = scale;
        return result;
    }
}