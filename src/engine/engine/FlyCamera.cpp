#include "FlyCamera.hpp"

namespace lei3d
{
    FlyCamera::FlyCamera(GLFWwindow* window, float yaw, float pitch, float flySpeed)
      : window{window}, // this is a member initializer list using uniform initialization C++17 feature
        yaw{yaw},
        pitch{pitch},
        flySpeed{flySpeed},
        m_FOVDeg{45.0f},
        m_NearPlane{0.1f},
        m_FarPlane{400.0f}
    {
      firstMouse = true; // the first time the mouse is entered onto the screen, do not flick

      int screenWidth, screenHeight;
      glfwGetWindowSize(window, &screenWidth, &screenHeight);
      lastX = screenWidth / 2.0f;  
      lastY = screenHeight / 2.0f; 
      m_Aspect = (float) screenWidth / screenHeight;

      cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
      cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
      cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    FlyCamera::~FlyCamera()
    {
        // no need rn
    }

    void FlyCamera::cameraMouseCallback(GLFWwindow* window, double xPosInput, double yPosInput)
    {
        float xpos = static_cast<float>(xPosInput);
        float ypos = static_cast<float>(yPosInput);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // openGL inverted y
        lastX = xpos;
        lastY = ypos;

        const float mouseSensitivity = 0.1f;
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction) * flySpeed;
    }

    void FlyCamera::SetFOV(float fovDeg) {
        m_FOVDeg = fovDeg;
    }

    void FlyCamera::SetClipPlanes(float near, float far) {
        m_NearPlane = near;
        m_FarPlane = far;
    }

    void FlyCamera::handleForward(float deltaTime)
    {
		  cameraPos += cameraFront * flySpeed * deltaTime;
    }

    void FlyCamera::handleBack(float deltaTime)
    {
		  cameraPos -= cameraFront * flySpeed * deltaTime;
    }

    void FlyCamera::handleLeft(float deltaTime)
    {
		  cameraPos -= glm::cross(cameraFront, cameraUp) * flySpeed * deltaTime;
    }

    void FlyCamera::handleRight(float deltaTime)
    {
		  cameraPos += glm::cross(cameraFront, cameraUp) * flySpeed * deltaTime;
    }

    glm::mat4 FlyCamera::GetView() 
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    glm::mat4 FlyCamera::GetProj()
    {
        return glm::perspective(glm::radians(m_FOVDeg), 800.0f / 600.0f, m_NearPlane, m_FarPlane);
    }
}