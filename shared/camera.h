#pragma once


#ifdef LINUX_BUILD
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif // LINUX_BUILD

#ifdef WINDOWS_BUILD
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#endif // WINDOWS_BUILD


#if defined(LINUX_BUILD) || defined(WINDOWS_BUILD)

struct Camera
{
    // const values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;
    const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera(glm::vec3 p_position, glm::vec3 p_target) : position(p_position), target(p_target)
        , up(0.0f, 1.0f, 0.0f), front(0.0f, 0.0f, -1.0f), right(-1.0f, 0.0f, 0.0f)
    {
        yaw = YAW;
        pitch = PITCH;
    }

    glm::vec3 up;    // = glm::vec3(0.0f, 1.0f, 0.0f);  
    glm::vec3 front; // = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right; // = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 position;                          // Camera position
    glm::vec3 target;                            // Point camera is looking at

    glm::vec2 lastMousePos = glm::vec2(0, 0);
    bool firstMouseInput = false;

    // euler Angles
    float yaw;
    float pitch;

    float fov = 45.0f;
    float aspectRatio = 16.0f / 9.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    // Helper functions to get the view and projection matrices
    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(position, position + front, up);
    }
    glm::mat4 GetProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    glm::mat4 GetMVP() const
    {
        return GetViewMatrix() * GetProjectionMatrix() * glm::translate(glm::mat4(1.0f), position);;
    }

    void UpdateCamVecs()
    {
        // calculate the new Front vector
        glm::vec3 _front;
        _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        _front.y = sin(glm::radians(pitch));
        _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(_front);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, WORLD_UP));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, front));
    }

};

#endif // WINDOWS_BUILD