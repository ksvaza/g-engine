#include "../include/camera.hpp"
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace Gengine
{
    void Camera::Create()
    {
        mesh = Mesh::Empty();
        transform = NewTransform();
        visible = 1;
        fov = 90.0;
        aspectRatio = 16.0 / 9.0;
        nearClip = 0.1;
        farClip = 1000.0;
        Update();
    }
    void Camera::Update()
    {
        UpdateView();
        UpdateProjection();
    }
    void Camera::UpdateView()
    {
        // Calculate the rotation matrix from Euler angles (stored in degrees)
        glm::mat4 rotationMatrix = glm::yawPitchRoll(
            glm::radians(transform.rotation.y),
            glm::radians(transform.rotation.x),
            glm::radians(transform.rotation.z)
        );
        // Obtain the forward vector by transforming the default forward (-Z) direction
        glm::vec3 forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        // Obtain the up vector by transforming the default up (+Y) direction
        glm::vec3 up = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
        // Create the view matrix using the calculated direction vectors
        viewMatrix = glm::lookAt(transform.position, transform.position + forward, up);
        
        // Create a scaling matrix
        /*glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);
        // Adjust for rotation of the scaling axes
        scaleMatrix = glm::yawPitchRoll(
            glm::radians(transform.rotation.y),
            glm::radians(transform.rotation.x),
            glm::radians(transform.rotation.z)
        ) * scaleMatrix;
        // Apply the scaling matrix to the view matrix
        viewMatrix = viewMatrix * scaleMatrix;*/
    }
    void Camera::UpdateProjection()
    {
        // Create the projection matrix using the stored field of view, aspect ratio, and near/far clip planes
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    }
}
