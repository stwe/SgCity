#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace sg::ogl::camera
{
    class Camera
    {
    public:
        //-------------------------------------------------
        // Type
        //-------------------------------------------------

        enum class Direction
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
            UP,
            DOWN
        };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Camera() = delete;
        Camera(const glm::vec3& t_position, float t_yaw, float t_pitch);

        Camera(const Camera& t_other) = delete;
        Camera(Camera&& t_other) noexcept = delete;
        Camera& operator=(const Camera& t_other) = delete;
        Camera& operator=(Camera&& t_other) noexcept = delete;

        ~Camera() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        auto& GetPosition() noexcept { return m_position; }
        [[nodiscard]] const auto& GetPosition() const noexcept { return m_position; }

        [[nodiscard]] auto GetViewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }

        auto& GetYaw() noexcept { return m_yaw; }
        [[nodiscard]] const auto& GetYaw() const noexcept { return m_yaw; }

        auto& GetPitch() noexcept { return m_pitch; }
        [[nodiscard]] const auto& GetPitch() const noexcept { return m_pitch; }

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCameraSpeed(const float t_speed) { m_movementSpeed = t_speed; }
        void SetMouseSensitivity(const float t_sensitivity) { m_mouseSensitivity = t_sensitivity; }
        void SetPosition(const glm::vec3& t_position) { m_position = t_position; }

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update();

        //-------------------------------------------------
        // Keyboard && Mouse
        //-------------------------------------------------

        void ProcessKeyboard(Direction t_direction, float t_dt);
        void ProcessMouse(const glm::vec2& t_displVec);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        float m_movementSpeed{ 1.0f };
        float m_mouseSensitivity{ 0.2f };

        glm::vec3 m_position{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 m_front{ glm::vec3(0.0f, 0.0f, 1.0f) };
        glm::vec3 m_up{ glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 m_worldUp{ glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 m_right{ glm::vec3(1.0f, 0.0f, 0.0f) };

        float m_yaw{ 90.0f };
        float m_pitch{ 0.0f };
    };
}
