// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include <glm/gtc/matrix_transform.hpp>

//-------------------------------------------------
// Camera
//-------------------------------------------------

namespace sg::ogl::camera
{
    class Camera
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        static constexpr auto MOVEMENT_SPEED{ 1.0f / 60.0f / 16.0f };

        static constexpr auto MOUSE_SENSITIVITY{ 0.2f };

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
        explicit Camera(const glm::vec3& t_position);
        Camera(const glm::vec3& t_position, float t_yaw, float t_pitch);

        Camera(const Camera& t_other) = delete;
        Camera(Camera&& t_other) noexcept = delete;
        Camera& operator=(const Camera& t_other) = delete;
        Camera& operator=(Camera&& t_other) noexcept = delete;

        ~Camera() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        auto& GetPosition() { return m_position; }
        [[nodiscard]] const auto& GetPosition() const { return m_position; }

        [[nodiscard]] auto GetViewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }

        auto& GetYaw() { return m_yaw; }
        [[nodiscard]] const auto& GetYaw() const { return m_yaw; }

        auto& GetPitch() { return m_pitch; }
        [[nodiscard]] const auto& GetPitch() const { return m_pitch; }

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCameraSpeed(const float t_speed) { m_movementSpeed = t_speed; }
        void SetMouseSensitivity(const float t_sensitivity) { m_mouseSensitivity = t_sensitivity; }
        void SetPosition(const glm::vec3& t_position) { m_position = t_position; }

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void InvertPitch()
        {
            m_pitch = -m_pitch;
            Update();
        }

        void Update();

        //-------------------------------------------------
        // Keyboard && Mouse
        //-------------------------------------------------

        void ProcessKeyboard(Direction t_direction);
        void ProcessMouse(const glm::vec2& t_displVec);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        float m_movementSpeed{ MOVEMENT_SPEED };
        float m_mouseSensitivity{ MOUSE_SENSITIVITY };

        glm::vec3 m_position{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 m_front{ glm::vec3(0.0f, 0.0f, 1.0f) };
        glm::vec3 m_up{ glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 m_worldUp{ glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 m_right{ glm::vec3(1.0f, 0.0f, 0.0f) };

        float m_yaw{ 90.0f };
        float m_pitch{ 0.0f };
    };
}
